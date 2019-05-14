#pragma once

#include "bluetooth/characteristics/gatt_characteristic.h"
#include "bluetooth/services/gatt_service.h"
#include "types/unit.h"
#include <stdint.h>
#include <string>

//0x2A00 -> org.bluetooth.characteristic.gap.device_name
class DeviceNameCharacteristic : public GattCharacteristic<
    BTOrgCharType<0x2A00>,
    std::string,
    CharValChangeNotify,
    CharReadable<PropSecureNone>,
    CharWritable<PropSecureNone>> {

public:
  void setValue(const std::string& newValue) {
    GattCharacteristic::setValue(newValue);
  }
};
