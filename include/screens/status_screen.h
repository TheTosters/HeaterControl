#pragma once

#include "resources/xbm_icons.h"
#include "screens/screen.h"
#include "buttons.h"
#include "display.h"
#include "sensors/sensor_factory.h"
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
    printMacAddr();
    ScreenRedraw::notify(id, false);
  }

  void onButtonEvent(ButtonId event) {
    if (event == ButtonId::OK) {
      ScreenSelect::notify(SelectedScreen::DEFAULT);
    }
  }
private:
  void printMacAddr() {
    union {
      struct __attribute__((packed)) data {
        uint16_t hBytes;
        uint32_t lBytes;
      } dataBuf;
      uint8_t raw[sizeof(data)];
    } buffer;
    buffer.dataBuf.hBytes = (uint16_t)NRF_FICR->DEVICEADDR[1] | 0xC000;
    buffer.dataBuf.lBytes  = NRF_FICR->DEVICEADDR[0];
    std::stringstream tmp;
    tmp << std::hex << static_cast<unsigned int>(buffer.raw[1]);
    tmp << ':' << std::hex << static_cast<unsigned int>(buffer.raw[0]);
    tmp << ':' << std::hex << static_cast<unsigned int>(buffer.raw[5]);
    tmp << ':' << std::hex << static_cast<unsigned int>(buffer.raw[4]);
    tmp << ':' << std::hex << static_cast<unsigned int>(buffer.raw[3]);
    tmp << ':' << std::hex << static_cast<unsigned int>(buffer.raw[2]);
    display.drawString(0, 12, tmp.str());
  }

};
