#pragma once

#include "resources/xbm_icons.h"
#include "screens/screen.h"
#include "buttons.h"
#include "display.h"
#include "sensors/sensor_factory.h"
#include "bluetooth/bluetooth_ctrl.h"
#include "bluetooth/bluetooth_mode_selector.h"
#include "timer_owner.h"
#include <string>
#include <stdint.h>
#include <sstream>
#include <ios>

class RemoteConfigScreen : public TimerOwner, public Screen {
public:
  RemoteConfigScreen(Display& display, BluetoothModeSelector& btSelector) :
    TimerOwner(false, RemoteConfigScreen::timerHandler),
    Screen(display, SelectedScreen::REMOTE_CONFIG), btSelector(btSelector)
  {
  }

  void render() {
    if (btSelector.getMode() != BluetoothModeSelector::Mode::GATT) {
      enableGattMode();
      startTimer(REFRESH_INTERVAL);
    }
    display.clear();
    display.selectFont(SelectedFont::SMALL);
    display.drawString(0, 0, "MAC:");
    display.drawString(0, 12, BluetoothController::getMacAddress());
    drawBTStatus();

    display.drawString(0, 50, "OK -> Powrot");
    ScreenRedraw::notify(id, false);
  }

  void onButtonEvent(ButtonId event) {
    switch(event){
      case ButtonId::OK:
        enableBroadcastMode();
        stopTimer();
        ScreenSelect::notify(SelectedScreen::STATUS);
        break;

      case ButtonId::NEXT:
      case ButtonId::PREV:
        break;
    }
  }
private:
  static constexpr unsigned int REFRESH_INTERVAL = APP_TIMER_TICKS(3 * 1000);
  BluetoothModeSelector& btSelector;

  static void timerHandler(void* selfPtr) {
    RemoteConfigScreen* self = static_cast<RemoteConfigScreen*>(selfPtr);
    self->ScreenRedraw::notify(self->id, true);
    //we want screen to be whole time powered in this mode
    self->display.sustainOn();
    self->startTimer(REFRESH_INTERVAL);
  }

  void enableGattMode() {
    btSelector.setMode(BluetoothModeSelector::Mode::GATT);
  }

  void enableBroadcastMode() {
    btSelector.setMode(BluetoothModeSelector::Mode::BROADCAST);
  }

  void drawBTStatus() {
    if (btSelector.isClientConnected()) {
      display.drawString(0, 24, "Status: Polaczony");

    } else {
      display.drawString(0, 24, "Status: Widoczny");
    }
  }
};

