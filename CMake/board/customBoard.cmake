set(customBoard_DEFINES
    -DCUSTOM_BOARD_INC=${CUSTOM_BOARD_INC}
    -DCONFIG_GPIO_AS_PINRESET)

set(customBoard_INCLUDE_DIRS
    "${SDK_ROOT}/components/boards")
