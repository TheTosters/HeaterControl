#pragma once

extern "C" {
#include "app_error.h"
#include "app_util.h"
#include "nrf_log.h"
#include "ble_gap.h"
}

#include "bluetooth/subcomponents/btctrl_sub_comp.h"
#include <string>

class GAPSubComp : public BTCtrlSubComp {
public:
  std::string deviceName {"TestDev"};

  void enable() {
    NRF_LOG_DEBUG("GAPSubComp enable.");
    ble_gap_conn_sec_mode_t sec_mode{};
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    ret_code_t err_code = sd_ble_gap_device_name_set(&sec_mode,
        reinterpret_cast<const uint8_t*>(deviceName.c_str()), deviceName.length());
    APP_ERROR_CHECK(err_code);

    ble_gap_conn_params_t gap_conn_params{};
    gap_conn_params.min_conn_interval = MSEC_TO_UNITS(100, UNIT_1_25_MS);
    gap_conn_params.max_conn_interval = MSEC_TO_UNITS(200, UNIT_1_25_MS);
    gap_conn_params.slave_latency     = 0;
    gap_conn_params.conn_sup_timeout  = MSEC_TO_UNITS(4000, UNIT_10_MS);

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
  }

};
