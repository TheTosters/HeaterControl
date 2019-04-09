#pragma once

#include "timer_owner.h"
#include "bridges/i2c_bridge.h"
#include "bridges/one_wire.h"
#include "ds18b20.h"
#include "observable.h"
#include <cinttypes>
#include <math.h>
#include "sht30.h"

extern "C" {
#include "app_timer.h"
#include "nrf_log.h"
#include "boards.h"
#include "nrf_gpio.h"
#include "ble_advdata.h"
}

using SensorsObserver = std::function<void(float, int)>;

class Sensors : TimerOwner, public Observable<SensorsObserver> {
public:
  float temperature;
  int humidity;

  Sensors(I2c_Bridge& bridge)
  : bridge(bridge), TimerOwner(false, Sensors::timerHandler), timerId(&timer), state(WAIT),
    temperature(0), humidity(0), sht30(bridge) {

    nrf_gpio_cfg_output(CONFIG_SENSORS_PWR_PIN);
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

  I2c_Bridge& bridge;
  app_timer_t timer{};
  app_timer_id_t timerId;
  State state;
  OneWire oneWire{CONFIG_DS18B20_PIN};
  Ds18b20 ds18b20{oneWire};
  float lastTemp{0};
  int lastHum{0};
  Sht30 sht30;

  void configureSensors() {
    ds18b20.begin();
    ds18b20.setResolution(Ds18b20::Res9Bit); //0.5 deg resolution is ok
  }

  void powerUp() {
    nrf_gpio_pin_set(CONFIG_SENSORS_PWR_PIN);
  }

  void powerDown() {
    nrf_gpio_pin_clear(CONFIG_SENSORS_PWR_PIN);
  }

  void measure() {
    sht30.RequestMeasurements();
    ds18b20.requestTemperatures();
  }

  void collectMeasurement() {
    temperature = static_cast<float>(sht30.GetTemperature());
    humidity = static_cast<float>(sht30.GetRelHumidity());
    //temperature = ds18b20.getTempC();
    checkForNotification();
    powerDown();
  }

  void checkForNotification() {
    int curT = (int)(temperature * 100);
    int lastT = (int)(lastTemp * 100);
    if ((abs(curT - lastT) > 20) or
        (lastHum != humidity)) {
      lastTemp = temperature;
      lastHum = humidity;
      notify(temperature, humidity);
    }
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
