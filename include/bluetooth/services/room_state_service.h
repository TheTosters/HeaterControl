#pragma once

extern "C" {
}
#include "bluetooth/services/gatt_service.h"
#include "bluetooth/characteristics/temperature_characteristic.h"

#include <stdint.h>
static constexpr ble_uuid128_t ROOM_STATE_UUID_BASE {
  { 0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15,
    0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00
  }
};

template<typename Stack>
class RoomStateService : public GattService<ROOM_STATE_UUID_BASE, 0xABCD, Stack> {
  public:
    void collectServicesUids(ServicesUidsVect& collection) {
      ble_uuid_t data{this->uuid.uuid, BLE_UUID_TYPE_VENDOR_BEGIN};
      collection.emplace_back(data);
    }

  protected:
    //TODO: Ask Piter about this construct, kind of stink?
    TemperatureCharacteristic tempChar{*this}; //TODO: temporary for experiments
};
