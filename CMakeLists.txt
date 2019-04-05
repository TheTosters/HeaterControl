cmake_minimum_required(VERSION 3.6)

if (NOT BUILD_HOST)
    set(CMAKE_SYSTEM_NAME "Generic")
    set(CMAKE_SYSTEM_PROCESSOR "ARM")
endif (NOT BUILD_HOST)

set(CMAKE_CONFIG_DIR "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR} CACHE STRING "" FORCE)

include("cmake/Toolchain.cmake")
include("cmake/FindDependency.cmake")
include("cmake/FindSDK.cmake")

# We have to return manually from here s.t. the CMake generation
# doesn't stop and we have the nRF5_SDK target available.
if (NOT SDK_ROOT)
    return()
endif ()

project(home-multi-temp C CXX ASM)

set(NRF_TARGET "nrf52")

#use this for production board -> production_board.h 
#set(CUSTOM_BOARD_INC "production_board")
#use this for development board -> dev_board.h
set(CUSTOM_BOARD_INC "dev_board")


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

list(APPEND SOURCE_FILES 
  "src/main.cpp"
  "src/ssd1306.cpp"
  ${EXTRA_SDK_SOURCE_FILES}
) 

nRF5x_addExecutable(${PROJECT_NAME} "${SOURCE_FILES}")
