#pragma once

extern "C" {
}
#include "bluetooth/characteristics/gatt_characteristic.h"
#include "bluetooth/services/gatt_service.h"
#include "types/unit.h"
#include <stdint.h>

//0x2A6F -> org.bluetooth.characteristic.humidity
class HumidityCharacteristic : public GattCharacteristic<
    0x2A6F,
    uint16_t,
    CharValChangeNotify,
    CharReadable<PropSecureNone>> {

public:
  void setValue(const RelativeHumidity& newValue) {
    int tmp = static_cast<int>(newValue);
    GattCharacteristic::setValue(static_cast<uint16_t>(tmp));
  }
};
