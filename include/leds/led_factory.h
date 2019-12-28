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

led_factory.h
Created on: Oct 13, 2019
    Author: Bartłomiej Żarnowski (Toster)
*/
#ifndef led_factory_hpp
#define led_factory_hpp

extern "C" {
#include <components/boards/boards.h>
}
#include "types/hardware_pin.h"
#include "rgb_led.h"
#include "rgb_led_emulation.h"
#include "rgb_led_none.h"

enum class RGBLedType {
  NONE, FULL_3PIN_RGB, ONE_LED_EMULATION
};

template<RGBLedType ledType>
struct LedFactoryTrait {
  using LedType = void;
  static LedType build() {
  }
};

template<>
struct LedFactoryTrait<RGBLedType::NONE> {
  using LedType = NoRGBLed;
  static LedType build() {
    return LedType{};
  }
};

//Specialisation of factory for Full RGB common catode
template<>
struct LedFactoryTrait<RGBLedType::FULL_3PIN_RGB> {
  using LedType = RGBLed;

    static LedType build() {
      return LedType { HardwarePin { CONFIG_RGB_R_PIN }, HardwarePin { CONFIG_RGB_G_PIN },
        HardwarePin {CONFIG_RGB_B_PIN } };
    }
};

//Specialisation of factory for emulation of RGB led
template<>
struct LedFactoryTrait<RGBLedType::ONE_LED_EMULATION> {
  using LedType = RGBLedEmul;
    static LedType build() {
      return LedType{ HardwarePin { CONFIG_RGB_R_PIN } };
    }
};

using OnBoardRGBLed = LedFactoryTrait<BOARD_RGB_LED>::LedType;
OnBoardRGBLed& RGBLeds();

#endif /* led_factory_hpp */
