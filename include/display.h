#pragma once

extern "C" {
#include "app_timer.h"
#include "nrf_log.h"
#include "boards.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
}

#include "i2c_bridge.h"
#include "ssd1306.h"
#include <string>

extern uint8_t Chewy_Regular_42[];

class Display {
public:
  Display(I2c_Bridge& bridge)
    : ssd1306(bridge),
      chewyRegularFont { Chewy_Regular_42 },
      isPowered(false),
      timerId(&timer) {
  }

  void begin() {
    nrf_gpio_cfg_output(CONFIG_OLED_PWR_PIN);
    ret_code_t ret = app_timer_create(&timerId, APP_TIMER_MODE_SINGLE_SHOT,
        Display::timerHandler);
    APP_ERROR_CHECK(ret);
  }

  void clear() {
    powerUp();
    ssd1306.clear();
  }

  void drawString(int x, int y, const std::string& text) {
    powerUp();
    ssd1306.drawString(x, y, text);
  }

  void update() {
    powerUp();
    ssd1306.updateDisplay();
    //this will restart timer if already running -> prolong execution
    startTimer();
  }

private:
  static constexpr unsigned int TURNOFF_DELAY = APP_TIMER_TICKS(3000);
  static constexpr unsigned int WARMUP_DELAY_MS = 13;
  SSD1306 ssd1306;
  FontBridge chewyRegularFont;
  bool isPowered;
  app_timer_t timer;
  app_timer_id_t timerId;

  void powerUp() {
    if (not isPowered) {
      nrf_gpio_pin_set(CONFIG_OLED_PWR_PIN);
      //TODO: this is spinlock, change to something more power efficient
      nrf_delay_ms(WARMUP_DELAY_MS);
      ssd1306.begin();
      ssd1306.setFont(&chewyRegularFont);
      isPowered = true;
    }
  }

  void powerOff() {
    isPowered = false;
    ssd1306.end();
    nrf_gpio_pin_clear(CONFIG_OLED_PWR_PIN);
  }

  void startTimer() {
    ret_code_t err_code = app_timer_stop(timerId);
    if (err_code != NRFX_SUCCESS) {
      NRF_LOG_ERROR("Can't stop timer, this is not too good...");
    }
    err_code = app_timer_start(timerId, TURNOFF_DELAY, this);
    if (err_code != NRFX_SUCCESS) {
      NRF_LOG_ERROR("Can't start timer, queue full?");
    }
    APP_ERROR_CHECK(err_code);
  }

  static void timerHandler(void* self) {
    static_cast<Display*>(self)->powerOff();
  }

};
