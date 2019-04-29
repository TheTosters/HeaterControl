#pragma once

extern "C" {
#include "app_error.h"
#include "app_util.h"
#include "nrf_log.h"
#include "ble_conn_params.h"
}

#include "bluetooth/subcomponents/btctrl_sub_comp.h"
#include <string>
#include <stdint.h>

class ConnParamsCgfComp : public BTCtrlSubComp {
public:
  void enable() {
    NRF_LOG_DEBUG("ConnParamsCgfComp enable");
    ble_conn_params_init_t cp_init{};

    cp_init.p_conn_params = nullptr;
    /*Time from initiating event (connect or start of notification) to first
     time sd_ble_gap_conn_param_update is called */
    cp_init.first_conn_params_update_delay = APP_TIMER_TICKS(5000);
    //Time between each call to sd_ble_gap_conn_param_update after the first call
    cp_init.next_conn_params_update_delay =  APP_TIMER_TICKS(30000);
    //Number of attempts before giving up the connection parameter negotiation.
    cp_init.max_conn_params_update_count = 3;
    cp_init.start_on_notify_cccd_handle = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail = true;
    cp_init.evt_handler = nullptr;
    cp_init.error_handler = BTCtrlSubComp::onSdkError;

    ret_code_t err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
  }

  void disable() {
    ble_conn_params_stop();
    NRF_LOG_DEBUG("ConnParamsCgfComp disable");
  }
};
