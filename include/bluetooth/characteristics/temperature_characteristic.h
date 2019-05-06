#pragma once

extern "C" {
}
#include "bluetooth/characteristics/gatt_characteristic.h"
#include "bluetooth/services/gatt_service.h"

//0x2A1F -> Temperature Celsius org.bluetooth.characteristic.temperature_celsius
class TemperatureCharacteristic : public GattCharacteristic<
    0x2A1F,
    int16_t,
    CharValChangeNotify,
    CharReadable<PropSecureNone>> {
};
