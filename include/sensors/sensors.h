#pragma once

extern "C" {
#include "app_timer.h"
#include "nrf_log.h"
#include "boards.h"
#include "nrf_gpio.h"
#include "ble_advdata.h"
}

#include "timer_owner.h"
#include "bridges/i2c_bridge.h"
#include "bridges/one_wire.h"
#include "ds18b20.h"
#include "observable.h"
#include "unit.h"
#include "sht30.h"

#include <stdint.h>
#include <math.h>

using SensorsObserver = std::function<void(float, int)>;

template<typename MainSensor, typename Bridge>
class Sensors : TimerOwner, public Observable<SensorsObserver> {
public:
  TemperatureC temperature;
  RelativeHumidity humidity;

  Sensors(Bridge& bridge)
  : TimerOwner(false, Sensors::timerHandler), state(WAIT),
    temperature(0), humidity(0), mainSensor(bridge) {

    //TODO: Power off nrf_gpio_cfg_output(CONFIG_SENSORS_PWR_PIN);
    startTimer(STARTUP_INTERVAL);
  }

  Sensors(const Sensors&) = delete;
  Sensors& operator=(const Sensors&) = delete;

private:
  //initial configuration and power on delay
  static constexpr unsigned int STARTUP_INTERVAL = APP_TIMER_TICKS(1 * 1000);

  //how often measurements should take place
  //constexpr unsigned int MEASUREMENT_INTERVAL = APP_TIMER_TICKS(3 * 60 * 1000);
  static constexpr unsigned int MEASUREMENT_INTERVAL = APP_TIMER_TICKS(3 * 1000);

  //should be maximum of power up for DS18B20 and/or SHT30
  static constexpr unsigned int POWER_UP_DELAY = APP_TIMER_TICKS(MainSensor::POWER_UP_DELAY_MS);

  //should be maximum time of measure for DS18B20 and/or SHT30
  static constexpr unsigned int MEASURING_DELAY = APP_TIMER_TICKS(MainSensor::MEASURING_DELAY_MS);

  enum State {
    CONFIGURING, WAIT, POWERING_UP, MEASURING
  };

  State state;
  TemperatureC lastTemp{0};
  RelativeHumidity lastHum{0};
  MainSensor mainSensor;

  void configureSensors() {
    mainSensor.configure(); //0.5 deg resolution is ok
  }

  void powerUp() {
    //TODO: power up nrf_gpio_pin_set(CONFIG_SENSORS_PWR_PIN);
  }

  void powerDown() {
    //TODO: Power off nrf_gpio_pin_clear(CONFIG_SENSORS_PWR_PIN);
  }

  void measure() {
    mainSensor.requestMeasurements();
  }

  void collectMeasurement() {
    temperature = mainSensor.getTemperature();
    humidity = mainSensor.getRelHumidity();
    checkForNotification();
    powerDown();
  }

  void checkForNotification() {
    ensureMainThread();
    if ( (lastTemp != temperature) or
         (lastHum != humidity)) {
      lastTemp = temperature;
      lastHum = humidity;
      notify(static_cast<float>(temperature), static_cast<int>(humidity));
    }
  }

  static void timerHandler(void* selfPtr) {
    ensureMainThread();
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
