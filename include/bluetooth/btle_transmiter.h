#pragma once

#include "sensors/sensor_factory.h"
#include "bluetooth/btle_adv.h"
#include "timer_owner.h"
#include "events_dispatcher.h"
#include <memory>


//NOTE: Pass true to timeOwner constructor to have periodic broadcast, or
//set to false if broadcast should be only on sensors change

class BtleTransmiter {
public:
  BtleTransmiter(const OnBoardSensor& sensors)
    : sensors(sensors)
  {
  }

  void enable() {
    doBuildPackage();
    doTransmit();
  }

  void disable() {
    advertiser.stop();
  }

  void transmitNow() {
    doBuildPackage();
    doTransmit();
  }

private:
  //small values might lead to not visible broadcast
  static constexpr unsigned int ADV_COUNT = 32;

  const OnBoardSensor& sensors;
  AdvMeasurements_t measurementsPackage;
  BleAdvertiser advertiser;

  void doBuildPackage() {
    measurementsPackage.temperature =
        static_cast<int16_t>(static_cast<float>(sensors.temperature * 10));
    measurementsPackage.humidity =
        static_cast<int16_t>(static_cast<int>(sensors.humidity));
    measurementsPackage.battery =
        static_cast<int16_t>(static_cast<int>(sensors.battery));
    measurementsPackage.localTime = 0; //TODO: Future
  }

  void doTransmit() {
    if (advertiser.isTransmiting()) {
      NRF_LOG_ERROR("Illegal state of transport!");
      APP_ERROR_CHECK(NRF_ERROR_INVALID_STATE);

    } else {
      advertiser.startAdvertisement(ADV_COUNT, measurementsPackage);
    }
  }
};

constexpr unsigned int BtleTransmiter::ADV_COUNT;
