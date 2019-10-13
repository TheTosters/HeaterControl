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

 rgb_led_emulation.h
 Created on: Oct 13, 2019
 Author: Bartłomiej Żarnowski (Toster)
 */
#ifndef rgb_led_emulation_hpp
#define rgb_led_emulation_hpp

extern "C" {
#include <components/boards/boards.h>
}
#include <stdint.h>
#include <string_view>
#include "types/hardware_pin.h"
#include "timer_owner.h"
#include "leds/led_color.h"

class RGBLedEmul: TimerOwner {
  public:
    RGBLedEmul(HardwarePin pin) :
        TimerOwner(false, RGBLedEmul::timerHandler), ledPin(pin) {
      pinUp(pin);
    }

    void setColor(const LedColor& col, const unsigned int timeoutMs) {
      setColor(col);
      timeLeft = std::min(getSequenceTime(), timeoutMs);
    }

    void setColor(const LedColor& col) {
      stopTimer();

      //this make sense while it's repeating (setColor with timeout)
      // Meaning: '.' - short pulse, '-' - long pulse, ' ' short turned off
      switch (col) {
        case LedColor::RED:
          sequence = ". ";
          break;
        case LedColor::GREEN:
          sequence = "-  ";
          break;
        case LedColor::YELLOW:
          sequence = ". -  ";
          break;
        default:
        case LedColor::BLUE:
        case LedColor::CYAN:
        case LedColor::WHITE:
        case LedColor::NONE:
          break;
      }
      index = 0;
      if (sequence.size() > 0) {
        parseSequence();
      }
    }
  private:
    static constexpr unsigned int SHORT_BLINK = 75;
    static constexpr unsigned int LONG_BLINK = 150;

    HardwarePin ledPin;
    unsigned int index;
    std::string_view sequence;
    unsigned int timeLeft;

    unsigned int getSequenceTime() {
      unsigned int result = 0;
      for (const auto& c : sequence) {
        switch (c) {
          case '-':
            result += LONG_BLINK;
            break;

          case ' ':
          case '.':
            result += SHORT_BLINK;
            break;
        }
      }
      return result;
    }

    void parseSequence() {
      if (index < sequence.size()) {
        processNextSymbol();

      } else {
        if (timeLeft > 0) {
          index = 0;
          processNextSymbol();
        }
      }
    }

    void processNextSymbol() {
      unsigned int timeout = 0;
      switch (sequence[index]) {
        case '-':
          timeout = LONG_BLINK;
          pinDown(ledPin);
          break;

        case ' ':
          timeout = SHORT_BLINK;
          pinUp(ledPin);
          break;

        case '.':
          timeout = SHORT_BLINK;
          pinDown(ledPin);
          break;
      }
      index++;
      startTimer(timeout);
    }

    static void timerHandler(void* selfPtr) {
      static_cast<RGBLedEmul*>(selfPtr)->parseSequence();
    }

    void pinUp(const HardwarePin& pin) {
      nrf_gpio_cfg(pin.get(), NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLUP,
          NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
    }

    void pinDown(const HardwarePin& pin) {
      nrf_gpio_cfg(pin.get(), NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT, NRF_GPIO_PIN_PULLDOWN,
          NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
    }
};

#endif /* rgb_led_emulation_hpp */
