#pragma once

#include "resources/xbm_icons.h"
#include "screens/screen.h"
#include "buttons.h"
#include "display.h"
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

class DefaultScreen : public Screen {
public:
  DefaultScreen(Display& display) :
    Screen(display, SelectedScreen::DEFAULT)
  {
  }

  void render() {
    display.clear();
    drawMeasurements();
    drawTime();
    if (isHeating) {
      display.drawXbm(display.width() - flameIcon.width, 0, flameIcon);
    }
    ScreenRedraw::notify(id, false);
  }

  void setTempAndHum(float temperature, int humidity) {
    this->temperature = temperature;
    this->humidity = humidity;
    ScreenRedraw::notify(id, true);
  }

  void setTime(const std::tm time) {
    this->time = time;
    ScreenRedraw::notify(id, true);
  }

  void setHeatingIndicator(bool isHeating) {
    this->isHeating = isHeating;
    ScreenRedraw::notify(id, true);
  }

  void onButtonEvent(ButtonId event) {
    if (event == ButtonId::OK) {
      ScreenSelect::notify(SelectedScreen::STATUS);
    }
  }
private:
  int humidity{0};
  float temperature{0};
  bool isHeating{false};
  std::tm time{};

  std::vector<std::string> dayNames {
    "Niedziela", "Poniedzialek", "Wtorek", "Sroda", "Czwartek", "Piatek",
    "Sobota"};

  void drawMeasurements() {
    std::stringstream s;
    int integral = static_cast<int>(temperature);
    int fract = static_cast<int>((temperature * 100) - integral * 100);
    fract = fract < 0 ? -fract : fract;

    s.precision(2);
    s << integral << '.' << fract;

    display.selectFont(SelectedFont::LARGE);
    auto strWidth = display.getStringWidth(s.str());
    auto posX = (display.width() - strWidth) / 2;
    display.drawString(posX, 8, s.str());

    if (humidity > 0) {
      display.selectFont(SelectedFont::SMALL);
      std::stringstream s2;
      s2 << static_cast<int>(humidity) << ' ' << '%';
      display.drawString(0, 50, s2.str());
    }
  }

  void drawTime() {
    display.selectFont(SelectedFont::SMALL);
    std::stringstream s;
    s << dayNames[time.tm_wday] << ' ' <<
        std::setfill('0') << std::setw(2) <<
        time.tm_hour << ':' <<
        std::setfill('0') << std::setw(2) <<
        time.tm_min << ':' <<
        std::setfill('0') << std::setw(2) <<
        time.tm_sec;
    display.drawString(0, 0, s.str());
  }
};
