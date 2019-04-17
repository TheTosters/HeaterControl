#pragma once

#include <functional>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
extern "C" {
#include "nordic_common.h"
#include "bsp.h"
#include "nrf_log.h"
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
}

#define BTLE_DEBUG 1

/**
* Struct with data used to advertise measurements.
*/
struct __attribute__((packed)) AdvMeasurements_t {
  int16_t temperature;
  int16_t humidity;
  int16_t battery;
  int32_t localTime;
};

using AdvFinishedCallback = std::function<void(void)>;

class BleAdvertiser {
public:
  static BleAdvertiser& getInstance() {
    static BleAdvertiser instance;
    return instance;
  }

  void startAdvertisement(int advCount, AdvMeasurements_t& data,
      AdvFinishedCallback callback = nullptr) {
    doneCallback = callback;
    initBleStack();
    initAdvertising(advCount, data);
    printMacAddress();
    ret_code_t err_code = sd_ble_gap_adv_start(advHandle, APP_BLE_CONN_CFG_TAG);
    APP_ERROR_CHECK(err_code);
    transmitting = true;
  }

  bool isTransmiting() {
    return transmitting;
  }

private:
  static constexpr uint8_t APP_BLE_CONN_CFG_TAG = 1;
  static constexpr int APP_BLE_OBSERVER_PRIO = 2;

  /** Parameters to be passed to the stack when starting advertising. */
  ble_gap_adv_params_t advParams;

  /** Advertising handle used to identify an advertising set. */
  uint8_t advHandle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;

  /** Buffer for storing an encoded advertising set. */
  uint8_t encodedAdvPayload[BLE_GAP_ADV_SET_DATA_SIZE_MAX];

  /**@brief Struct that contains pointers to the encoded advertising data. */
  ble_gap_adv_data_t gapAdvData = {
      {encodedAdvPayload,BLE_GAP_ADV_SET_DATA_SIZE_MAX},
      {nullptr, 0}
  };
//      .adv_data = {
//          .p_data = encodedAdvPayload,
//          .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX
//      },
//      .scan_rsp_data = {
//          .p_data = NULL,
//          .len    = 0
//
//      }
//  };

  /** Callback will be called when advertising is finished */
  AdvFinishedCallback doneCallback;
  bool transmitting{};

  BleAdvertiser(){}

  void finalizeAdvertiser() {
    doneCallback = nullptr;

    //don't care about error, it should be already stopped, called just in case
    ret_code_t err_code = sd_ble_gap_adv_stop(advHandle);

    err_code = nrf_sdh_disable_request();
    APP_ERROR_CHECK(err_code);
    transmitting = false;
  }

  void printMacAddress() {
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
        BleAdvertiser::bleEventHandler, nullptr);
  }

  void initAdvertising(int advCount, AdvMeasurements_t& data) {

      ble_advdata_manuf_data_t manuf_specific_data;
      //Company identifier as per www.bluetooth.org.
      manuf_specific_data.company_identifier = 0x01FF;
      manuf_specific_data.data.p_data = reinterpret_cast<uint8_t*>(&data);
      manuf_specific_data.data.size = sizeof(data);

      // Build and set advertising data.
      ble_advdata_t advdata;
      memset(&advdata, 0, sizeof(advdata));

      advdata.name_type = BLE_ADVDATA_NO_NAME;
      advdata.flags = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED;
      advdata.p_manuf_specific_data = &manuf_specific_data;

      // Initialize advertising parameters (used when starting advertising).
      memset(&advParams, 0, sizeof(advParams));

      advParams.properties.type =
        BLE_GAP_ADV_TYPE_NONCONNECTABLE_NONSCANNABLE_UNDIRECTED;
      advParams.p_peer_addr = NULL;    // Undirected advertisement.
      advParams.filter_policy = BLE_GAP_ADV_FP_ANY;
      advParams.interval = MSEC_TO_UNITS(100, UNIT_0_625_MS);
      advParams.duration = 0;       // Never time out.
      advParams.max_adv_evts = advCount;

      uint32_t err_code = ble_advdata_encode(&advdata, gapAdvData.adv_data.p_data,
        &gapAdvData.adv_data.len);
      APP_ERROR_CHECK(err_code);

      err_code = sd_ble_gap_adv_set_configure(&advHandle, &gapAdvData,
        &advParams);
      APP_ERROR_CHECK(err_code);
  }

  static void bleEventHandler(ble_evt_t const * p_ble_evt, void * p_context) {
    switch (p_ble_evt->header.evt_id) {
      case BLE_GAP_EVT_ADV_SET_TERMINATED: {
        BleAdvertiser& instance = BleAdvertiser::getInstance();
        instance.finalizeAdvertiser();
        if (instance.doneCallback) {
          instance.doneCallback();
        }
        break;
      }

      default:
        break;
    }
  }
};
