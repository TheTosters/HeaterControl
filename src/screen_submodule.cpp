/*
MIT License

Copyright (c) 2018 Bartłomiej Żarnowski

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

screen_stack.cpp
Created on: Sep 16, 2019
    Author: Bartłomiej Żarnowski (Toster)
*/
extern "C" {
#include <components/boards/boards.h>
}
#include "bridges/i2c_bridge.h"
#include "screens/screens_stack.h"
#include "sensors/sensors.h"
#include "buttons.h"
#include "calendar.h"
#include "heating_model.h"
#include "display.h"

Display& display() {
  static Display* inst = new Display(i2cBridge());
  return *inst;
}

ScreensStack& screensStack() {
  static ScreensStack* inst = new ScreensStack( display() );
  return *inst;
}

void setupScreens() {
#if DISPLAY
  ScreensStack& stack = screensStack();
  DefaultScreen& screen = stack.add( DefaultScreen{display()} );

  sensors().addObserver([&screen](TemperatureC t, RelativeHumidity h, BatteryPrc  b) {
    screen.setTempAndHum(t, h);
    //TODO: Add battery to screen?
  });
  calendar().addObserver([&screen](DecodedTime t) {screen.setTime(t);});
//  heatingModel.addObserver(
//      [&screen](bool h, TemperatureC t, HeatingPlan p) {
//        screen.setHeatingStatus(h, t, p);
//  });

  StatusScreen stScr = stack.add( StatusScreen{display()} );

  stack.add( TimeSetupScreen{display(), calendar()});

  buttons().addObserver([&stack](ButtonId event) {stack.onButtonEvent(event);});

  stack.selectScreen(SelectedScreen::DEFAULT);

  display().sustainOn();
  stack.render();
#endif
}
