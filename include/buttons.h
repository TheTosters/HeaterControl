#pragma once
#include "observable.h"
#include <cinttypes>
#include <functional>
#include "events_dispatcher.h"
extern "C" {
#include <components/libraries/bsp/bsp.h>
}

enum class ButtonId{
  PREV, NEXT, OK
};

class Buttons;
Buttons& buttons();

class Buttons : public Observable<ButtonId> {
public:
  Buttons() {
    ret_code_t err_code = bsp_init(BSP_INIT_BUTTONS, Buttons::buttonsHandler);
    APP_ERROR_CHECK(err_code);
  }

private:

  static void mainThreadExecutor(void* p_event_data, uint16_t event_size) {
    ButtonId* btn = extractDispatchedData<ButtonId>(p_event_data, event_size);
    buttons().notify(*btn);
  }

  static void buttonsHandler(bsp_event_t event) {
    switch (event) {
      //PREV button pressed
      case BSP_EVENT_KEY_0:
        dispatchOnMainThread<ButtonId>(ButtonId::PREV,
            Buttons::mainThreadExecutor);
        break;

      //NEXT button pressed
      case BSP_EVENT_KEY_1:
        dispatchOnMainThread<ButtonId>(ButtonId::NEXT,
            Buttons::mainThreadExecutor);
        break;

      //OK button pressed
      case BSP_EVENT_KEY_2:
        dispatchOnMainThread<ButtonId>(ButtonId::OK,
            Buttons::mainThreadExecutor);
        break;

      default:
        return; // no implementation needed
    }
  }

};
