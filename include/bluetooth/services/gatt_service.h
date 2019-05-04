#pragma once

extern "C" {
#include "ble_types.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_log.h"
}
#include <vector>
#include <stdint.h>

using ServicesUidsVect = std::vector<ble_uuid_t>;
using uint128_t = uint8_t[16];

template<ble_uuid128_t const& UUID_BASE, uint16_t serviceUid, typename Stack>
class GattService {
public:
  void enable(Stack& stack) {
    ret_code_t err_code = sd_ble_uuid_vs_add(this->getBaseUid(), &this->uuid.type);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &this->uuid,
                                        &this->handle);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEBUG("Enabling RoomStateService");
    NRF_LOG_DEBUG("    UUID: 0x%#04x", this->uuid.uuid);
    NRF_LOG_DEBUG("    Type: 0x%#02x", this->uuid.type);
    NRF_LOG_DEBUG("  Handle: 0x%#04x", this->handle);
  }

  void disable(Stack& stack) {
    //TODO: Description of this function suggests problems...
    ret_code_t err_code = sd_ble_uuid_vs_remove(&this->uuid.type);
    APP_ERROR_CHECK(err_code);
  }

  ble_uuid128_t* getBaseUid() { return &baseUid; }

  uint16_t getHandle() { return handle; }

protected:
  ble_uuid128_t baseUid{UUID_BASE};
  uint16_t handle {};
  ble_uuid_t uuid {serviceUid, 0};
};
