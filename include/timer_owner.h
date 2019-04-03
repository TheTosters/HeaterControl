#pragma once
extern "C" {
#include "app_timer.h"
#include "nrf_log.h"
#include "nrfx_glue.h"
}

class TimerOwner {
public:
  TimerOwner(bool repeating, app_timer_timeout_handler_t TimerHandler)
    : timerId(&timer) {
    ret_code_t ret = app_timer_create(&timerId,
        repeating ? APP_TIMER_MODE_REPEATED : APP_TIMER_MODE_SINGLE_SHOT,
            TimerHandler);
    APP_ERROR_CHECK(ret);
  }

  void startTimer(unsigned int interval) {
    ret_code_t ret = app_timer_start(timerId, interval, this);
    if (ret != NRFX_SUCCESS) {
      NRF_LOG_ERROR("Can't start timer, queue full?");
    }
    APP_ERROR_CHECK(ret);
  }

  void stopTimer() {
    ret_code_t ret = app_timer_stop(timerId);
    if (ret != NRFX_SUCCESS) {
      NRF_LOG_ERROR("Can't stop timer, queue full?");
    }
    APP_ERROR_CHECK(ret);
  }

  void restartTimer(unsigned int interval) {
    stopTimer();
    startTimer(interval);
  }
protected:
  app_timer_t timer {};
  app_timer_id_t timerId;
};
