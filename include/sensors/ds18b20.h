#pragma once

#include "bridges/one_wire.h"
#include "types/unit.h"
#include <stdint.h>
#include <algorithm>
#include <array>
extern "C" {
#include "boards.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"
}

#define DS_DEBUG 1

enum Ds18b20_Resolution {
  Res9Bit =  0x1F,
  Res10Bit = 0x3F,
  Res11Bit = 0x5F,
  Res12Bit = 0x7F
};

//based on https://github.com/RobTillaart/Arduino/blob/master/libraries/DS18B20/DS18B20.cpp
template<Ds18b20_Resolution Resolution = Res9Bit>
class Ds18b20 {
public:
  //should be maximum of power up for DS18B20 and/or SHT30
  static constexpr unsigned int CONFIGURE_DELAY_MS = 30;

  //should be maximum time of measure for DS18B20 and/or SHT30
  static constexpr unsigned int MEASURING_DELAY_MS = 95;

  static constexpr float DEVICE_DISCONNECTED = -127.0f;

  Ds18b20(OneWire& onwWire) : wire(onwWire) {}

  Ds18b20(const Ds18b20&) =  delete;
  Ds18b20& operator=(const Ds18b20&) = delete;

  bool enable() {
    wire.resetSearch();
    deviceAddress = wire.search(true);
#ifdef DS_DEBUG
    NRF_LOG_ERROR("Device address:");
    for(auto i = deviceAddress.begin(); i != deviceAddress.end(); i++) {
      NRF_LOG_ERROR("0x%x", *i);
    }
#endif
    return wire.crc8(deviceAddress.data(), 7) != deviceAddress[7];
  }

  void setResolution(Ds18b20_Resolution res) {
    wire.reset();
    wire.select(deviceAddress);
    wire.write(WRITESCRATCH);
    // two dummy values for LOW & HIGH ALARM
    wire.write(0);
    wire.write(100);
    wire.write(res);
    wire.reset();
  }

  void configure() {
      enable();
      setResolution(Resolution);
  }

  static std::string getName() {
    return "DS18B20";
  }

  void requestMeasurements() {
    wire.reset();
    wire.skip();
    wire.write(STARTCONVO, false);
  }

  TemperatureC getTemperature() {
    while(not isConversionComplete()) {
      nrf_delay_us(5);
    }
    ScratchPad scratchPad;
    readScratchPad(scratchPad, 2);

    int rawTemperature = scratchPad[TEMP_MSB];
    rawTemperature <<= 8;
    rawTemperature |= scratchPad[TEMP_LSB];

    TemperatureC temp(0.0625f * rawTemperature);
    return temp < TemperatureC(-55) ? TemperatureC(DEVICE_DISCONNECTED) : temp;
  }

  RelativeHumidity getRelHumidity() {
      return RelativeHumidity(0);
  }

  bool isConversionComplete() {
    return wire.read_bit() == 1;
  }

private:
  enum ScratchPadLocations{
    TEMP_LSB = 0,
    TEMP_MSB = 1,
    HIGH_ALARM_TEMP = 2,
    LOW_ALARM_TEMP = 3,
    CONFIGURATION = 4,
    INTERNAL_BYTE = 5,
    COUNT_REMAIN = 6,
    COUNT_PER_C = 7,
    SCRATCHPAD_CRC = 8
  };
  enum Commands {
    STARTCONVO = 0x44,
    READSCRATCH = 0xBE,
    WRITESCRATCH = 0x4E
  };

  using ScratchPad = std::array<uint8_t, 9>;

  OneWire::DeviceAddress deviceAddress;
  OneWire& wire;

  void readScratchPad(ScratchPad& scratchPad, uint8_t fields) {
    wire.reset();
    wire.select(deviceAddress);
    wire.write(READSCRATCH);

    std::generate_n(std::begin(scratchPad),
        std::min( static_cast<size_t>(fields), scratchPad.size()),
        [&](){ return wire.read();});
//    for(uint8_t i = 0; i < fields; i++) {
//      scratchPad[i] = wire.read();
//    }
#ifdef DS_DEBUG
    NRF_LOG_ERROR("scratchPad:");
    for(auto i = scratchPad.begin(); i != scratchPad.begin() + fields; i++) {
      NRF_LOG_ERROR("%x", *i);
    }
#endif
    wire.reset();
  }
};
