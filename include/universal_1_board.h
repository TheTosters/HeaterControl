#pragma once

//This is custom board done for this project, it refers to PCB: hardware/universalSensor

//This things need to be set from CMAKE or compiler options
#ifndef BOARD_SENSOR
  #error "BOARD_SENSOR macro need to be set!"
#endif

#ifndef BOARD_COMMUNICATION
  #error "BOARD_COMMUNICATION macro need to be set!"
#endif

#ifndef BOARD_RGB_LED
  #warning "BOARD_RGB_LED  macro should be set, falback to NONE"
  #define BOARD_RGB_LED   RGBLedType::NONE
#endif

#ifndef MEASURE_ON_BUTTON_PRESS
  #warning "MEASURE_ON_BUTTON_PRESS is not set by build system, falback to 0"
  #define MEASURE_ON_BUTTON_PRESS   1
#endif

#ifndef BUTTONS_NUMBER
  #warning "BUTTONS_NUMBER is not set by build system, falback to 1"
  #define BUTTONS_NUMBER  1
#endif

#ifndef LEDS_NUMBER
  #warning "LEDS_NUMBER is not set by build system, falback to 0"
  #define LEDS_NUMBER    0  //BSP LEDS not RGB!
#endif

#ifndef DISPLAY
  #warning "DISPLAY is not set by build system, falback to 0"
  #define DISPLAY        0
#endif


//I2C
#define CONFIG_SDA_PIN      NRF_GPIO_PIN_MAP(0, 26)
#define CONFIG_SCL_PIN      NRF_GPIO_PIN_MAP(0, 27)

#define MEASURE_INTERVAL_MS   (5 * 60 * 1000)
#define MEASURE_DELTA_TEMP    (0.5f)
#define MEASURE_DELTA_HUM     (5)

//BSP buttons
#define BUTTON_PREV     NRF_GPIO_PIN_MAP(0, 6)
#define BUTTON_PULL     NRF_GPIO_PIN_PULLUP
#define BUTTONS_ACTIVE_STATE 0
#define BUTTONS_LIST   {BUTTON_PREV}
#define BSP_BUTTON_0   BUTTON_PREV

//RGB led config (Not BSP led)
#define CONFIG_RGB_R_PIN      NRF_GPIO_PIN_MAP(1, 14)
#define CONFIG_RGB_G_PIN      NRF_GPIO_PIN_MAP(1, 15)
#define CONFIG_RGB_B_PIN      NRF_GPIO_PIN_MAP(1, 5)

//unused but helps to compile ;)
#define CONFIG_SSD1306_I2C_ADDRESS   ((uint8_t)0x3C)
#define CONFIG_DS18B20_PIN           NRF_GPIO_PIN_MAP(0, 1)
#define BUTTONS_ACTIVE_STATE         0
#define LEDS_ACTIVE_STATE            0
