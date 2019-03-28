#include "i2c_bridge.h"
extern "C" {
#include "nrf_delay.h"
#include "nrf_drv_twi.h"
#include "nrf_pwr_mgmt.h"
}
#include <cinttypes>

namespace {

void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context) {
  switch (p_event->type) {
      case NRF_DRV_TWI_EVT_DATA_NACK:
        break;
      default:
        break;
  }
}

void defaultErrorHandler(ret_code_t err) {
  APP_ERROR_CHECK(err);
}

}

I2c_Bridge::I2c_Bridge(TwiErrorHandler errorHandler)
: errorHandler(errorHandler == nullptr ? defaultErrorHandler : errorHandler){
}

void I2c_Bridge::begin(PinNo sdaPin, PinNo sclPin) {
  const nrf_drv_twi_config_t config = {
     .scl                = sclPin,
     .sda                = sdaPin,
     .frequency          = NRF_DRV_TWI_FREQ_100K,
     .interrupt_priority = APP_IRQ_PRIORITY_LOW,
     .clear_bus_init     = false
  };

  ret_code_t err = nrf_drv_twi_init(&twiInstance, &config, twi_handler, nullptr);
  if (err != NRF_SUCCESS){
    errorHandler(err);
  }

  nrf_drv_twi_enable(&twiInstance);
}

void I2c_Bridge::twiWaitIfBusy() {
  while(nrf_drv_twi_is_busy(&twiInstance)) {
    nrf_pwr_mgmt_run();
  }
}

void I2c_Bridge::send(uint8_t i2cAddress, uint8_t* buffer, uint8_t bufSize) {
  twiWaitIfBusy();
  ret_code_t err_code = nrf_drv_twi_tx(&twiInstance, i2cAddress, buffer,
      bufSize, false);
  if (err_code != NRF_SUCCESS){
    errorHandler(err_code);
  }
  twiWaitIfBusy();
}

void I2c_Bridge::send(uint8_t i2cAddress, uint8_t b1, uint8_t b2, uint8_t b3) {
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

void I2c_Bridge::send(uint8_t i2cAddress, uint8_t b1, uint8_t b2) {
  twiWaitIfBusy();

  innerTransferBuffer[0] = b1;
  innerTransferBuffer[1] = b2;
  ret_code_t err_code = nrf_drv_twi_tx(&twiInstance, i2cAddress,
      innerTransferBuffer, 2, false);
  if (err_code != NRF_SUCCESS){
    errorHandler(err_code);
  }
}

void I2c_Bridge::send(uint8_t i2cAddress, uint8_t b1) {
  twiWaitIfBusy();

  innerTransferBuffer[0] = b1;
  ret_code_t err_code = nrf_drv_twi_tx(&twiInstance, i2cAddress,
      innerTransferBuffer, 1, false);
  if (err_code != NRF_SUCCESS){
    errorHandler(err_code);
  }
}


