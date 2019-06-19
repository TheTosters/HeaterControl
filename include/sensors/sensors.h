#pragma once

extern "C" {
#include "app_timer.h"
#include "nrf_log.h"
#include "boards.h"
#include "nrf_gpio.h"
#include "ble_advdata.h"
}

#include "timer_owner.h"
#include "observable.h"
#include "types/unit.h"
#include "battery_sensor.h"

#include <stdint.h>
#include <math.h>
#include <string>

template<typename MainSensor, typename Bridge>
class Sensors : TimerOwner, public Observable<TemperatureC, RelativeHumidity, BatteryPrc> {
public:
  TemperatureC temperature;
  RelativeHumidity humidity;
  BatteryPrc battery;

  Sensors(Bridge& bridge)
  : TimerOwner(false, Sensors::timerHandler), state(WAIT),
    temperature(0), humidity(0), battery(0), mainSensor(bridge) {

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
  BatteryPrc lastBattery{0};
  MainSensor mainSensor;
  BatterySensor batterySensor;

  void collectMeasurement() {
    temperature = mainSensor.getTemperature();
    humidity = mainSensor.getRelHumidity();
    battery = batterySensor.getBatteryPrc();
    checkForNotification();
  }

  void checkForNotification() {
    ensureMainThread();
    if ( (lastTemp != temperature) or
         (lastHum != humidity) or (lastBattery != battery)) {
      lastTemp = temperature;
      lastHum = humidity;
      lastBattery = battery;
      notify(temperature, humidity, battery);
    }
  }

  static void timerHandler(void* selfPtr) {
    Sensors* self = static_cast<Sensors*>(selfPtr);
    switch(self->state){
      default:
      case WAIT:
        self->state = CONFIGURING;
        self->batterySensor.configure();
        self->mainSensor.configure();
        self->startTimer(CONFIGURE_DELAY);
        break;

      case CONFIGURING:
        self->batterySensor.requestMeasurements();
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
