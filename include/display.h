#pragma once

extern "C" {
#include "app_timer.h"
#include "nrf_log.h"
#include "boards.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
}
#include "timer_owner.h"
#include "bridges/i2c_bridge.h"
#include "ssd1306.h"
#include "bridges/icon_bridge.h"
#include "resources/fonts.h"
#include <string>

enum class SelectedFont {
  SMALL, LARGE
};

class Display : TimerOwner{
public:
  Display(I2c_Bridge& bridge)
    : TimerOwner(false, Display::timerHandler),
      ssd1306(bridge),
      largeFont { Chewy_Regular_42 },
      smallFont { ArialMT_Plain_10 },
      isPowered(false),
      selectedFont(SelectedFont::SMALL) {

    nrf_gpio_cfg_output(CONFIG_OLED_PWR_PIN);
  }

  void clear() {
    powerUp();
    ssd1306.clear();
  }

  void drawString(int x, int y, const std::string& text) {
    powerUp();
    ssd1306.drawString(x, y, text);
  }

  unsigned int getStringWidth(const std::string& text) const {
    return ssd1306.getStringWidth(text);
  }

  void update() {
    powerUp();
    ssd1306.updateDisplay();
    //this will restart timer if already running -> prolong execution
    restartTimer(TURNOFF_DELAY);
  }

  void selectFont(SelectedFont font) {
    selectedFont = font;
    if (isPowered) {
      applySelectedFont();
    }
  }

  void drawXbm(int x, int y, const IconBridge& icon) {
    powerUp();
    ssd1306.drawXbm(x, y, icon);
  }

  int width() {
    return ssd1306.width();
  }

  int height() {
    return ssd1306.width();
  }

private:
  static constexpr unsigned int TURNOFF_DELAY = APP_TIMER_TICKS(3000);
  static constexpr unsigned int WARMUP_DELAY_MS = 13;
  SSD1306 ssd1306;
  const FontBridge& largeFont;
  const FontBridge& smallFont;
  bool isPowered;
  SelectedFont selectedFont;

  void applySelectedFont() {
    if (selectedFont == SelectedFont::SMALL) {
      ssd1306.setFont(&smallFont);

    } else {
      ssd1306.setFont(&largeFont);
    }
  }

  void powerUp() {
    if (not isPowered) {
      nrf_gpio_pin_set(CONFIG_OLED_PWR_PIN);
      //TODO: this is spinlock, change to something more power efficient
      nrf_delay_ms(WARMUP_DELAY_MS);
      ssd1306.begin();
      applySelectedFont();
      isPowered = true;
    }
  }

  void powerOff() {
    isPowered = false;
    ssd1306.end();
    nrf_gpio_pin_clear(CONFIG_OLED_PWR_PIN);
  }

  static void timerHandler(void* self) {
    static_cast<Display*>(self)->powerOff();
  }

};
