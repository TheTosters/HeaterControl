#pragma once

extern "C" {
}
#include "bluetooth/services/gatt_service.h"
#include "bluetooth/characteristics/temperature_characteristic.h"
#include "bluetooth/characteristics/humidity_characteristic.h"
#include <tuple>
#include <stdint.h>

static constexpr ble_uuid128_t ROOM_STATE_UUID_BASE {
//{//Random uuid
//  0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15,
//  0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00
//}
  {//Environmental Sensing UUID
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x00, 0x00, 0x1a, 0x18, 0x00, 0x00

//    0x00, 0x00, 0x18, 0x1a, 0x00, 0x00, 0x10, 0x00,
//    0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb,
  }
};

template<typename Stack>
class RoomStateService : public GattService<BTOrgServiceBase<0x181A>, Stack
  ,TemperatureCharacteristic, HumidityCharacteristic> {

  public:
    explicit RoomStateService() {
      this->connHandlerDelegate = [this](uint16_t connHandle){
        std::apply([this, connHandle](auto&&... args) {
                ((args.setConnectionHandle(connHandle)), ...);
            }, this->characteristics);
      };
    }

    void collectServicesUids(ServicesUidsVect& collection) {
      //ble_uuid_t data{this->uuid.uuid, BLE_UUID_TYPE_VENDOR_BEGIN};
      //ble_uuid_t data{this->uuid.uuid, BLE_UUID_TYPE_VENDOR_BEGIN};
      ble_uuid_t data{this->uuid.uuid, BLE_UUID_TYPE_BLE};
      collection.emplace_back(data);
    }
};
