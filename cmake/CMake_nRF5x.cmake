cmake_minimum_required(VERSION 3.6)

# check if all the necessary toolchain SDK and tools paths have been provided.
if (NOT ARM_NONE_EABI_TOOLCHAIN_PATH)
    message(FATAL_ERROR "The path to the arm-none-eabi-gcc toolchain (ARM_NONE_EABI_TOOLCHAIN_PATH) must be set.")
endif ()

if (NOT NRF5_SDK_PATH)
    message(FATAL_ERROR "The path to the nRF5 SDK (NRF5_SDK_PATH) must be set.")
endif ()

if (NOT NRFJPROG)
    message(FATAL_ERROR "The path to the nrfjprog utility (NRFJPROG) must be set.")
endif ()

# check if the nRF target has been set
if (NRF_TARGET MATCHES "nrf51")

elseif (NRF_TARGET MATCHES "nrf52")

elseif (NOT NRF_TARGET)
    message(FATAL_ERROR "nRF target must be defined")
else ()
    message(FATAL_ERROR "Only nRF51 and rRF52 boards are supported right now")
endif ()

macro(nRF5x_setup)
    # fix on macOS: prevent cmake from adding implicit parameters to Xcode
    set(CMAKE_OSX_SYSROOT "/")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "")

    # language standard/version settings
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_CXX_STANDARD 17)
    if (CMAKE_HOST_UNIX)
        # configure cmake to use the arm-none-eabi-gcc for unix
        set(CMAKE_C_COMPILER "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc")
        set(CMAKE_CXX_COMPILER "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-c++")
        set(CMAKE_ASM_COMPILER "${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-gcc")
        message(STATUS "Host: Linux (Unix)")
        
    elseif(CMAKE_HOST_WIN32)
        # configure cmake to use the arm-none-eabi-gcc for windows
        set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
        set(CMAKE_C_COMPILER arm-none-eabi-gcc)
        set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
        message(STATUS "Host: Windows")
    else()
        message( FATAL_ERROR "Cross-compilation on your platform is not supported by this cmake toolchain" )
    endif()
    
    include_directories(
        "${NRF5_SDK_PATH}/components/softdevice/common/softdevice_handler"
        "${NRF5_SDK_PATH}/modules/nrfx"
        "${NRF5_SDK_PATH}/modules/nrfx/mdk"
        "${NRF5_SDK_PATH}/modules/nrfx/hal"
        "${NRF5_SDK_PATH}/components/toolchain/cmsis/include"
        "${NRF5_SDK_PATH}/components/toolchain/cmsis/dsp/ARM"
    )

    list(APPEND SDK_SOURCE_FILES
#            "${NRF5_SDK_PATH}/components/softdevice/common/softdevice_handler/softdevice_handler.c"
            )

    # CPU specyfic settings
    if (NRF_TARGET MATCHES "nrf52")
        # nRF52 (nRF52-DK => PCA10040)

        set(NRF5_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/gcc_nrf52.ld")
        set(CPU_FLAGS "-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")

        if(DEFINED CUSTOM_BOARD_INC)
           SET(BOARD_VAR "-DNRF52840_XXAA" -DNRF52_SERIES -DCUSTOM_BOARD_INC=${CUSTOM_BOARD_INC})
        else()
           SET(BOARD_VAR "-DNRF52840 -DBOARD_PCA10056 -DNRF52_SERIES -DNRF52840_XXAA")
        endif()

        add_definitions(${BOARD_VAR} -DCONFIG_APP_IN_CORE -DUSE_APP_CONFIG -DCONFIG_GPIO_AS_PINRESET)
        list(APPEND SDK_SOURCE_FILES
                "${NRF5_SDK_PATH}/modules/nrfx/mdk/system_nrf52840.c"
                "${NRF5_SDK_PATH}/modules/nrfx/mdk/gcc_startup_nrf52840.S"
                )
    endif ()

    set(COMMON_FLAGS "-MP -MD -mthumb -mabi=aapcs  -Werror -O0 -g3 -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums -Wno-psabi ${CPU_FLAGS} ")

    # compiler/assambler/linker flags
    set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
    set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}")
    set(CMAKE_ASM_FLAGS "-MP -MD -std=c99 -x assembler-with-cpp -DNRF52840_XXAA -DBOARD_PCA10056")
    set(CMAKE_EXE_LINKER_FLAGS "-mthumb -mabi=aapcs -std=gnu++98 -std=c99 -L ${NRF5_SDK_PATH}/components/toolchain/gcc -L ${NRF5_SDK_PATH}/modules/nrfx/mdk -T${NRF5_LINKER_SCRIPT} ${CPU_FLAGS} -Wl,--gc-sections --specs=nano.specs -lc -lnosys -lm")
    # note: we must override the default cmake linker flags so that CMAKE_C_FLAGS are not added implicitly
    set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <LINK_FLAGS> <OBJECTS> -o <TARGET>")
    set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <LINK_FLAGS> <OBJECTS> -lstdc++ -o <TARGET>")

    include_directories(".")

    # adds target for erasing and flashing the board with a softdevice
    add_custom_target(FLASH_SOFTDEVICE 
            COMMAND ${NRFJPROG} --program ${SOFTDEVICE_PATH} -f ${NRF_TARGET} --sectorerase
            COMMAND ${NRFJPROG} --reset -f ${NRF_TARGET}
            COMMENT "flashing SoftDevice"
            )

    add_custom_target(FLASH_ERASE 
            COMMAND ${NRFJPROG} --eraseall -f ${NRF_TARGET}
            COMMENT "erasing flashing"
            )
