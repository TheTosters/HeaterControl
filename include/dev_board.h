#pragma once

#include "pca10056.h"

#define CONFIG_SDA_PIN      NRF_GPIO_PIN_MAP(0, 26)
#define CONFIG_SCL_PIN      NRF_GPIO_PIN_MAP(0, 27)
#define CONFIG_SSD1306_I2C_ADDRESS   ((uint8_t)0x3C)

#define CONFIG_DS18B20_PIN  NRF_GPIO_PIN_MAP(0,  2)

#define BOARD_SENSOR  BoardSensor::SHT30
#define MEASURE_INTERVAL_MS   (5 * 60 * 1000)
#define MEASURE_DELTA_TEMP    (0.5f)
#define MEASURE_DELTA_HUM     (5)

//#define BOARD_COMMUNICATION CommMode::BT_ADV
#define BOARD_COMMUNICATION CommMode::BT_GATT

