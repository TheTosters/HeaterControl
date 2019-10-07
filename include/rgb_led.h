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

rgb_led.h
Created on: Oct 7, 2019
    Author: Bartłomiej Żarnowski (Toster)
*/
#ifndef rgb_led_hpp
#define rgb_led_hpp

extern "C" {
#include <components/boards/boards.h>
}
#include <stdint.h>
#include "types/hardware_pin.h"

class RGBLed {
public:
    enum class Color {
      NONE, RED, GREEN, BLUE, YELLOW, WHITE, CYAN
    };

    RGBLed(HardwarePin r, HardwarePin g, HardwarePin b) :
      rPin(r), gPin(g), bPin(b)
    {
      pinUp(rPin);
      pinUp(gPin);
      pinUp(bPin);
    }

    void setColor(const Color& col) {
      pinUp(rPin);
      pinUp(gPin);
      pinUp(bPin);
      switch(col) {
        case Color::RED:
          pinDown(rPin);
          break;
        case Color::GREEN:
          pinDown(gPin);
          break;
        case Color::BLUE:
          pinDown(bPin);
          break;
        case Color::YELLOW:
          pinDown(rPin);
          pinDown(gPin);
          break;
        case Color::CYAN:
          pinDown(bPin);
          pinDown(gPin);
          break;
        case Color::WHITE:
          pinDown(rPin);
          pinDown(gPin);
          pinDown(bPin);
          break;
        default:
        case Color::NONE:
          break;
      }
    }
private:
    HardwarePin rPin;
    HardwarePin gPin;
    HardwarePin bPin;

    void pinUp(const HardwarePin& pin) {
      nrf_gpio_cfg(
                    pin.get(),
                    NRF_GPIO_PIN_DIR_OUTPUT,
                    NRF_GPIO_PIN_INPUT_DISCONNECT,
                    NRF_GPIO_PIN_PULLUP,
                    NRF_GPIO_PIN_S0S1,
                    NRF_GPIO_PIN_NOSENSE);
    }

    void pinDown(const HardwarePin& pin) {
          nrf_gpio_cfg(
                        pin.get(),
                        NRF_GPIO_PIN_DIR_OUTPUT,
                        NRF_GPIO_PIN_INPUT_DISCONNECT,
                        NRF_GPIO_PIN_PULLDOWN,
                        NRF_GPIO_PIN_S0S1,
                        NRF_GPIO_PIN_NOSENSE);
        }
};

#endif /* rgb_led_hpp */
