#pragma once

extern "C" {
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_drv_twi.h"
#include "nrf_pwr_mgmt.h"
#include "sdk_errors.h"
}
#include "types/hardware_pin.h"
#include "types/named_value_type.h"
#include <stdint.h>
#include <functional>

class I2c_Bridge {
public:
  using I2cAddress = NamedValueType<uint8_t, struct I2cAddress_tag>;
  using TwiErrorHandler = std::function<void(ret_code_t)>;

  I2c_Bridge(HardwarePin sdaPin, HardwarePin sclPin,
      TwiErrorHandler errorHandler) :
      errorHandler(errorHandler == nullptr ?
          I2c_Bridge::defaultErrorHandler : errorHandler) {

    nrf_drv_twi_config_t config;
    config.scl = sclPin.get();
    config.sda = sdaPin.get();
    config.frequency = NRF_DRV_TWI_FREQ_100K;
    config.interrupt_priority = APP_IRQ_PRIORITY_LOW;
    config.clear_bus_init = true;//false

    ret_code_t err = nrf_drv_twi_init(&twiInstance, &config,
        I2c_Bridge::twi_handler, this);
    if (err != NRF_SUCCESS) {
      errorHandler(err);
    }

    nrf_drv_twi_enable(&twiInstance);
  }

  void send(I2cAddress i2cAddress, uint8_t b1) {
    twiWaitIfBusy();

    innerTransferBuffer[0] = b1;
    ret_code_t err_code = nrf_drv_twi_tx(&twiInstance, i2cAddress.get(),
        innerTransferBuffer, 1, false);
    if (err_code != NRF_SUCCESS){
      errorHandler(err_code);
    }
  }

  void send(I2cAddress i2cAddress, uint8_t b1, uint8_t b2) {
    twiWaitIfBusy();

    innerTransferBuffer[0] = b1;
    innerTransferBuffer[1] = b2;
    ret_code_t err_code = nrf_drv_twi_tx(&twiInstance, i2cAddress.get(),
        innerTransferBuffer, 2, false);
    if (err_code != NRF_SUCCESS){
      errorHandler(err_code);
    }
  }

  void send(I2cAddress i2cAddress, uint8_t b1, uint8_t b2, uint8_t b3) {
    twiWaitIfBusy();

    innerTransferBuffer[0] = b1;
    innerTransferBuffer[1] = b2;
    innerTransferBuffer[2] = b3;
    ret_code_t err_code = nrf_drv_twi_tx(&twiInstance, i2cAddress.get(),
        innerTransferBuffer, 3, false);
    if (err_code != NRF_SUCCESS){
      errorHandler(err_code);
    }
  }

  void send(I2cAddress i2cAddress, const uint8_t* buffer, uint8_t size) {
    twiWaitIfBusy();
    txDone = false;
    ret_code_t err_code = nrf_drv_twi_tx(&twiInstance, i2cAddress.get(), buffer,
        size, false);
    if (err_code != NRF_SUCCESS){
      errorHandler(err_code);
    }
    twiWaitIfBusy();
  }

  template<typename T>
  auto read(I2cAddress i2cAddress) {
      T retVal;
      twiWaitIfBusy();
      ret_code_t err_code = nrf_drv_twi_rx(&twiInstance, i2cAddress.get(),
          reinterpret_cast<uint8_t*>(std::addressof(retVal)), sizeof(T));
      if (err_code != NRF_SUCCESS){
        errorHandler(err_code);
      }
      twiWaitIfBusy();
      return retVal;
  }

private:
  TwiErrorHandler errorHandler;
  const nrf_drv_twi_t twiInstance = NRF_DRV_TWI_INSTANCE(0);
  uint8_t innerTransferBuffer[3] {};
  bool txDone = true;

  void twiWaitIfBusy() {
    while(I2c_Bridge::txDone == false || nrf_drv_twi_is_busy(&twiInstance)) {
        nrf_pwr_mgmt_run();
      }
  }

  static void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context) {
    I2c_Bridge* self = static_cast<I2c_Bridge*>(p_context);
    self->txDone = true;
  }

  static void defaultErrorHandler(ret_code_t err) {
    APP_ERROR_CHECK(err);
  }
};
