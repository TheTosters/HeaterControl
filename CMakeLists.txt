set(PROJECT_NAME "HeaterControl")

# Choose platform: "nrf52832_xxAA", "nrf52840_xxAA", "nrf52810_xxAA", "nrf51422_xxAC"
# BUILD_HOST override it if enabled
set(PLATFORM "nrf52840_xxAA" CACHE STRING "Choose the target platform to build for.")

# Uncomment and set custom board name if needed
set(CUSTOM_BOARD_INC "dev_board")

# Set a default build type: "Debug" "Release" "MinSizeRel" "RelWithDebInfo"
set(default_build_type "Debug") 

option(BUILD_HOST "Build for host (unit test build)" OFF)

cmake_minimum_required(VERSION 3.6)
if (NOT CMAKE_VERSION VERSION_LESS 3.9)
    # Allow user to enable CMAKE_INTERPROCEDURAL_OPTIMIZATION (LTO) if supported for the toolchain.
    # This is supported from CMake version 9 and later.
    cmake_policy(SET CMP0069 NEW)
endif ()

if (NOT BUILD_HOST)
    set(CMAKE_SYSTEM_NAME "Generic")
    set(CMAKE_SYSTEM_PROCESSOR "ARM")
endif (NOT BUILD_HOST)

project(${PROJECT_NAME} LANGUAGES NONE)

set(CMAKE_CONFIG_DIR "${CMAKE_SOURCE_DIR}/CMake")

include("local_env.cmake")
include("fix.cmake")
include("${CMAKE_CONFIG_DIR}/Toolchain.cmake")
include("${CMAKE_CONFIG_DIR}/Platform.cmake")
include("${CMAKE_CONFIG_DIR}/SoftDevice.cmake")
include("${CMAKE_CONFIG_DIR}/BuildType.cmake")
include("${CMAKE_CONFIG_DIR}/Board.cmake")
include("${CMAKE_CONFIG_DIR}/platform/${PLATFORM}.cmake")
include("${CMAKE_CONFIG_DIR}/softdevice/${SOFTDEVICE}.cmake")
include("${CMAKE_CONFIG_DIR}/board/${BOARD}.cmake")

set(ARCH ${${PLATFORM}_ARCH})
set(TARGET_NAME "${PROJECT_NAME}_${PLATFORM}_${SOFTDEVICE}")

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

if (NOT BUILD_HOST)
    set(CMAKE_EXECUTABLE_SUFFIX ".elf")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
endif ()

# Export compilation commands to .json file (used by clang-complete backends)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT BUILD_HOST)
    include("${CMAKE_CONFIG_DIR}/Nrfjprog.cmake")
endif ()

set(TARGET_SOURCE_FILES
    ${${PLATFORM}_SOURCE_FILES}
    ${${nRF5_SDK_VERSION}_SOURCE_FILES}
)

set(TARGET_INCLUDE_DIRS
    ${SDK_ROOT}/config/nrf52840/config
    ${${SOFTDEVICE}_INCLUDE_DIRS}
    ${${PLATFORM}_INCLUDE_DIRS}
    ${${BOARD}_INCLUDE_DIRS}
    ${${nRF5_SDK_VERSION}_INCLUDE_DIRS}
    ${ARM_NONE_EABI_TOOLCHAIN_PATH}/arm-none-eabi/include
)

add_executable(${TARGET_NAME}
    "src/main.cpp"
    "src/ssd1306.cpp"
    ${TARGET_SOURCE_FILES}
    ${EXTRA_SDK_SOURCE_FILES}
)

target_include_directories(${TARGET_NAME} PUBLIC
    "include"
    ${TARGET_INCLUDE_DIRS}
    ${SDK_SEARCH_DIRECTORIES}
)

target_compile_definitions(${TARGET_NAME} PUBLIC
    -DCONFIG_APP_IN_CORE
    -DUSE_APP_CONFIG
    ${${PLATFORM}_DEFINES}
    ${${SOFTDEVICE}_DEFINES}
    ${${BOARD}_DEFINES}
)

target_compile_options(${TARGET_NAME} PUBLIC
    ${${ARCH}_DEFINES})

if (NOT BUILD_HOST)
    set_target_link_options(${TARGET_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/linker/${PLATFORM}_${SOFTDEVICE})
        
    create_hex(${TARGET_NAME})
    add_flash_target(${TARGET_NAME})
endif ()

get_property(target_include_dirs TARGET ${TARGET_NAME} PROPERTY INCLUDE_DIRECTORIES)

message(STATUS "-----------------------------------")
message(STATUS "BuildType: ${CMAKE_BUILD_TYPE}")
message(STATUS "Arch: ${${PLATFORM}_ARCH}")
message(STATUS "Platform: ${PLATFORM}")
message(STATUS "SDK: ${SDK_ROOT}")
message(STATUS "SoftDevice: ${SOFTDEVICE}")
message(STATUS "Board: ${BOARD}")
message(STATUS "-----------------------------------")