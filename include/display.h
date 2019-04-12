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

class Display : TimerOwner {
public:
  Display(I2c_Bridge& bridge)
    : TimerOwner(false, Display::timerHandler),
      ssd1306(bridge),
      largeFont { Chewy_Regular_42 },
      smallFont { DejaVuSans13 },
      powerIsOn(false)
  {
  }

  void clear() {
    ssd1306.clear();
  }

  void drawString(int x, int y, const std::string& text) {
    ssd1306.drawString(x, y, text);
  }

  unsigned int getStringWidth(const std::string& text) const {
    return ssd1306.getStringWidth(text);
  }

  void update() {
    if (powerIsOn) {
      ssd1306.updateDisplay();
    }
  }

  void sustainOn() {
    if (not powerIsOn) {
      powerUp();
      ssd1306.updateDisplay();
    }
    restartTimer(TURNOFF_DELAY);
  }

  void selectFont(SelectedFont font) {
    if (font == SelectedFont::SMALL) {
      ssd1306.setFont(&smallFont);

    } else {
      ssd1306.setFont(&largeFont);
    }
  }

  void drawXbm(int x, int y, const IconBridge& icon) {
    ssd1306.drawXbm(x, y, icon);
  }

  int width() {
    return ssd1306.width();
  }

  int height() {
    return ssd1306.width();
  }

  bool isPowered() {
    return powerIsOn;
  }
private:
  static constexpr unsigned int TURNOFF_DELAY = APP_TIMER_TICKS(13000);
  SSD1306 ssd1306;
  const FontBridge& largeFont;
  const FontBridge& smallFont;
  bool powerIsOn;

  void powerUp() {
    if (not powerIsOn) {
      NRF_LOG_INFO("Display PWR ON");
      ssd1306.enable();
      powerIsOn = true;
    }
  }

  void powerOff() {
    NRF_LOG_INFO("Display PWR OFF");
    powerIsOn = false;
    ssd1306.disable();
  }

  static void timerHandler(void* selfPtr) {
    Display* self = static_cast<Display*>(selfPtr);
    self->powerOff();
  }

};
