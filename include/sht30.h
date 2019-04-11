#pragma once

#include "bridges/i2c_bridge.h"
#include "unit.h"
#include <stdint.h>

    enum class Sht30Mode {
        Single_HighRep_ClockStretch,
        Single_MediumRep_ClockStretch,
        Single_LowRep_ClockStretch,
        Single_HighRep_NoClockStretch,
        Single_MediumRep_NoClockStretch,
        Single_LowRep_NoClockStretch
    };

    enum class Sht30Address {
        VSS = 0x44,
        VDD = 0x45,
    };

template<Sht30Mode Mode = Sht30Mode::Single_HighRep_ClockStretch, Sht30Address Address = Sht30Address::VSS>
class Sht30 {
public:

private:
    constexpr static uint8_t readLength = 6;

    I2c_Bridge& bridge;
    Sht30Address address;
    Sht30Mode mode;
    uint8_t measLSB{0};
    uint8_t measMSB{0};
    TemperatureC temperature{0};
    RelativeHumidity relHumidity{0};

    struct __attribute__ ((packed)) Readings {
        constexpr static size_t size = 6;
        uint8_t tempMSB;
        uint8_t tempLSB;
        uint8_t tempCRC;
        uint8_t humMSB;
        uint8_t humLSB;
        uint8_t humCRC;
    };

    static_assert(Readings::size == 6  && offsetof(Readings, humCRC) == Readings::size-1, "Wrong size!");

    /*
    *   Name  : CRC-8
    *   Poly  : 0x31    x^8 + x^5 + x^4 + 1
    *   Init  : 0xFF
    *   Revert: false
    *   XorOut: 0x00
    *   Check : for 0xBE,0xEF CRC is 0x92
    */
    uint8_t calculateCrc8 (uint8_t crc, uint8_t byte) {
        crc ^= byte;
        for (int i = 0; i < 8; i++)
        {
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
        }
        return crc;
    }

    bool isCrc8Valid(Readings readings) {
        return    calculateCrc8(calculateCrc8(0xFF, readings.tempMSB), readings.tempLSB) == readings.tempCRC
              and calculateCrc8(calculateCrc8(0xFF, readings.humMSB), readings.humLSB)  == readings.humCRC;
    }

    void sendCmd(uint8_t MSB, uint8_t LSB) {
        bridge.send(static_cast<uint8_t>(address), MSB, LSB);
    }

    void heaterOn() {
        sendCmd(0x30, 0x6D);
    }

    void heaterOff() {
        sendCmd(0x30, 0x66);
    }

    void softReset() {
        sendCmd(0x30, 0xA2);
    }

public:
    Sht30 (I2c_Bridge& bridge) :
        bridge(bridge), mode(Mode), address(Address) {

        setMode(this->mode);
    }

    Sht30(const Sht30&) = delete;
    Sht30& operator=(const Sht30&) = delete;

    void setMode(Sht30Mode mode) {
        switch (mode)
        {
            case Sht30Mode::Single_HighRep_ClockStretch:
            {
                measMSB=0x2C;
                measLSB=0x06;
                break;
            }
            case Sht30Mode::Single_MediumRep_ClockStretch:
            {
                measMSB=0x2C;
                measLSB=0x0D;
                break;
            }
            case Sht30Mode::Single_LowRep_ClockStretch:
            {
                measMSB=0x2C;
                measLSB=0x10;
                break;
            }
            case Sht30Mode::Single_HighRep_NoClockStretch:
            {
                measMSB=0x24;
                measLSB=0x00;
                break;
            }
            case Sht30Mode::Single_MediumRep_NoClockStretch:
            {
                measMSB=0x24;
                measLSB=0x0B;
                break;
            }
            case Sht30Mode::Single_LowRep_NoClockStretch:
            {
                measMSB=0x24;
                measLSB=0x16;
                break;
            }
            default:
            {
                measMSB=0x2C;
                measLSB=0x06;
                break;
            }
        }
    }

    void requestMeasurements() {
        sendCmd(measMSB, measLSB);
        auto readings = bridge.read<Readings>(static_cast<uint8_t>(address));
        if (isCrc8Valid(readings)) {
            temperature = TemperatureC((readings.tempMSB*256 + readings.tempLSB) * 0.00267033 - 45.0);
            relHumidity = RelativeHumidity((readings.humMSB*256 + readings.humLSB) * 0.0015259);
        }
    }

    TemperatureC getTemperature() {
        return temperature;
    }
    RelativeHumidity getRelHumidity() {
        return relHumidity;
    }

    void configure() {
    }
};
