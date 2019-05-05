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

class GattServiceBase {
  public:
    GattServiceBase(const ble_uuid128_t& uid,
                    const uint16_t& serviceUid)
    : baseUid(uid), uuid({serviceUid, 0}) { }

    ble_uuid128_t* getBaseUid() { return &baseUid; }

    uint16_t getHandle() { return handle; }

    uint16_t getConnectionHandle() {return connectionHandle;}
  protected:
    ble_uuid128_t baseUid {};
    uint16_t handle {};
    ble_uuid_t uuid {};
    uint16_t connectionHandle {BLE_CONN_HANDLE_INVALID};
};

template<ble_uuid128_t const& UUID_BASE, uint16_t serviceUid, typename Stack>
class GattService : public GattServiceBase {
public:
  GattService() : GattServiceBase(UUID_BASE, serviceUid) {}

  void enable(Stack& stack) {
    ret_code_t err_code = sd_ble_uuid_vs_add(&baseUid, &uuid.type);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &uuid,
                                        &handle);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEBUG("Enabling Service");
    NRF_LOG_DEBUG("    UUID: 0x%#04x", uuid.uuid);
    NRF_LOG_DEBUG("    Type: 0x%#02x", uuid.type);
    NRF_LOG_DEBUG("  Handle: 0x%#04x", handle);
  }

  void disable(Stack& stack) {
    //TODO: Description of this function suggests problems...
    ret_code_t err_code = sd_ble_uuid_vs_remove(&uuid.type);
    APP_ERROR_CHECK(err_code);
  }

  void onBtleEvent(ble_evt_t const* event) {
    switch (event->header.evt_id) {
      case BLE_GAP_EVT_CONNECTED:
        connectionHandle = event->evt.gap_evt.conn_handle;
        break;

      case BLE_GAP_EVT_DISCONNECTED:
        connectionHandle = BLE_CONN_HANDLE_INVALID;
        break;

      default:
        break;
    }
  }

};
