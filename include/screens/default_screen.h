#pragma once

#include "resources/xbm_icons.h"
#include "screens/screen.h"
#include "buttons.h"
#include "display.h"
#include "heating_model.h"
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
    ScreenRedraw::notify(id, false);
  }

  void setTempAndHum(TemperatureC temperature, RelativeHumidity humidity) {
    this->temperature = temperature;
    this->humidity = humidity;
    ScreenRedraw::notify(id, true);
  }

  void setTime(const DecodedTime time) {
    this->time = time;
    ScreenRedraw::notify(id, true);
  }

  void setHeatingStatus(bool isHeating, TemperatureC expTemp, HeatingPlan plan){
    this->isHeating = isHeating;
    this->expectedTemperature = expTemp;
    this->heatingPlan = plan;
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
  RelativeHumidity humidity{0};
  TemperatureC temperature{0};
  TemperatureC expectedTemperature{0};
  HeatingPlan heatingPlan{HeatingPlan::SCHEDULE};
  bool isHeating{false};
  DecodedTime time{};

  std::string tempToStr(const TemperatureC& temp) {
    std::stringstream s;
    float tmpF = static_cast<float>(temp);
    int integral = static_cast<int>(tmpF);
    int fract = static_cast<int>((tmpF * 100) - integral * 100);
    fract = fract < 0 ? -fract : fract;

    s.precision(2);
    s << integral << '.' << fract;
    return s.str();
  }

  void drawMeasurements() {
    std::string strTemp = tempToStr(temperature);
    display.selectFont(SelectedFont::LARGE);
    auto strWidth = display.getStringWidth(strTemp);
    auto posX = (display.width() - strWidth) / 2;
    display.drawString(posX, 8, strTemp);

    int relH = static_cast<int>(humidity);
    if (relH > 0) {
      display.selectFont(SelectedFont::SMALL);
      std::stringstream s;
      s << relH << ' ' << '%';
      display.drawString(0, 50, s.str());
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
    std::string text = heatingPlan == HeatingPlan::SCHEDULE ?
        "Auto" : "Wybr.";
    int len = display.getStringWidth(text);
    display.drawString(display.width() - len - 2, 50, text);

    std::string strTemp = tempToStr(expectedTemperature);
    len = display.getStringWidth(strTemp);
    display.drawString((display.width() - len)/2, 50, strTemp);

    if (isHeating) {
      display.drawXbm(display.width() - flameIcon.width,
          (display.height() - flameIcon.height)/2, flameIcon);
    }
  }
};
