#pragma once

#include <tuple>

class BTCtrlSubComp {
public:
  /** Called when BT is starting */
  void enable() {}

  /** Called when BT is stopping */
  void disable() {}

  static void onSdkError(ret_code_t err) {
    APP_ERROR_CHECK(err);
  }
};
