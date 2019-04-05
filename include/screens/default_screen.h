#pragma once

#include "resources/xbm_icons.h"
#include "screens/screen.h"
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
    display.update();
  }

  void setTempAndHum(float temperature, int humidity) {
    this->temperature = temperature;
    this->humidity = humidity;
  }

  void setTime(const std::tm time) {
    this->time = time;
  }

  void setHeatingIndicator(bool isHeating) {
    this->isHeating = isHeating;
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
    int integral = (int)temperature;
    int fract = (int)(temperature * 100) - integral * 100;
    fract = fract < 0 ? -fract : fract;

    s.precision(2);
    s << integral << '.' << fract;

    display.selectFont(SelectedFont::LARGE);
    auto strWidth = display.getStringWidth(s.str());
    auto posX = (display.width() - strWidth) / 2;
    display.drawString(posX, 10, s.str());
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
