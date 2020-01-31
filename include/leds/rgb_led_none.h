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

rgb_led_none.h
Created on: Dec 28, 2019
    Author: Bartłomiej Żarnowski (Toster)
*/
#ifndef rgb_led_none_hpp
#define rgb_led_none_hpp

#include "leds/led_color.h"

class NoRGBLed {
public:
    void setColor(const LedColor& col, const unsigned int timeoutMs) {
      (void)col;
      (void)timeoutMs;
    }

    void setColor(const LedColor& col) {
      (void)col;
    }
};

#endif /* rgb_led_none_hpp */
