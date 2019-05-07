#pragma once
extern "C" {
#include "app_error.h"
#include "ble_advertising.h"
}

#include "bluetooth/services/gatt_service.h"
#include <tuple>

template<typename Stack>
class GattAdvertiser {
public:
  GattAdvertiser() = default;
  GattAdvertiser(const GattAdvertiser&) = delete;
  GattAdvertiser operator=(const GattAdvertiser&) = delete;

  void enable(Stack& stack) {
    NRF_LOG_ERROR("== enable-0");
    servicesUids.clear();
    NRF_LOG_ERROR("== enable-1");
    stack.collectServicesUids(GattAdvertiser::servicesUids);

    configure();
    NRF_LOG_ERROR("== enable-2");
    ret_code_t err_code = ble_advertising_start(&advertisingInstance,
        BLE_ADV_MODE_FAST);
    NRF_LOG_ERROR("== enable:%d", err_code);
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
    NRF_LOG_INFO("adv configure");
    ble_advertising_init_t init{};
    init.advdata.name_type = BLE_ADVDATA_FULL_NAME;
    init.advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    init.srdata.uuids_complete.uuid_cnt = 0;//servicesUids.size();
    init.srdata.uuids_complete.p_uuids  = 0;//servicesUids.data();

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;
    init.evt_handler = GattAdvertiser::on_adv_evt;
    nrf_delay_ms(500);  //TODO: probably BTLE need time to bootup, investigate further
    ret_code_t err_code = ble_advertising_init(&advertisingInstance, &init);
    NRF_LOG_INFO("ble_advertising_init:%d", err_code);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&advertisingInstance,
        BluetoothController::APP_BLE_CONN_CFG_TAG);
    NRF_LOG_INFO("adv configure-");
//    NRF_SDH_BLE_OBSERVER(advertisingInstanceBleObs,
//                         BLE_ADV_BLE_OBSERVER_PRIO,
//                         ble_advertising_on_ble_evt,
//                         &advertisingInstance);
//    NRF_SDH_SOC_OBSERVER(advertisingInstanceSocObs,
//                         BLE_ADV_SOC_OBSERVER_PRIO,
//                         ble_advertising_on_sys_evt,
//                         &advertisingInstance);
  }
};

template<typename Stack>
ble_advertising_t GattAdvertiser<Stack>::advertisingInstance;

template<typename Stack>
ServicesUidsVect GattAdvertiser<Stack>::servicesUids;
