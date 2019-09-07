#pragma once

#include "bluetooth/services/gatt_service.h"
#include "bluetooth/characteristics/measurement_interval_characteristic.h"
#include "bluetooth/characteristics/device_name_characteristic.h"
#include <tuple>
#include <stdint.h>

//Device Information  org.bluetooth.service.device_information  0x180A
template<typename Stack>
class DeviceInformationService : public GattService<
  BTOrgServiceBase<0x180A>,
  Stack,
  DeviceNameCharacteristic, MeasurementIntervalCharacteristic> {

  public:
    explicit DeviceInformationService() {
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
