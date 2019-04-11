#pragma once

#include "sensors/sensors.h"
#include "boards.h"

#ifdef SHT30_ON_BOARD
#include "bridges/i2c_bridge.h"
#include "sensors/sht30.h"

using Sht30Sensor = Sht30<Sht30Mode::Single_HighRep_ClockStretch,
                          Sht30Address::VDD>;
using OnBoardSensor = Sensors<Sht30Sensor, I2c_Bridge>;

OnBoardSensor instantiateSensor(I2c_Bridge& bridge) {
  return OnBoardSensor{bridge};
}
#endif

#ifdef DS18B20_ON_BOARD
#include "bridges/one_wire.h"
#include "sensors/ds18b20.h"

using Ds18b20Sensor = Ds18b20<Res9Bit>;
using OnBoardSensor = Sensors<Ds18b20Sensor, OneWire>;

OnBoardSensor instantiateSensor(I2c_Bridge& bridge) {
  UNUSED_PARAMETER(bridge);
  static OneWire oneWire{CONFIG_DS18B20_PIN};
  return OnBoardSensor{oneWire};
}
#endif



