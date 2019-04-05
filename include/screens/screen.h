#pragma once
#include "display.h"

enum class SelectedScreen {
  DEFAULT
};

class Screen {
public:
  const SelectedScreen id;

  Screen(Display& display, const SelectedScreen id) :
    id(id), display(display) {
  }

protected:
  Display& display;
};
