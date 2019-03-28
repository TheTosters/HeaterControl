#pragma once

#include "nrf_drv_twi.h"
#include "sdk_errors.h"
#include <cinttypes>
#include <functional>

class I2c_Bridge {
public:
  using TwiErrorHandler = std::function<void(ret_code_t)>;
  using PinNo = uint32_t;

  I2c_Bridge(TwiErrorHandler errorHandler);

  void begin(PinNo sdaPin, PinNo sclPin);
  void send(uint8_t i2cAddress, uint8_t b1);
  void send(uint8_t i2cAddress, uint8_t b1, uint8_t b2);
  void send(uint8_t i2cAddress, uint8_t b1, uint8_t b2, uint8_t b3);
  void send(uint8_t i2cAddress, uint8_t* buffer, uint8_t size);

private:
  TwiErrorHandler errorHandler;
  const nrf_drv_twi_t twiInstance = NRF_DRV_TWI_INSTANCE(0);
  uint8_t innerTransferBuffer[3];

  void twiWaitIfBusy();
};
