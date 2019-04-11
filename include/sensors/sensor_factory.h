#pragma once

#include "boards.h"
#include "bridges/i2c_bridge.h"
#include "bridges/one_wire.h"
#include "sensors/sensors.h"
#include "sensors/sht30.h"
#include "sensors/ds18b20.h"

enum class BoardSensor {
  SHT30, DS18B20
};

template<BoardSensor sensorType>
struct FactoryTrait {
  using BoardSensorType = void;
  static BoardSensorType build(I2c_Bridge& bridge) {
    UNUSED_PARAMETER(bridge);
  }
};

//Specialization of factory for DS sensor
template<>
struct FactoryTrait<BoardSensor::DS18B20> {
  using Ds18b20Sensor = Ds18b20<Res9Bit>;
  using BoardSensorType = Sensors<Ds18b20Sensor, OneWire>;

  static BoardSensorType build(I2c_Bridge& bridge) {
    UNUSED_PARAMETER(bridge);
    static OneWire oneWire{CONFIG_DS18B20_PIN};
    return BoardSensorType{oneWire};
  }
};

//Specialization of factory for SHT30 sensor
template<>
struct FactoryTrait<BoardSensor::SHT30> {
  using Sht30Sensor = Sht30<Sht30Mode::Single_HighRep_ClockStretch,
                            Sht30Address::VDD>;
  using BoardSensorType = Sensors<Sht30Sensor, I2c_Bridge>;

  static BoardSensorType build(I2c_Bridge& bridge) {
    return BoardSensorType{bridge};
  }
};

using OnBoardSensor = FactoryTrait<BOARD_SENSOR>::BoardSensorType;

OnBoardSensor instantiateSensor(I2c_Bridge& bridge) {
  return FactoryTrait<BOARD_SENSOR>::build(bridge);
}
