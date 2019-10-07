#pragma once

//This is custom board done for this project, it will cover mapping of
//buttons and other things related strict to hardware board layout

//-----------------
// This are pins configuration for various code parts

//I2C
#define CONFIG_SDA_PIN      NRF_GPIO_PIN_MAP(0, 26)
#define CONFIG_SCL_PIN      NRF_GPIO_PIN_MAP(0, 27)
#define CONFIG_SSD1306_I2C_ADDRESS   ((uint8_t)0x3C)

#define CONFIG_DS18B20_PIN  NRF_GPIO_PIN_MAP(0, 1)

//#define BOARD_SENSOR  BoardSensor::SHT30
#define BOARD_SENSOR BoardSensor::DS18B20

//#define BOARD_COMMUNICATION CommMode::BT_ADV
#define BOARD_COMMUNICATION CommMode::BT_GATT

//Used by BSP module for easy buttons
#define BUTTONS_NUMBER 3

#define BUTTON_PREV     NRF_GPIO_PIN_MAP(1, 14)
#define BUTTON_NEXT     NRF_GPIO_PIN_MAP(1, 15)
#define BUTTON_OK       NRF_GPIO_PIN_MAP(1, 5)
#define BUTTON_PULL     NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST { BUTTON_PREV, BUTTON_NEXT, BUTTON_OK}

#define BSP_BUTTON_0   BUTTON_PREV
#define BSP_BUTTON_1   BUTTON_NEXT
#define BSP_BUTTON_2   BUTTON_OK

// LEDs definitions for PCA10056
#define LEDS_NUMBER    1

#define LED_1          NRF_GPIO_PIN_MAP(0, 13)
#define LED_START      LED_1
#define LED_STOP       LED_1

#define LEDS_ACTIVE_STATE 0

#define LEDS_LIST {LED_1}

#define LEDS_INV_MASK  LEDS_MASK

