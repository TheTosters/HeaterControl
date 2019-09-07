extern "C" {
#include <components/libraries/log/nrf_log.h>
#include <components/libraries/util/app_error.h>
#include <components/boards/boards.h>
}
#include "events_dispatcher.h"

void ensureMainThread() {
  bool mainThread = current_int_priority_get() == APP_IRQ_PRIORITY_THREAD;
  if (not mainThread) {
    NRF_LOG_ERROR("Expectation for main thread failed");
  }
  APP_ERROR_CHECK(mainThread ? NRFX_SUCCESS : NRFX_ERROR_INVALID_STATE);
}
