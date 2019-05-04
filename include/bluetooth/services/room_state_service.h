#pragma once

extern "C" {
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_log.h"
}
#include "bluetooth/services/gatt_service.h"
#include "bluetooth/characteristics/temperature_characteristic.h"

#include <stdint.h>

template<typename Stack>
class RoomStateService : public GattService<Stack> {
  public:
    static constexpr uint16_t ROOM_STATE_SERVICE {0xABCD};
    static constexpr ble_uuid128_t UUID_BASE {
      {0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
    };

    void enable(Stack& stack) {
      serviceUuid.uuid = ROOM_STATE_SERVICE;
      ret_code_t err_code = sd_ble_uuid_vs_add(&UUID_BASE, &serviceUuid.type);
      APP_ERROR_CHECK(err_code);

      err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                          &serviceUuid,
                                          &serviceHandle);
      APP_ERROR_CHECK(err_code);

      NRF_LOG_DEBUG("Enabling RoomStateService");
      NRF_LOG_DEBUG("    UUID: 0x%#04x", serviceUuid.uuid);
      NRF_LOG_DEBUG("    Type: 0x%#02x", serviceUuid.type);
      NRF_LOG_DEBUG("  Handle: 0x%#04x", serviceHandle);
    }

    void disable(Stack& stack) {
      //TODO: Description of this function suggests problems...
      ret_code_t err_code = sd_ble_uuid_vs_remove(&serviceUuid.type);
      APP_ERROR_CHECK(err_code);
    }

    void collectServicesUids(ServicesUidsVect& collection) {
      ble_uuid_t data{ROOM_STATE_SERVICE, BLE_UUID_TYPE_VENDOR_BEGIN};
      collection.emplace_back(data);
    }

  protected:
    uint16_t serviceHandle;
    ble_uuid_t serviceUuid; //TODO: Move to parent class

    TemperatureCharacteristic tempChar; //TODO: temporary for experiments
};
