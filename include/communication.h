#pragma once

#include <components/boards/boards.h>
#include "bluetooth/btle_transmiter.h"
#include "bluetooth/gatt_stack.h"
#include "bluetooth/services/gatt_service.h"
#include "bluetooth/services/battery_service.h"
#include "bluetooth/services/environmental_sensing_service.h"
#include "sensors/sensor_factory.h"

enum class CommMode {
  BT_ADV, BT_GATT
};

template<CommMode COM_MODE>
struct ComFactoryTrait {
    static void build() {
    }

    static void start() {
    }

    static void sendMeasurementsNow(){
    }
};

//Specialisation of factory for Bluetooth advertisement
template<>
struct ComFactoryTrait<CommMode::BT_ADV> {
    static BtleTransmiter* btleTransmiter;

    static void build() {
      BluetoothController::getInstance();
      btleTransmiter = new BtleTransmiter { sensors() };
      sensors().addObserver([](TemperatureC t, RelativeHumidity h, BatteryPrc b) {
        btleTransmiter->transmitNow();
      });
    }

    static void start() {
      btleTransmiter->enable();
    }

    static void sendMeasurementsNow() {
      btleTransmiter->transmitNow();
    }
};

//Specialisation of factory for for Bluetooth GATT
template<>
struct ComFactoryTrait<CommMode::BT_GATT> {
    using GattType = GattStack<EnvironmentalSensingService, BatteryService>;
    static GattType* gattStack;

    static void build() {
      BluetoothController::getInstance();
      gattStack = new GattType { "PioPio" };

      sensors().addObserver([](TemperatureC t, RelativeHumidity h, BatteryPrc b) {
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

    static void sendMeasurementsNow() {
      const OnBoardSensor& sensor = sensors();

      auto& serv = gattStack->getService<EnvironmentalSensingService>();
      auto& tempChr = serv.getCharacteristic<TemperatureCharacteristic>();
      tempChr.setValue(sensor.temperature);

      auto& humChr = serv.getCharacteristic<HumidityCharacteristic>();
      humChr.setValue(sensor.humidity);

      auto& servB = gattStack->getService<BatteryService>();
      auto& batChr = servB.getCharacteristic<BatteryLevelCharacteristic>();
      batChr.setValue(sensor.battery);
    }

    static void start() {
      gattStack->enable();
    }
};

/**
 This code should be added when play with DFU after GATT stack start!
   //--- dfu test
//  static ble_dfu_buttonless_init_t dfus_init;
//  err_code = ble_dfu_buttonless_init(&dfus_init);
//
//  NRF_LOG_INFO("DFU ERROR: %d", err_code);
//  NRF_LOG_FLUSH();
//  APP_ERROR_CHECK(err_code);
  //----^
 */

using Communication = ComFactoryTrait<BOARD_COMMUNICATION>;
