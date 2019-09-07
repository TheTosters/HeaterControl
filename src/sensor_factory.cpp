#include "sensors/sensor_factory.h"
#include "bridges/i2c_bridge.h"

OnBoardSensor& sensors() {
    static OnBoardSensor* sensor = new OnBoardSensor{FactoryTrait<BOARD_SENSOR>::build(i2cBridge())};
    return *sensor;
}
