#pragma once

#include "bluetooth/services/gatt_service.h"
#include "bluetooth/characteristics/current_time_characteristic.h"
#include <tuple>
#include <stdint.h>

//Current Time Service  org.bluetooth.service.current_time  0x1805
template<typename Stack>
class CurrentTimeService : public GattService<
  BTOrgServiceBase<0x1805>,
  Stack,
  CurrentTimeCharacteristic> {

  public:
    explicit CurrentTimeService() {
      this->connHandlerDelegate = [this](uint16_t connHandle){
        std::apply([this, connHandle](auto&&... args) {
                ((args.setConnectionHandle(connHandle)), ...);
            }, this->characteristics);
      };
    }

    void collectServicesUids(ServicesUidsVect& collection) {
      ble_uuid_t data{this->uuid.uuid, BLE_UUID_TYPE_BLE};
      collection.emplace_back(data);
    }
};
