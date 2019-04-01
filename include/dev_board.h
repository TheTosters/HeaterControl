#pragma once

#include "pca10056.h"

#define CONFIG_SDA_PIN      NRF_GPIO_PIN_MAP(0, 26)
#define CONFIG_SCL_PIN      NRF_GPIO_PIN_MAP(0, 27)
#define CONFIG_SSD1306_I2C_ADDRESS   ((uint8_t)0x3C)

#define CONFIG_SENSORS_PWR_PIN  NRF_GPIO_PIN_MAP(1, 15)
#define CONFIG_OLED_PWR_PIN     NRF_GPIO_PIN_MAP(1, 12)

#define CONFIG_DS18B20_PIN  NRF_GPIO_PIN_MAP(0,  2)
