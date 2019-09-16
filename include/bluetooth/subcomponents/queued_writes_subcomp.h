#pragma once

extern "C" {
#include "app_error.h"
#include "nrf_ble_qwr.h"
#include "nrf_log.h"
}

#include "bluetooth/subcomponents/btctrl_sub_comp.h"

class QueuedWritesSubComp : public BTCtrlSubComp {
public:
  QueuedWritesSubComp() {
    //Warning: this is compilation time constant macro
    NRF_SDH_BLE_OBSERVER(qwrInstanceObs,
                         NRF_BLE_QWR_BLE_OBSERVER_PRIO,
                         nrf_ble_qwr_on_ble_evt,
                         &qwrInstance);
    qwrInstance = {};
  }

  void enable() {
    if (qwrInstance.initialized == 0) {
      singeTimeInit();
    }
  }

private:
  static nrf_ble_qwr_t qwrInstance;

  void singeTimeInit() {
    NRF_LOG_DEBUG("Queued Writes Module Init");

    //todo: check value for NRF_BLE_QWR_MAX_ATTR
    nrf_ble_qwr_init_t qwr_init{};
    qwr_init.error_handler = BTCtrlSubComp::onSdkError;;

    ret_code_t err_code = nrf_ble_qwr_init(&qwrInstance, &qwr_init);
    APP_ERROR_CHECK(err_code);
  }
};
