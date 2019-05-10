#pragma once

#include "bluetooth/characteristics/gatt_characteristic.h"
#include "bluetooth/services/gatt_service.h"
#include "types/unit.h"
#include <stdint.h>
#include <algorithm>

//0x2A2B -> org.bluetooth.characteristic.current_time
class CurrentTimeCharacteristic : public GattCharacteristic<
    BTOrgCharType<0x2A2B>,
    uint8_t,  //TODO: implement org.bluetooth.characteristic.exact_time_256
    CharValChangeNotify,
    CharReadable<PropSecureNone>> {

public:
  //TODO: implement org.bluetooth.characteristic.exact_time_256
  void setValue(const uint8_t& newValue) {
    GattCharacteristic::setValue( newValue );
  }
};
