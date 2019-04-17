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
#include "types/unit.h"
#include "sht30.h"

#include <stdint.h>
#include <math.h>
#include <string>

template<typename MainSensor, typename Bridge>
class Sensors : TimerOwner, public Observable<TemperatureC, RelativeHumidity> {
public:
  TemperatureC temperature;
  RelativeHumidity humidity;

  Sensors(Bridge& bridge)
  : TimerOwner(false, Sensors::timerHandler), state(WAIT),
    temperature(0), humidity(0), mainSensor(bridge) {

    startTimer(CONFIGURE_DELAY);
  }

  Sensors(const Sensors&) = delete;
  Sensors& operator=(const Sensors&) = delete;

  static std::string getName() {
    return MainSensor::getName();
  }
private:
  //how often measurements should take place
  //constexpr unsigned int MEASUREMENT_INTERVAL = APP_TIMER_TICKS(3 * 60 * 1000);
  static constexpr unsigned int MEASUREMENT_INTERVAL = APP_TIMER_TICKS(3 * 1000);
  static constexpr unsigned int CONFIGURE_DELAY =
      APP_TIMER_TICKS(MainSensor::CONFIGURE_DELAY_MS);
  static constexpr unsigned int MEASURING_DELAY =
      APP_TIMER_TICKS(MainSensor::MEASURING_DELAY_MS);

  enum State {
    WAIT, CONFIGURING, MEASURING
  };

  State state;
  TemperatureC lastTemp{0};
  RelativeHumidity lastHum{0};
  MainSensor mainSensor;

  void collectMeasurement() {
    temperature = mainSensor.getTemperature();
    humidity = mainSensor.getRelHumidity();
    checkForNotification();
  }

  void checkForNotification() {
    ensureMainThread();
    if ( (lastTemp != temperature) or
         (lastHum != humidity)) {
      lastTemp = temperature;
      lastHum = humidity;
      notify(temperature, humidity);
    }
  }

  static void timerHandler(void* selfPtr) {
    Sensors* self = static_cast<Sensors*>(selfPtr);
    switch(self->state){
      default:
      case WAIT:
        self->state = CONFIGURING;
        self->mainSensor.configure();
        self->startTimer(CONFIGURE_DELAY);
        break;

      case CONFIGURING:
        self->mainSensor.requestMeasurements();
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
