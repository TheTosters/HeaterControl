#pragma once

extern "C" {
}
#include "bluetooth/characteristics/gatt_characteristic.h"
#include "bluetooth/services/gatt_service.h"
#include "types/unit.h"
#include <stdint.h>

//0x2A1F -> Temperature Celsius org.bluetooth.characteristic.temperature_celsius
//0x2A6E -> Temperature org.bluetooth.characteristic.temperature
class TemperatureCharacteristic : public GattCharacteristic<
    0x2A6E,
    int16_t,
    CharValChangeNotify,
    CharReadable<PropSecureNone>> {

public:
  void setValue(const TemperatureC& newValue) {
    float tmp = static_cast<float>(newValue);
    int16_t fixed = static_cast<int16_t>(100 * tmp);
    GattCharacteristic::setValue(fixed);
  }
};
