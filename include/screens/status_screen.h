#pragma once

#include "resources/xbm_icons.h"
#include "screens/screen.h"
#include "buttons.h"
#include "display.h"
#include "sensors/sensor_factory.h"
#include "bluetooth/bluetooth_ctrl.h"
#include <string>
#include <stdint.h>
#include <sstream>
#include <ios>

class StatusScreen : public Screen {
public:
  StatusScreen(Display& display) :
    Screen(display, SelectedScreen::STATUS)
  {
  }

  void render() {
    display.clear();
    display.selectFont(SelectedFont::SMALL);
    display.drawString(0, 0, "Sensor:" + OnBoardSensor::getName());
    display.drawString(0, 12, BluetoothController::getMacAddress());
    printSelection();
    ScreenRedraw::notify(id, false);
  }

  void onButtonEvent(ButtonId event) {
    switch(event){
      case ButtonId::OK:
        ScreenSelect::notify(selected->destination);
        break;

      case ButtonId::PREV:
        if (selected != options.begin()) {
          selected--;
          ScreenRedraw::notify(id, true);
        }
        break;

      case ButtonId::NEXT:
        if (std::next(selected) != options.end()) {
          selected++;
          ScreenRedraw::notify(id, true);
        }
        break;
    }
  }
private:
  struct OptionItem {
    std::string displayName;
    SelectedScreen destination;
  };

  using OptionsVect = std::vector<OptionItem>;
  OptionsVect options{
    {"Powrot", SelectedScreen::DEFAULT},
    {"Ustaw czas", SelectedScreen::TIME_SETUP},
    {"Zdalny konf.", SelectedScreen::REMOTE_CONFIG}
  };
  OptionsVect::iterator selected{options.begin()};

  void printSelection() {
    if (selected != options.begin()) {
      display.drawString(0, 40, "<");
    }
    if (std::next(selected) != options.end()) {
      display.drawString(display.width() - display.getStringWidth(">"), 40, ">");
    }
    int w = display.getStringWidth(selected->displayName);
    display.drawString((display.width() - w)/2, 40, selected->displayName);
  }
};

