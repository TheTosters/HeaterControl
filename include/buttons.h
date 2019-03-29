#pragma once
#include <cinttypes>

extern "C" {
#include "bsp.h"
}

class Buttons {
public:
  void begin() {
    ret_code_t err_code = bsp_init(BSP_INIT_BUTTONS, Buttons::eventHandler);
    APP_ERROR_CHECK(err_code);
  }
private:
  static void eventHandler(bsp_event_t event);
};
