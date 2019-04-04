#pragma once
#include "display.h"

class Screen {
public:
  Screen(Display& display) :
    display(display) {
  }
  virtual ~Screen() = default;

  virtual void render() = 0;

protected:
  Display& display;
};
