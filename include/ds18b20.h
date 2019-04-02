#pragma once

#include "one_wire.h"
#include <stdint.h>
#include <algorithm>
#include <array>
extern "C" {
#include "boards.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"
}

//#define DS_DEBUG 1

//based on https://github.com/RobTillaart/Arduino/blob/master/libraries/DS18B20/DS18B20.cpp
class Ds18b20 {
public:
  enum Resolution {
    Res9Bit =  0x1F,
    Res10Bit = 0x3F,
    Res11Bit = 0x5F,
    Res12Bit = 0x7F
  };

  static constexpr float DEVICE_DISCONNECTED = -127.0f;

  Ds18b20(OneWire& onwWire) : wire(onwWire) {}

  bool begin() {
    wire.resetSearch();
    wire.search(deviceAddress, true);
#ifdef DS_DEBUG
    NRF_LOG_ERROR("Device address:");
    for(auto i = deviceAddress.begin(); i != deviceAddress.end(); i++) {
      NRF_LOG_ERROR("0x%x", *i);
    }
#endif
    return wire.crc8(deviceAddress.data(), 7) != deviceAddress[7];
  }

  void setResolution(Resolution res) {
    wire.reset();
    wire.select(deviceAddress);
    wire.write(WRITESCRATCH);
    // two dummy values for LOW & HIGH ALARM
    wire.write(0);
    wire.write(100);
    wire.write(res);
    wire.reset();
  }

  void requestTemperatures() {
    wire.reset();
    wire.skip();
    wire.write(STARTCONVO, false);
  }

  float getTempC() {
    while(not isConversionComplete()) {
      nrf_delay_us(5);
    }
    ScratchPad scratchPad;
    readScratchPad(scratchPad, 2);

    int rawTemperature = scratchPad[TEMP_MSB];
    rawTemperature <<= 8;
    rawTemperature |= scratchPad[TEMP_LSB];

    float temp = 0.0625f * rawTemperature;
    return temp < -55 ? DEVICE_DISCONNECTED : temp;
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

    for(uint8_t i = 0; i < fields; i++) {
      scratchPad[i] = wire.read();
    }
#ifdef DS_DEBUG
    NRF_LOG_ERROR("scratchPad:");
    for(auto i = scratchPad.begin(); i != scratchPad.begin() + fields; i++) {
      NRF_LOG_ERROR("%x", *i);
    }
#endif
    wire.reset();
  }
};