endmacro(nRF5x_setup)

# adds a target for comiling and flashing an executable
macro(nRF5x_addExecutable EXECUTABLE_NAME SOURCE_FILES)
    # executable
    add_executable(${EXECUTABLE_NAME} ${SDK_SOURCE_FILES} ${SOURCE_FILES})
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES SUFFIX ".out")
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES LINK_FLAGS "-Wl,-Map=${EXECUTABLE_NAME}.map")

    # additional POST BUILD setps to create the .bin and .hex files
    add_custom_command(TARGET ${EXECUTABLE_NAME}
            POST_BUILD
            COMMAND ${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-size ${EXECUTABLE_NAME}.out
            COMMAND ${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-objcopy -O binary ${EXECUTABLE_NAME}.out "${EXECUTABLE_NAME}.bin"
            COMMAND ${ARM_NONE_EABI_TOOLCHAIN_PATH}/bin/arm-none-eabi-objcopy -O ihex ${EXECUTABLE_NAME}.out "${EXECUTABLE_NAME}.hex"
            COMMENT "post build steps for ${EXECUTABLE_NAME}")

    # custom target for flashing the board
    add_custom_target("FLASH_${EXECUTABLE_NAME}"
            COMMAND ${NRFJPROG} --program ${EXECUTABLE_NAME}.hex -f ${NRF_TARGET} --sectorerase
            COMMAND ${NRFJPROG} --reset -f ${NRF_TARGET}
            DEPENDS ${EXECUTABLE_NAME}
            COMMENT "flashing ${EXECUTABLE_NAME}.hex"
            )
endmacro()

# adds app-level scheduler library
macro(nRF5x_addAppScheduler)
    include_directories(
            "${NRF5_SDK_PATH}/components/libraries/scheduler"
    )

    list(APPEND SDK_SOURCE_FILES
            "${NRF5_SDK_PATH}/components/libraries/scheduler/app_scheduler.c"
            "${NRF5_SDK_PATH}/components/softdevice/common/softdevice_handler/softdevice_handler_appsh.c"
            )

endmacro(nRF5x_addAppScheduler)

# adds app-level FIFO libraries
macro(nRF5x_addAppFIFO)
    include_directories(
            "${NRF5_SDK_PATH}/components/libraries/fifo"
    )

    list(APPEND SDK_SOURCE_FILES
            "${NRF5_SDK_PATH}/components/libraries/fifo/app_fifo.c"
            )

endmacro(nRF5x_addAppFIFO)

# adds app-level Timer libraries
macro(nRF5x_addAppTimer)
    list(APPEND SDK_SOURCE_FILES
            "${NRF5_SDK_PATH}/components/libraries/timer/app_timer.c"
            )
endmacro(nRF5x_addAppTimer)

# adds app-level UART libraries
macro(nRF5x_addAppUART)
    include_directories(
            "${NRF5_SDK_PATH}/components/libraries/uart"
    )

    list(APPEND SDK_SOURCE_FILES
            "${NRF5_SDK_PATH}/components/libraries/uart/app_uart_fifo.c"
            )

endmacro(nRF5x_addAppUART)

# adds app-level Button library
macro(nRF5x_addAppButton)
    include_directories(
            "${NRF5_SDK_PATH}/components/libraries/button"
    )

    list(APPEND SDK_SOURCE_FILES
            "${NRF5_SDK_PATH}/components/libraries/button/app_button.c"
            )

endmacro(nRF5x_addAppButton)

# adds BSP (board support package) library
macro(nRF5x_addBSP WITH_BLE_BTN WITH_ANT_BTN WITH_NFC)
    include_directories(
            "${NRF5_SDK_PATH}/components/libraries/bsp"
    )

    list(APPEND SDK_SOURCE_FILES
            "${NRF5_SDK_PATH}/components/libraries/bsp/bsp.c"
            )

    if (${WITH_BLE_BTN})
        list(APPEND SDK_SOURCE_FILES
                "${NRF5_SDK_PATH}/components/libraries/bsp/bsp_btn_ble.c"
                )
    endif ()

    if (${WITH_ANT_BTN})
        list(APPEND SDK_SOURCE_FILES
                "${NRF5_SDK_PATH}/components/libraries/bsp/bsp_btn_ant.c"
                )
    endif ()

    if (${WITH_NFC})
        list(APPEND SDK_SOURCE_FILES
                "${NRF5_SDK_PATH}/components/libraries/bsp/bsp_nfc.c"
                )
    endif ()

endmacro(nRF5x_addBSP)

# adds Bluetooth Low Energy GATT support library
macro(nRF5x_addBLEGATT)
    include_directories(
            "${NRF5_SDK_PATH}/components/ble/nrf_ble_gatt"
    )

    list(APPEND SDK_SOURCE_FILES
            "${NRF5_SDK_PATH}/components/ble/nrf_ble_gatt/nrf_ble_gatt.c"
            )

endmacro(nRF5x_addBLEGATT)

# adds Bluetooth Low Energy advertising support library
macro(nRF5x_addBLEAdvertising)
    include_directories(
            "${NRF5_SDK_PATH}/components/ble/ble_advertising"
    )

    list(APPEND SDK_SOURCE_FILES
            "${NRF5_SDK_PATH}/components/ble/ble_advertising/ble_advertising.c"
            )

endmacro(nRF5x_addBLEAdvertising)

# adds Bluetooth Low Energy advertising support library
macro(nRF5x_addBLEPeerManager)
    include_directories(
            "${NRF5_SDK_PATH}/components/ble/peer_manager"
    )

    list(APPEND SDK_SOURCE_FILES
            "${NRF5_SDK_PATH}/components/ble/peer_manager/gatt_cache_manager.c"
            "${NRF5_SDK_PATH}/components/ble/peer_manager/gatts_cache_manager.c"
            "${NRF5_SDK_PATH}/components/ble/peer_manager/id_manager.c"
            "${NRF5_SDK_PATH}/components/ble/peer_manager/peer_data.c"
            "${NRF5_SDK_PATH}/components/ble/peer_manager/peer_data_storage.c"
            "${NRF5_SDK_PATH}/components/ble/peer_manager/peer_database.c"
            "${NRF5_SDK_PATH}/components/ble/peer_manager/peer_id.c"
            "${NRF5_SDK_PATH}/components/ble/peer_manager/peer_manager.c"
            "${NRF5_SDK_PATH}/components/ble/peer_manager/pm_buffer.c"
            "${NRF5_SDK_PATH}/components/ble/peer_manager/pm_mutex.c"
            "${NRF5_SDK_PATH}/components/ble/peer_manager/security_dispatcher.c"
            "${NRF5_SDK_PATH}/components/ble/peer_manager/security_manager.c"
            )

endmacro(nRF5x_addBLEPeerManager)

# adds app-level FDS (flash data storage) library
macro(nRF5x_addAppFDS)
    include_directories(
            "${NRF5_SDK_PATH}/components/libraries/fds"
            "${NRF5_SDK_PATH}/components/libraries/fstorage"
            "${NRF5_SDK_PATH}/components/libraries/experimental_section_vars"
    )

    list(APPEND SDK_SOURCE_FILES
            "${NRF5_SDK_PATH}/components/libraries/fds/fds.c"
            "${NRF5_SDK_PATH}/components/libraries/fstorage/fstorage.c"
            )

endmacro(nRF5x_addAppFDS)
