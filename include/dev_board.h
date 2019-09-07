#pragma once

#include "pca10056.h"

#define CONFIG_SDA_PIN      NRF_GPIO_PIN_MAP(0, 26)
#define CONFIG_SCL_PIN      NRF_GPIO_PIN_MAP(0, 27)
#define CONFIG_SSD1306_I2C_ADDRESS   ((uint8_t)0x3C)

#define CONFIG_DS18B20_PIN  NRF_GPIO_PIN_MAP(0,  2)

#define BOARD_SENSOR  BoardSensor::SHT30

//#define BOARD_COMMUNICATION CommMode::BT_ADV
#define BOARD_COMMUNICATION CommMode::BT_GATT
