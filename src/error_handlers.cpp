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

error_handlers.cpp
Created on: Oct 9, 2019
    Author: Bartłomiej Żarnowski (Toster)
*/
extern "C" {
#include "app_error.h"
#include <modules/nrfx/hal/nrf_power.h>
}
#include "rgb_led.h"

namespace {
  uint32_t millis()
  {
    return(app_timer_cnt_get() / 32.768);
  }
  #define OVERFLOW_1 ((uint32_t)(0xFFFFFFFF/32.768))

  uint32_t compareMillis(uint32_t previousMillis, uint32_t currentMillis)
  {
    if(currentMillis < previousMillis) {
      return currentMillis + OVERFLOW_1 + 1 - previousMillis;
    } else {
      return currentMillis - previousMillis;
    }
  }

  void finalErrorNotification() {
    RGBLeds().setColor(RGBLed::Color::RED, 3 * 1000);
    uint32_t now = millis();
    while( compareMillis(now, millis()) < 4 * 1000) {
    }
    nrf_power_system_off();
  }
}
extern "C" void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info) {
  finalErrorNotification();
}

extern "C" void HardFault_Handler(void) {
  finalErrorNotification();
}

