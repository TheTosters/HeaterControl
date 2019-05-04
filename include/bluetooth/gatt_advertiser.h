#pragma once
extern "C" {
//#include "nrf.h"
#include "app_error.h"
//#include "ble.h"
//#include "ble_hci.h"
//#include "ble_srv_common.h"
//#include "ble_advdata.h"
#include "ble_advertising.h"
//#include "ble_conn_params.h"
//#include "nrf_sdh.h"
//#include "nrf_sdh_soc.h"
//#include "nrf_sdh_ble.h"
//#include "app_timer.h"
//#include "fds.h"
//#include "peer_manager.h"
//#include "ble_conn_state.h"
//#include "nrf_ble_gatt.h"
}

#include "bluetooth/services/gatt_service.h"
#include <tuple>

template<typename Stack>
class GattAdvertiser {
public:
  void enable(Stack& stack) {
    servicesUids.clear();
    stack.collectServicesUids(GattAdvertiser::servicesUids);

    configure();
    ret_code_t err_code = ble_advertising_start(&advertisingInstance,
        BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
  }

  void disable(Stack& stack) {
    sd_ble_gap_adv_stop(advertisingInstance.adv_handle);
  }

private:
  /** The advertising interval (in units of 0.625 ms.
   * This value corresponds to 187.5 ms).
   */
  static constexpr uint32_t APP_ADV_INTERVAL{50};

  /** The advertising duration (180 seconds) in units of 10 milliseconds. */
  static constexpr uint32_t APP_ADV_DURATION{18000};

  static ble_advertising_t advertisingInstance;
  static ServicesUidsVect servicesUids;

  static void on_adv_evt(ble_adv_evt_t ble_adv_evt) {
    switch (ble_adv_evt) {
      case BLE_ADV_EVT_FAST:
        NRF_LOG_INFO("Fast advertising.");
        break;

      case BLE_ADV_EVT_IDLE:
        NRF_LOG_INFO("Adv idle.");
        //sleep_mode_enter();
        break;

      default:
        break;
    }
  }

  void configure() {
    ble_advertising_init_t init{};
    init.advdata.name_type = BLE_ADVDATA_FULL_NAME;
    init.advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    init.srdata.uuids_complete.uuid_cnt = servicesUids.size();
    init.srdata.uuids_complete.p_uuids  = servicesUids.data();

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;
    init.evt_handler = GattAdvertiser::on_adv_evt;

    ret_code_t err_code = ble_advertising_init(&advertisingInstance, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&advertisingInstance,
        BluetoothController::APP_BLE_CONN_CFG_TAG);

    NRF_SDH_BLE_OBSERVER(advertisingInstanceBleObs,
                         BLE_ADV_BLE_OBSERVER_PRIO,
                         ble_advertising_on_ble_evt,
                         &advertisingInstance);
    NRF_SDH_SOC_OBSERVER(advertisingInstanceSocObs,
                         BLE_ADV_SOC_OBSERVER_PRIO,
                         ble_advertising_on_sys_evt,
                         &advertisingInstance);
  }
};

template<typename Stack>
ble_advertising_t GattAdvertiser<Stack>::advertisingInstance;

template<typename Stack>
ServicesUidsVect GattAdvertiser<Stack>::servicesUids;
