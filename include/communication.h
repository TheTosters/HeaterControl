#pragma once

#include <components/boards/boards.h>
#include "bluetooth/btle_transmiter.h"
#include "bluetooth/gatt_stack.h"
#include "bluetooth/services/gatt_service.h"
#include "bluetooth/services/battery_service.h"
#include "sensors/sensor_factory.h"

enum class CommMode {
  BT_ADV, BT_GATT
};

template<CommMode COM_MODE>
struct ComFactoryTrait {
  static void build() { }
  static void start() {}
};

//Specialisation of factory for Bluetooth advertisement
template<>
struct ComFactoryTrait<CommMode::BT_ADV> {
  static BtleTransmiter* btleTransmiter;

  static void build() {
	  BluetoothController::getInstance();
	  btleTransmiter = new BtleTransmiter{sensors()};
  }

  static void start() {
	  btleTransmiter->enable();
  }
};
BtleTransmiter* ComFactoryTrait<CommMode::BT_ADV>::btleTransmiter;

//Specialisation of factory for for Bluetooth GATT
template<>
struct ComFactoryTrait<CommMode::BT_GATT> {
  using GattType = GattStack<EnvironmentalSensingService, BatteryService>;
  static GattType* gattStack;

  static void build() {
    BluetoothController::getInstance();
    gattStack = new GattType{"PioPio"};

    sensors().addObserver([](TemperatureC t, RelativeHumidity h, BatteryPrc  b){
      auto& serv = gattStack->getService<EnvironmentalSensingService>();
      auto& tempChr = serv.getCharacteristic<TemperatureCharacteristic>();
      tempChr.setValue(t);

      auto& humChr = serv.getCharacteristic<HumidityCharacteristic>();
      humChr.setValue(h);

      auto& servB = gattStack->getService<BatteryService>();
      auto& batChr = servB.getCharacteristic<BatteryLevelCharacteristic>();
      batChr.setValue(b);
    });

  }

  static void start() {
    gattStack->enable();
  }
};

ComFactoryTrait<CommMode::BT_GATT>::GattType* ComFactoryTrait<CommMode::BT_GATT>::gattStack;

using Communication = ComFactoryTrait<BOARD_COMMUNICATION>;
