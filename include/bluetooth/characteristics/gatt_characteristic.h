#pragma once

extern "C" {
}

#include <stdint.h>

template<uint16_t charUID>
class GattCharacteristic {
public:
protected:
    ble_uuid_t uuid {charUID, 0};
    ble_gatts_char_handles_t handles {};
};
