set(FIRMWARE_VARIANTS 
    Indoor_SHT_ADV
    Indoor_SHT_GATT
    Indoor_DS_FULL)
    
set(FIRMWARE_VARIANT "Indoor_SHT_ADV" CACHE STRING "Firmware build variant")
set_property(CACHE FIRMWARE_VARIANT PROPERTY STRINGS ${FIRMWARE_VARIANTS})

# For defined boards please refer to folder: include for files ends with *_board.h

function(configureFirmwareBuild)
    if(NOT FIRMWARE_VARIANT IN_LIST FIRMWARE_VARIANTS)
        message(FATAL_ERROR "FIRMWARE_VARIANT must be one of ${FIRMWARE_VARIANTS}")
    endif()

    if (FIRMWARE_VARIANT STREQUAL "Indoor_SHT_ADV") 
        message(STATUS "Configuring ${FIRMWARE_VARIANT}")
        set(CUSTOM_BOARD_INC "universal_1_board" PARENT_SCOPE)
        add_compile_definitions(BOARD_SENSOR=BoardSensor::SHT30)
        add_compile_definitions(BOARD_COMMUNICATION=CommMode::BT_ADV)
        add_compile_definitions(BUTTONS_NUMBER=0)
        add_compile_definitions(MEASURE_ON_BUTTON_PRESS=0)
        add_compile_definitions(BOARD_RGB_LED=RGBLedType::FULL_3PIN_RGB)
        add_compile_definitions(DISPLAY=0)
        add_compile_definitions(LEDS_NUMBER=0)
    
    elseif(FIRMWARE_VARIANT STREQUAL "Indoor_SHT_GATT")
    elseif(FIRMWARE_VARIANT STREQUAL "Indoor_DS_FULL")
    endif()
endfunction(configureFirmwareBuild)