#pragma once

extern "C" {
}

#include <stdint.h>

template<uint16_t charUID>
class GattCharacteristic {
public:
protected:
    ble_uuid_t charUuid {charUID, 0};
};
