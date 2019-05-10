#pragma once

#include "bluetooth/characteristics/gatt_characteristic.h"
#include "bluetooth/services/gatt_service.h"
#include "types/unit.h"
#include <stdint.h>
#include <algorithm>

//0x2A19 -> org.bluetooth.characteristic.battery_level
class BatteryLevelCharacteristic : public GattCharacteristic<
    BTOrgCharType<0x2A19>,
    uint8_t,
    CharValChangeNotify,
    CharReadable<PropSecureNone>> {

public:
  void setValue(uint8_t newValue) {
    GattCharacteristic::setValue( std::clamp(newValue, static_cast<uint8_t>(0),
        static_cast<uint8_t>(100)) );
  }
};