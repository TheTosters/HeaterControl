#pragma once

extern "C" {
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_drv_twi.h"
#include "nrf_pwr_mgmt.h"
#include "sdk_errors.h"
}

#include <cinttypes>
#include <functional>

class I2c_Bridge {
public:
  using TwiErrorHandler = std::function<void(ret_code_t)>;
  using PinNo = uint32_t;

  I2c_Bridge(PinNo sdaPin, PinNo sclPin, TwiErrorHandler errorHandler) :
      errorHandler(errorHandler == nullptr ?
          I2c_Bridge::defaultErrorHandler : errorHandler) {

    const nrf_drv_twi_config_t config = {
        .scl = sclPin,
        .sda = sdaPin,
        .frequency = NRF_DRV_TWI_FREQ_100K,
        .interrupt_priority = APP_IRQ_PRIORITY_LOW,
        .clear_bus_init = false
    };

    ret_code_t err = nrf_drv_twi_init(&twiInstance, &config,
        I2c_Bridge::twi_handler, nullptr);
    if (err != NRF_SUCCESS) {
      errorHandler(err);
    }

    nrf_drv_twi_enable(&twiInstance);
  }

  void send(uint8_t i2cAddress, uint8_t b1) {
    twiWaitIfBusy();

    innerTransferBuffer[0] = b1;
    ret_code_t err_code = nrf_drv_twi_tx(&twiInstance, i2cAddress,
        innerTransferBuffer, 1, false);
    if (err_code != NRF_SUCCESS){
      errorHandler(err_code);
    }
  }

  void send(uint8_t i2cAddress, uint8_t b1, uint8_t b2) {
    twiWaitIfBusy();

    innerTransferBuffer[0] = b1;
    innerTransferBuffer[1] = b2;
    ret_code_t err_code = nrf_drv_twi_tx(&twiInstance, i2cAddress,
        innerTransferBuffer, 2, false);
    if (err_code != NRF_SUCCESS){
      errorHandler(err_code);
    }
  }

  void send(uint8_t i2cAddress, uint8_t b1, uint8_t b2, uint8_t b3) {
    twiWaitIfBusy();

    innerTransferBuffer[0] = b1;
    innerTransferBuffer[1] = b2;
    innerTransferBuffer[2] = b3;
    ret_code_t err_code = nrf_drv_twi_tx(&twiInstance, i2cAddress,
        innerTransferBuffer, 3, false);
    if (err_code != NRF_SUCCESS){
      errorHandler(err_code);
    }
  }

  void send(uint8_t i2cAddress, uint8_t* buffer, uint8_t size) {
    twiWaitIfBusy();
    ret_code_t err_code = nrf_drv_twi_tx(&twiInstance, i2cAddress, buffer,
        size, false);
    if (err_code != NRF_SUCCESS){
      errorHandler(err_code);
    }
    twiWaitIfBusy();
  }

  template<typename T>
  auto read(uint8_t i2cAddress) {
      T retVal;
      twiWaitIfBusy();
      ret_code_t err_code = nrf_drv_twi_rx(&twiInstance, i2cAddress, reinterpret_cast<uint8_t*>(std::addressof(retVal)), T::size);
      if (err_code != NRF_SUCCESS){
        errorHandler(err_code);
      }

      return retVal;
  }

private:
  TwiErrorHandler errorHandler;
  const nrf_drv_twi_t twiInstance = NRF_DRV_TWI_INSTANCE(0);
  uint8_t innerTransferBuffer[3];

  void twiWaitIfBusy() {
    while(nrf_drv_twi_is_busy(&twiInstance)) {
        nrf_pwr_mgmt_run();
      }
  }

  static void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context) {
    switch (p_event->type) {
        case NRF_DRV_TWI_EVT_DATA_NACK:
          break;
        default:
          break;
    }
  }

  static void defaultErrorHandler(ret_code_t err) {
    APP_ERROR_CHECK(err);
  }
};
