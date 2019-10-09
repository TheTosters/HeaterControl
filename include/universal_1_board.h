#pragma once

//This is custom board done for this project, following things are on it:
// * An RGB LED
// * No user buttons
// * SHT30 Sensor
// * No display
// * GATT based measurements

//-----------------
// This are pins configuration for various code parts

//I2C
#define CONFIG_SDA_PIN      NRF_GPIO_PIN_MAP(0, 26)
#define CONFIG_SCL_PIN      NRF_GPIO_PIN_MAP(0, 27)

//#define BOARD_SENSOR  BoardSensor::SHT30
#define BOARD_SENSOR BoardSensor::DS18B20

#define MEASURE_INTERVAL_MS   (5 * 60 * 1000)
#define MEASURE_DELTA_TEMP    (0.5f)
#define MEASURE_DELTA_HUM     (5)

#define BOARD_COMMUNICATION CommMode::BT_GATT

#define MEASURE_ON_BUTTON_PRESS   1

#define BUTTONS_NUMBER  1
#define BUTTON_PREV     NRF_GPIO_PIN_MAP(1, 14)
#define BUTTON_PULL     NRF_GPIO_PIN_PULLUP
#define BUTTONS_ACTIVE_STATE 0
#define BUTTONS_LIST   {BUTTON_PREV}
#define BSP_BUTTON_0   BUTTON_PREV

#define LEDS_NUMBER    0
#define DISPLAY        1

#define CONFIG_RGB_R_PIN      NRF_GPIO_PIN_MAP(1, 14)
#define CONFIG_RGB_G_PIN      NRF_GPIO_PIN_MAP(1, 15)
#define CONFIG_RGB_B_PIN      NRF_GPIO_PIN_MAP(1, 5)

//unused but helps to compile ;)
#define CONFIG_SSD1306_I2C_ADDRESS   ((uint8_t)0x3C)
#define CONFIG_DS18B20_PIN           NRF_GPIO_PIN_MAP(0, 1)
#define BUTTONS_ACTIVE_STATE         0
#define LEDS_ACTIVE_STATE            0
