#pragma once

#include "sensors/sensor_factory.h"
#include "btle_adv.h"
#include "timer_owner.h"
#include "events_dispatcher.h"
#include <memory>


//NOTE: Pass true to timeOwner constructor to have periodic broadcast, or
//set to false if broadcast should be only on sensors change

class BtleTransmiter : TimerOwner{
public:
  BtleTransmiter(const OnBoardSensor& sensors)
    : TimerOwner(false, BtleTransmiter::timerHandler),
      sensors(sensors)
  {
  }

  void enable() {
    ret_code_t ret;
    startTimer(ADV_INTERVAL);
    doBuildPackage();
    doTransmit();
  }

  void disable() {
    stopTimer();
  }

private:
  //static constexpr unsigned int ADV_INTERVAL = APP_TIMER_TICKS(1 * 60 * 1000);
  static constexpr unsigned int ADV_INTERVAL = APP_TIMER_TICKS(9 * 1000);

  //small values might lead to not visible broadcast
  static constexpr unsigned int ADV_COUNT = 32;

  const OnBoardSensor& sensors;
  AdvMeasurements_t measurementsPackage;

  void doBuildPackage() {
    measurementsPackage.temperature =
        static_cast<int16_t>(static_cast<float>(sensors.temperature * 10));
    measurementsPackage.humidity =
        static_cast<int16_t>(static_cast<int>(sensors.humidity));
    measurementsPackage.battery = 0;  //TODO: Future
    measurementsPackage.localTime = 0; //TODO: Future
  }

  void doTransmit() {
    BleAdvertiser& advertiser = BleAdvertiser::getInstance();
    if (advertiser.isTransmiting()) {
      NRF_LOG_ERROR("Illegal state of transport!");
      APP_ERROR_CHECK(NRF_ERROR_INVALID_STATE);

    } else {
      advertiser.startAdvertisement(ADV_COUNT, measurementsPackage);
    }
  }

  static void timerHandler(void* selfPtr) {
    BtleTransmiter* self = static_cast<BtleTransmiter*>(selfPtr);
    self->doBuildPackage();
    self->doTransmit();
  }
};

constexpr unsigned int BtleTransmiter::ADV_COUNT;
