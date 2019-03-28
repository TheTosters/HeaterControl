cmake_minimum_required(VERSION 3.6)
project(home-multi-temp C CXX ASM)

set(NRF_TARGET "nrf52")

include("local_env.cmake")
include("cmake/CMake_nRF5x.cmake")
include("cmake/SoftDevice.cmake")
include("fix.cmake")

nRF5x_setup()
nRF5x_setup_softdevice(s140)

include_directories(${NRF5_SDK_PATH}/config/nrf52840/config)
include_directories("include")
include_directories(${SDK_SEARCH_DIRECTORIES})
include_directories(${NRF5_SDK_PATH}/arm-none-eabi/include)
#include_directories("ssd1306_driver")

list(APPEND SOURCE_FILES 
  "src/main.cpp"
  "src/ds18b20.c"
  "src/i2c_bridge.cpp"
  "src/ssd1306.cpp"
  "src/chewy_regular42.cpp"
  ${EXTRA_SDK_SOURCE_FILES}
) 

nRF5x_addExecutable(${PROJECT_NAME} "${SOURCE_FILES}")
