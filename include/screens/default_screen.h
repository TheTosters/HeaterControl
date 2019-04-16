#pragma once

#include "resources/xbm_icons.h"
#include "screens/screen.h"
#include "buttons.h"
#include "display.h"
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
    drawHeatingMode();
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

  void setTime(const DecodedTime time) {
    this->time = time;
    ScreenRedraw::notify(id, true);
  }

  void setHeatingIndicator(bool isHeating) {
    this->isHeating = isHeating;
    ScreenRedraw::notify(id, true);
  }

  void onButtonEvent(ButtonId event) {
    switch(event) {
      case ButtonId::OK:
        ScreenSelect::notify(SelectedScreen::STATUS);
        break;

      case ButtonId::PREV:
        //TODO: Prev heating mode
        ScreenRedraw::notify(id, true);
        break;

      case ButtonId::NEXT:
        //TODO: Next heating mode
        ScreenRedraw::notify(id, true);
        break;
    }
  }

private:
  int humidity{0};
  float temperature{0};
  bool isHeating{false};
  DecodedTime time{};

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
    s << time.dayName << ' ' <<
        std::setfill('0') << std::setw(2) << time.hour.count() << ':' <<
        std::setfill('0') << std::setw(2) << time.minute.count() << ':' <<
        std::setfill('0') << std::setw(2) << time.second.count();
    display.drawString(0, 0, s.str());
  }

  void drawHeatingMode() {
    //TODO: show proper heating plan
    std::string text = "Auto";
    int len = display.getStringWidth(text);
    display.drawString(display.width() - len - 2, 50, text);
  }
};
