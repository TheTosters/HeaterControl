#pragma once

#include "bluetooth/characteristics/gatt_characteristic.h"
#include "bluetooth/services/gatt_service.h"
#include "types/unit.h"
#include <stdint.h>
#include <algorithm>

//0x2A21 -> org.bluetooth.characteristic.measurement_interval
class BatteryLevelCharacteristic : public GattCharacteristic<
    BTOrgCharType<0x2A21>,
    uint16_t,
    CharValChangeNotify,
    CharReadable<PropSecureNone>> {

public:
  void setValue(const uint16_t& newValue) {
    GattCharacteristic::setValue( std::clamp(newValue, 1, 65535) );
  }
};
