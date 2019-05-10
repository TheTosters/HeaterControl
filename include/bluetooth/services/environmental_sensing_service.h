#pragma once

#include "bluetooth/services/gatt_service.h"
#include "bluetooth/characteristics/temperature_characteristic.h"
#include "bluetooth/characteristics/humidity_characteristic.h"
#include <tuple>
#include <stdint.h>

template<typename Stack>
class EnvironmentalSensingService : public GattService<
  BTOrgServiceBase<0x181A>,
  Stack,
  TemperatureCharacteristic, HumidityCharacteristic> {

  public:
    explicit EnvironmentalSensingService() {
      this->connHandlerDelegate = [this](uint16_t connHandle){
        this->connHandle = connHandle;
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

    uint16_t getConnectionHandle() {
      return connHandle;
    }
  private:
    uint16_t connHandle {BLE_CONN_HANDLE_INVALID};
};
