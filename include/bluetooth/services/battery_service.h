#pragma once

extern "C" {
}
#include "bluetooth/services/gatt_service.h"
#include "bluetooth/characteristics/battery_level_characteristic.h"
#include <tuple>
#include <stdint.h>

//static constexpr ble_uuid128_t ROOM_STATE_UUID_BASE {
//  {//Random uuid
//    0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x21, 0x15,
//    0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00
//  }
//};

template<typename Stack>
class BatteryService : public GattService<
  //CustomServiceBase<ROOM_STATE_UUID_BASE, 0x180F>,
  BTOrgServiceBase<0x180F>,
  Stack,
  BatteryLevelCharacteristic> {

  public:
    explicit BatteryService() {
      this->connHandlerDelegate = [this](uint16_t connHandle){
        std::apply([this, connHandle](auto&&... args) {
                ((args.setConnectionHandle(connHandle)), ...);
            }, this->characteristics);
      };
    }

    void collectServicesUids(ServicesUidsVect& collection) {
      //ble_uuid_t data{this->uuid.uuid, BLE_UUID_TYPE_VENDOR_BEGIN};
      ble_uuid_t data{this->uuid.uuid, BLE_UUID_TYPE_BLE};
      collection.emplace_back(data);
    }
};
