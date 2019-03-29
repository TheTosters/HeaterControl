#include "buttons.h"

void Buttons::eventHandler(bsp_event_t event) {
  uint32_t err_code;
  switch (event) {
    //PREV button pressed
    case BSP_EVENT_KEY_0:
      bsp_board_led_on(1);
      break;

    //NEXT button pressed
    case BSP_EVENT_KEY_1:
      bsp_board_led_off(1);
      break;

    //OK button pressed
    case BSP_EVENT_KEY_2:
      break;

    default:
      return; // no implementation needed
  }
}
