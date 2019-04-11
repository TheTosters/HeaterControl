#pragma once

#include "sensors.h"
#include "btle_adv.h"
#include "timer_owner.h"
#include "events_dispatcher.h"
#include <memory>


//NOTE: Pass true to timeOwner constructor to have periodic broadcast, or
//set to false if broadcast should be only on sensors change

class BtleTransmiter : TimerOwner{
public:
  BtleTransmiter(const Sensors& sensors)
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

  const Sensors& sensors;
  AdvMeasurements_t measurementsPackage;
  std::unique_ptr<BleAdvertiser> advertiser;

  void doBuildPackage() {
    measurementsPackage.temperature =
        static_cast<int16_t>(static_cast<float>(sensors.temperature * 10));
    measurementsPackage.humidity =
        static_cast<int16_t>(static_cast<int>(sensors.humidity));
    measurementsPackage.battery = 0;  //TODO: Future
    measurementsPackage.localTime = 0; //TODO: Future
  }

  void doTransmit() {
    if (advertiser) {
      NRF_LOG_ERROR("Illegal state of transport!");
      APP_ERROR_CHECK(NRF_ERROR_INVALID_STATE);
    } else {
      advertiser = std::make_unique<BleAdvertiser>(ADV_COUNT, measurementsPackage,
          [this](){
            //NRF_LOG_ERROR("Transmit done");
            dispatchOnMainThread(this, BtleTransmiter::mainThreadExecutor);
      });
    }
    advertiser->start();
  }

  static void mainThreadExecutor(void* p_event_data, uint16_t event_size) {
    //NRF_LOG_ERROR("Destroying advertiser");
    BtleTransmiter** self =
        extractDispatchedData<BtleTransmiter*>(p_event_data, event_size);
    (*self)->advertiser.reset(nullptr);
  }

  static void timerHandler(void* selfPtr) {
    BtleTransmiter* self = static_cast<BtleTransmiter*>(selfPtr);
    self->doBuildPackage();
    self->doTransmit();
  }
};

constexpr unsigned int BtleTransmiter::ADV_COUNT;
