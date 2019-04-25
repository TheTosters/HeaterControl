#pragma once

extern "C" {
#include "nordic_common.h"
#include "bsp.h"
#include "nrf_log.h"
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
}

#include "types/general_id_type.h"
#include "observable.h"

#include <inttypes.h>

#define BTLE_DEBUG 1

using BleEventPtr = ble_evt_t const*;
/**
 * This is a singleton :[
 */
class BluetoothController : public Observable<BleEventPtr> {
public:
  using BTLockId = TinyIdType<struct BTLockId_tag>;
  static constexpr uint8_t APP_BLE_CONN_CFG_TAG = 1;

  static BluetoothController& getInstance() {
    static BluetoothController instance;
    return instance;
  }

  void enableBluetooth() {
    //TODO: add turn-off policy and reinit support
    ASSERT(not inHandlerContext);
    APP_ERROR_CHECK( lockId == BTLockId::NOT_SET ?
        NRF_SUCCESS :
        NRF_ERROR_INVALID_STATE);
    initBleStack();
  }

  void disableBluetooth() {
    //TODO: add turn-off policy
    APP_ERROR_CHECK( lockId == BTLockId::NOT_SET ?
        NRF_SUCCESS :
        NRF_ERROR_INVALID_STATE);
    if (inHandlerContext) {
      //NOTE: If you disable softdevice while app_scheduler pull data from it
      //you will get fatal error, so we put disableSD later in scheduler queue
      dispatchOnMainThread(nullptr, [](void* p_event_data, uint16_t event_size){
        BluetoothController::getInstance().shutDownBleStack();
      });

    } else {
      shutDownBleStack();
    }
  }

  bool acquireBluetooth(BTLockId& lock) {
    if (lockId != BTLockId::NOT_SET) {
      return false;
    }
    enableBluetooth();
    //fairy random magic number, reconsider if changing id have sense here
    lockId = BTLockId{93};
    lock = lockId;
    return true;
  }

  bool releaseBluetooth(BTLockId& id) {
    if (id == lockId) {
      id = BTLockId::NOT_SET;
      lockId = BTLockId::NOT_SET;
      disableBluetooth();
      return true;
    }
    return false;
  }

  static void printMacAddress() {
#ifdef BTLE_DEBUG
      constexpr int SERIAL_NUMBER_STRING_SIZE = 12;

      char buff[SERIAL_NUMBER_STRING_SIZE + 1] = {0};
      // The masking makes the address match the Random Static BLE address.
      const uint16_t hBytes = (uint16_t)NRF_FICR->DEVICEADDR[1] | 0xC000;
      const uint32_t lBytes  = NRF_FICR->DEVICEADDR[0];
      snprintf(buff, SERIAL_NUMBER_STRING_SIZE + 1,
               "%04" PRIX16 "%08" PRIX32,
               hBytes, lBytes);
      NRF_LOG_INFO("MAC ADDR: %s", buff);
#endif
  }

private:
  static constexpr int APP_BLE_OBSERVER_PRIO = 2;
  bool inHandlerContext{};
  BTLockId lockId{};

  BluetoothController() {}

  void shutDownBleStack() {
    ret_code_t err_code = nrf_sdh_disable_request();
    APP_ERROR_CHECK(err_code);
  }

  void initBleStack() {
    ret_code_t err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO,
        BluetoothController::bleEventHandler, nullptr);
  }

  static void bleEventHandler(ble_evt_t const * p_ble_evt, void* p_context) {
    BluetoothController::getInstance().inHandlerContext = true;
    BluetoothController::getInstance().notify(p_ble_evt);
    BluetoothController::getInstance().inHandlerContext = false;
  }
};
