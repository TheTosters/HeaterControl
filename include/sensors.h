#pragma once

#include "i2c_bridge.h"
#include "one_wire.h"
#include "ds18b20.h"
#include <cinttypes>

extern "C" {
#include "app_timer.h"
#include "nrf_log.h"
#include "boards.h"
#include "nrf_gpio.h"
}


class Sensors {
public:
  float temperature;
  int humidity;

  Sensors(I2c_Bridge bridge)
  : timerId(&timer), state(WAIT), temperature(0), humidity(0) {
    UNUSED_PARAMETER(bridge); //needed for SHT30 in future
  }

  void begin() {
    nrf_gpio_cfg_output(CONFIG_SENSORS_PWR_PIN);
    ret_code_t ret = app_timer_create(&timerId, APP_TIMER_MODE_SINGLE_SHOT,
        Sensors::timerHandler);
    APP_ERROR_CHECK(ret);
    startTimer(STARTUP_INTERVAL);
  }

private:
  //initial configuration and power on delay
  static constexpr unsigned int STARTUP_INTERVAL = APP_TIMER_TICKS(1 * 1000);

  //how often measurements should take place
  //constexpr unsigned int MEASUREMENT_INTERVAL = APP_TIMER_TICKS(3 * 60 * 1000);
  static constexpr unsigned int MEASUREMENT_INTERVAL = APP_TIMER_TICKS(3 * 1000);

  //should be maximum of power up for DS18B20 and/or SHT30
  //for SHT30 -> 1ms (according to datasheet)
  //for DS18B20 -> 30ms (assumed due to various reading on net)
  static constexpr unsigned int POWER_UP_DELAY = APP_TIMER_TICKS(30);

  //should be maximum time of measure for DS18B20 and/or SHT30
  //DS18B20 at 9bits -> 94ms
  //SHT30 at High repeatability -> 15 ms
  static constexpr unsigned int MEASURING_DELAY = APP_TIMER_TICKS(95);

  enum State {
    CONFIGURING, WAIT, POWERING_UP, MEASURING
  };

  app_timer_t timer;
  app_timer_id_t timerId;
  State state;
  OneWire oneWire{CONFIG_DS18B20_PIN};
  Ds18b20 ds18b20{oneWire};

  void configureSensors() {
    //need to be called only once since it's in DS eprom
    ds18b20.begin();
    ds18b20.setResolution(Ds18b20::Res9Bit); //0.5 deg resolution is ok
    //TODO: Add code for SHT30 if needed
  }

  void startTimer(unsigned int delay) {
    ret_code_t ret = app_timer_start(timerId, delay, this);
    if (ret != NRFX_SUCCESS) {
      NRF_LOG_ERROR("Can't start timer, queue full?");
    }
    APP_ERROR_CHECK(ret);
  }

  void powerUp() {
    nrf_gpio_pin_set(CONFIG_SENSORS_PWR_PIN);
  }

  void powerDown() {
    nrf_gpio_pin_clear(CONFIG_SENSORS_PWR_PIN);
  }

  void measure() {
    //TODO: add sht30 if/when needed
    ds18b20.requestTemperatures();
  }

  void collectMeasurement() {
    //TODO: add sht30 if/when needed
    temperature = ds18b20.getTempC();
    powerDown();
  }

  static void timerHandler(void* selfPtr) {
    Sensors* self = static_cast<Sensors*>(selfPtr);
    switch(self->state){
      case WAIT:
        self->powerUp();
        self->state = POWERING_UP;
        self->startTimer(POWER_UP_DELAY);
        break;

      case POWERING_UP:
        self->configureSensors();
        self->measure();
        self->state = MEASURING;
        self->startTimer(MEASURING_DELAY);
        break;

      case CONFIGURING:
        self->configureSensors();
        self->measure();
        self->state = MEASURING;
        self->startTimer(MEASURING_DELAY);
        break;

      case MEASURING:
        self->collectMeasurement();
        self->state = WAIT;
        self->startTimer(MEASUREMENT_INTERVAL);
        break;
    }
  }
};
