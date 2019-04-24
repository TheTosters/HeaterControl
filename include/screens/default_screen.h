#pragma once

#include "resources/xbm_icons.h"
#include "screens/screen.h"
#include "buttons.h"
#include "display.h"
#include "heating_model.h"
#include <string>
#include <vector>

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

  void drawMeasurements() {
    std::string strTemp = temperature.toString();
    display.selectFont(SelectedFont::LARGE);
    auto strWidth = display.getStringWidth(strTemp);
    auto posX = (display.width() - strWidth) / 2;
    display.drawString(posX, 8, strTemp);

    if (humidity > RelativeHumidity{0}) {
      display.selectFont(SelectedFont::SMALL);
      display.drawString(0, 50, humidity.toString() + '%');
    }
  }

  void drawTime() {
    display.selectFont(SelectedFont::SMALL);
    display.drawString(0, 0, static_cast<std::string>(time));
  }

  void drawHeatingMode() {
    //TODO: show proper heating plan
    std::string text = heatingPlan == HeatingPlan::SCHEDULE ?
        "Auto" : "Wybr.";
    int len = display.getStringWidth(text);
    display.drawString(display.width() - len - 2, 50, text);

    std::string strTemp = expectedTemperature.toString();
    len = display.getStringWidth(strTemp);
    display.drawString((display.width() - len)/2, 50, strTemp);

    if (isHeating) {
      display.drawXbm(display.width() - flameIcon.width,
          (display.height() - flameIcon.height)/2, flameIcon);
    }
  }
};
