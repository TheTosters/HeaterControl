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

#include "bluetooth/bluetooth_ctrl.h"

#include <functional>
#include <stdbool.h>
#include <stdint.h>

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
  BleAdvertiser(){
    BluetoothController::getInstance().addObserver(
        [this](BleEventPtr eventPtr) {
          bleEventHandler(eventPtr);
    });
  }

  void startAdvertisement(int advCount, AdvMeasurements_t& data,
      AdvFinishedCallback callback = nullptr) {
    doneCallback = callback;
    //TODO: check true for acquire
    BluetoothController::getInstance().acquireBluetooth(btLock);
    initAdvertising(advCount, data);
    BluetoothController::printMacAddress();
    ret_code_t err_code = sd_ble_gap_adv_start(advHandle,
        BluetoothController::APP_BLE_CONN_CFG_TAG);
    APP_ERROR_CHECK(err_code);
    transmitting = true;
  }

  bool isTransmiting() {
    return transmitting;
  }

private:
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

  /** Callback will be called when advertising is finished */
  AdvFinishedCallback doneCallback;
  bool transmitting{};
  BluetoothController::BTLockId btLock{};

  void finalizeAdvertiser() {
    doneCallback = nullptr;

    //don't care about error, it should be already stopped, called just in case
    ret_code_t err_code = sd_ble_gap_adv_stop(advHandle);

    BluetoothController::getInstance().releaseBluetooth(btLock);
    transmitting = false;
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

  void bleEventHandler(ble_evt_t const * p_ble_evt) {
    if(p_ble_evt->header.evt_id == BLE_GAP_EVT_ADV_SET_TERMINATED) {
      finalizeAdvertiser();
      if (doneCallback) {
        doneCallback();
      }
    }
  }

};
