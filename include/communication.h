#pragma once

#include <components/boards/boards.h>
#include "bluetooth/btle_transmiter.h"
#include "bluetooth/gatt_stack.h"
#include "bluetooth/services/gatt_service.h"
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
  static GattStack<EnvironmentalSensingService>* gattStack;

  static void build() {
	  BluetoothController::getInstance();
	  gattStack = new GattStack<EnvironmentalSensingService>{"PioPio"};
  }

  static void start() {
	  gattStack->enable();
  }
};

GattStack<EnvironmentalSensingService>* ComFactoryTrait<CommMode::BT_GATT>::gattStack;

using Communication = ComFactoryTrait<BOARD_COMMUNICATION>;
