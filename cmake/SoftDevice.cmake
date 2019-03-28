macro(nRF5x_setup_softdevice SOFT_DEV)

  if(${SOFT_DEV} MATCHES "s140")
    
    message(STATUS "Using SoftDevice: s140_6.1.0")
    set(SOFT_DEV_DIR ${NRF5_SDK_PATH}/components/softdevice)
    include_directories(
        "${SOFT_DEV_DIR}/s140/headers/"
        "${SOFT_DEV_DIR}/s140/headers/nrf52/"
    )
    set(SOFTDEVICE_PATH "${SOFT_DEV_DIR}/s140/hex/s140_nrf52_6.1.0_softdevice.hex")
    add_definitions(
        -DS140
        -DSOFTDEVICE_PRESENT
        -DNRF_SD_BLE_API_VERSION=6
    )
  else()
    message(FATAL_ERROR "Not supported SoftDevice version")
  endif()
endmacro()
