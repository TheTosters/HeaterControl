/*
 * battery_sensor.h
 *
 *  Created on: Jun 19, 2019
 *      Author: bartlomiejzarnowski
 */

#ifndef INCLUDE_SENSORS_BATTERY_SENSOR_H_
#define INCLUDE_SENSORS_BATTERY_SENSOR_H_

extern "C" {
#include "app_error.h"
#include "nrfx_saadc.h"
#include "nrf_drv_saadc.h"
}

#include "types/unit.h"

class BatterySensor {
public:
  BatterySensor() {
    nrfx_saadc_config_t default_config{};
    default_config.resolution =
        (nrf_saadc_resolution_t) NRFX_SAADC_CONFIG_RESOLUTION;
    default_config.oversample =
        (nrf_saadc_oversample_t) NRFX_SAADC_CONFIG_OVERSAMPLE;
    default_config.interrupt_priority = NRFX_SAADC_CONFIG_IRQ_PRIORITY;
    default_config.low_power_mode = NRFX_SAADC_CONFIG_LP_MODE;
    ret_code_t err_code = nrfx_saadc_init(&default_config, saadcEventHandler);
    APP_ERROR_CHECK(err_code);

    nrf_saadc_channel_config_t config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_VDD);
    err_code = nrf_drv_saadc_channel_init(0, &config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[0], 1);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[1], 1);
    APP_ERROR_CHECK(err_code);
  }

  void configure(){

  }

  void requestMeasurements() {
    ret_code_t err_code = nrf_drv_saadc_sample();
    APP_ERROR_CHECK(err_code);
  }

  BatteryPrc getBatteryPrc() {
    return battery;
  }

private:
  static constexpr unsigned int ADC_REF_VOLTAGE_IN_MILLIVOLTS{600};
  static constexpr unsigned int ADC_PRE_SCALING_COMPENSATION{6};
  static constexpr unsigned int DIODE_FWD_VOLT_DROP_MILLIVOLTS{64};
  static constexpr unsigned int ADC_RES_10BIT{1024};

  nrf_saadc_value_t adc_buf[2];
  static unsigned int adcMv;
  static BatteryPrc battery;

  static void saadcEventHandler(nrf_drv_saadc_evt_t const* event) {
    if (event->type == NRF_DRV_SAADC_EVT_DONE) {
      uint8_t percentage_batt_lvl;
      ret_code_t err_code;

      nrf_saadc_value_t adcResult = event->data.done.p_buffer[0];

      err_code = nrf_drv_saadc_buffer_convert(event->data.done.p_buffer, 1);
      APP_ERROR_CHECK(err_code);

      unsigned int battLvlMv = adcResult * ADC_REF_VOLTAGE_IN_MILLIVOLTS;
      battLvlMv /= ADC_RES_10BIT;
      battLvlMv *= ADC_PRE_SCALING_COMPENSATION;
      battLvlMv += DIODE_FWD_VOLT_DROP_MILLIVOLTS;
      constexpr float ETA = 0.9f;
      adcMv = (1.0f - ETA) * battLvlMv + ETA * adcMv;
      battery = BatteryPrc{battery_level_in_percent(adcMv)};
      NRF_LOG_INFO("Battery %d%%, %dmV, (%dmV)",
          battery_level_in_percent(adcMv), adcMv, battLvlMv);
    }
  }
};

BatteryPrc BatterySensor::battery{0};
unsigned int BatterySensor::adcMv{3000};
#endif /* INCLUDE_SENSORS_BATTERY_SENSOR_H_ */
