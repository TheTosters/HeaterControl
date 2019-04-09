#pragma once

#include "bridges/i2c_bridge.h"
#include <stdint.h>
#include <cstddef>

class Sht30 {
public:

//    struct episolon05 {
//        constexpr static float epsilon = 0.5f;
//    };

    template<typename T>
    struct default_trait
    {
        constexpr static T epsilon = std::numeric_limits<T>::epsilon();
    };
    template <typename T, typename Tag, typename Traits = default_trait<T>>
    class unit {
    private:
        constexpr static T epsilon = Traits::epsilon;
        T value;
    public:
        explicit unit(T value) : value(value) {};

        explicit operator T() {
            return value;
        }

        explicit operator T() const {
            return value;
        }

        bool operator ==(const unit& rhs) const noexcept {

            if constexpr (std::is_floating_point<T>::value)
                return (*this-rhs).abs() < unit(epsilon);
            else
                return this->value == rhs.value;
        }

        bool operator !=(const unit& rhs) const noexcept {
            return not (*this == rhs) ;
        }

        bool operator <(const unit& rhs) const noexcept {
            return this->value < rhs.value;
        }

        bool operator >=(const unit& rhs) const noexcept {
            return not (*this < rhs);
        }

        bool operator <=(const unit& rhs) const noexcept {
            return *this < rhs || *this == rhs;
        }

        bool operator >(const unit& rhs) const noexcept {
            return *this >= rhs && *this != rhs;
        }

        unit operator +(const unit& rhs) const noexcept {
            return unit(this->value + rhs.value);
        }

        unit operator -(const unit& rhs) const noexcept {
            return unit(this->value - rhs.value);
        }

        unit operator *(const int& rhs) const noexcept {
            return unit(this->value * rhs);
        }

        unit operator *(const float& rhs) const noexcept {
            return unit(this->value * rhs);
        }

        unit abs() const noexcept {
            return unit(std::abs(this->value));
        }

    };

    using tempC = unit<float, struct tempC_tag>;
    using relHum = unit<int, struct relHum_tag>;

    enum Sht30Mode {
        Sht30Mode_Single_HighRep_ClockStretch,
        Sht30Mode_Single_MediumRep_ClockStretch,
        Sht30Mode_Single_LowRep_ClockStretch,
        Sht30Mode_Single_HighRep_NoClockStretch,
        Sht30Mode_Single_MediumRep_NoClockStretch,
        Sht30Mode_Single_LowRep_NoClockStretch
    };

    enum Sht30Address {
        Sht30Address_VSS = 0x44,
        Sht30Address_VDD = 0x45,
    };

private:
    constexpr static uint8_t readLength = 6;

    I2c_Bridge& bridge;
    Sht30Address address;
    Sht30Mode mode;
    uint8_t measLSB;
    uint8_t measMSB;
    tempC temperature{0};
    relHum relHumidity{0};

    struct Readings {
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
    uint8_t CalculateCrc8 (uint8_t init, uint8_t byte) {
        uint8_t crc = 0xFF;
        crc ^= byte;
        for (int i = 0; i < 8; i++)
        {
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
        }
        return crc;
    }

    bool IsCrc8Valid(Readings readings) {
        return    CalculateCrc8(CalculateCrc8(0xFF, readings.tempMSB), readings.tempLSB) == readings.tempCRC
               && CalculateCrc8(CalculateCrc8(0xFF, readings.humMSB), readings.humLSB) == readings.humCRC;
    }

public:

    Sht30 (I2c_Bridge& bridge, Sht30Mode mode = Sht30Mode_Single_HighRep_ClockStretch, Sht30Address address = Sht30Address_VSS) :
        bridge(bridge), mode(mode), address(address) {

        SetMode(this->mode);

        if (this->address != Sht30Address_VSS || this->address != Sht30Address_VDD)
        {
            this->address = Sht30Address_VSS;
        }
    }

    Sht30(const Sht30&) = delete;
    Sht30& operator=(const Sht30&) = delete;

    void SetMode(Sht30Mode mode) {
        switch (mode)
        {
            case Sht30Mode_Single_HighRep_ClockStretch:
            {
                measMSB=0x2C;
                measLSB=0x06;
                break;
            }
            case Sht30Mode_Single_MediumRep_ClockStretch:
            {
                measMSB=0x2C;
                measLSB=0x0D;
                break;
            }
            case Sht30Mode_Single_LowRep_ClockStretch:
            {
                measMSB=0x2C;
                measLSB=0x10;
                break;
            }
            case Sht30Mode_Single_HighRep_NoClockStretch:
            {
                measMSB=0x24;
                measLSB=0x00;
                break;
            }
            case Sht30Mode_Single_MediumRep_NoClockStretch:
            {
                measMSB=0x24;
                measLSB=0x0B;
                break;
            }
            case Sht30Mode_Single_LowRep_NoClockStretch:
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

    void RequestMeasurements() {
        bridge.send(address, measMSB, measLSB);
        auto readings = bridge.read<Readings>(address);
        if (IsCrc8Valid(readings)) {
            temperature = static_cast<tempC>((readings.tempMSB<<8 + readings.tempLSB) * 0.00267033 - 45.0);
            relHumidity = static_cast<relHum>((readings.humMSB<<8 + readings.humLSB) * 0.0015259);
        }
    }

    tempC GetTemperature() {
        return temperature;
    }
    relHum GetRelHumidity() {
        return relHumidity;
    }

    void HeaterOn() {
        bridge.send(address, 0x30, 0x6D);
    }

    void HeaterOff() {
        bridge.send(address, 0x30, 0x66);
    }

    void SoftReset() {
        bridge.send(address, 0x30, 0xA2);
    }
};
