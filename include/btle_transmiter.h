#pragma once

#include "sensors.h"
#include "btle_adv.h"
#include <memory>

class BtleTransmiter {
public:
  BtleTransmiter(const Sensors& sensors)
    : sensors(sensors), timerId(&timer), timerCreated(false)
  {
  }

  void begin() {
    ret_code_t ret;
    if (not timerCreated) {
      timerCreated = true;
      ret = app_timer_create(&timerId, APP_TIMER_MODE_REPEATED,
          BtleTransmiter::timerHandler);
      APP_ERROR_CHECK(ret);
    }
    ret = app_timer_start(timerId, ADV_INTERVAL, this);
    if (ret != NRFX_SUCCESS) {
      NRF_LOG_ERROR("Can't start timer, queue full?");
    }
    APP_ERROR_CHECK(ret);
    doBuildPackage();
    doTransmit();
  }

  void end() {
    ret_code_t ret = app_timer_stop(timerId);
    if (ret != NRFX_SUCCESS) {
      NRF_LOG_ERROR("Can't start timer, queue full?");
    }
  }

private:
  //static constexpr unsigned int ADV_INTERVAL = APP_TIMER_TICKS(1 * 60 * 1000);
  static constexpr unsigned int ADV_INTERVAL = APP_TIMER_TICKS(3 * 1000);

  static constexpr unsigned int ADV_COUNT = 6;

  const Sensors& sensors;
  app_timer_t timer;
  app_timer_id_t timerId;
  AdvMeasurements_t measurementsPackage;
  std::unique_ptr<BleAdvertiser> advertiser;
  bool timerCreated;

  void doBuildPackage() {
    measurementsPackage.temperature = (int16_t)(sensors.temperature * 10);
    measurementsPackage.humidity = sensors.humidity;
    measurementsPackage.battery = 0;  //TODO: Future
    measurementsPackage.localTime = 0; //TODO: Future
  }

  void doTransmit() {
    //TODO: Bad madafaka keep crashing, this is temporary solution
    if (advertiser) {
      NRF_LOG_ERROR("Illegal state of transport!");
      //APP_ERROR_CHECK(NRF_ERROR_INVALID_STATE);
    } else {
      advertiser = std::make_unique<BleAdvertiser>(ADV_COUNT, measurementsPackage,
          [this](){
            NRF_LOG_ERROR("Transmit done");
            //this->advertiser.reset(nullptr);
      });
    }
    advertiser->start();
  }

  static void timerHandler(void* selfPtr) {
    BtleTransmiter* self = static_cast<BtleTransmiter*>(selfPtr);
    self->doBuildPackage();
    self->doTransmit();
  }
};

constexpr unsigned int BtleTransmiter::ADV_COUNT;
