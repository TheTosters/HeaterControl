#pragma once

#include "bluetooth/characteristics/gatt_characteristic.h"
#include "bluetooth/services/gatt_service.h"
#include "types/unit.h"
#include <stdint.h>
#include <algorithm>

//0x2A21 -> org.bluetooth.characteristic.measurement_interval
class MeasurementIntervalCharacteristic : public GattCharacteristic<
    BTOrgCharType<0x2A21>,
    uint16_t,
    CharValChangeNotify,
    CharReadable<PropSecureNone>> {

public:
  void setValue(const uint16_t& newValue) {
    GattCharacteristic::setValue( std::clamp(newValue, static_cast<uint16_t>(1),
        static_cast<uint16_t>(65535)) );
  }
};
