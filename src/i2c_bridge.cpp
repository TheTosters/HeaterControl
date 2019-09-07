#include <components/boards/boards.h>
#include "bridges/i2c_bridge.h"
#include "types/hardware_pin.h"

I2c_Bridge& i2cBridge(){
  static I2c_Bridge* bridge = new I2c_Bridge{HardwarePin{CONFIG_SDA_PIN},
	    HardwarePin{CONFIG_SCL_PIN}, nullptr};
  return *bridge;
}
