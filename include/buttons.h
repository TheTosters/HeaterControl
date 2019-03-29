#pragma once
#include <cinttypes>

extern "C" {
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "app_config.h"
}

class Buttons {
public:
  Buttons() {
    nrf_gpio_cfg_input(CONFIG_BUTTON_1_PIN, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(CONFIG_BUTTON_2_PIN, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(CONFIG_BUTTON_3_PIN, NRF_GPIO_PIN_PULLUP);
  }
private:
};
