#pragma once

extern "C" {
#include "app_timer.h"
}

#include <functional>
#include <stdint.h>
#include <ctime>
#include <algorithm>
#include <chrono>

#include "timer_owner.h"
#include "observable.h"
#include "weekUnit.h"

class Calendar : TimerOwner, public Observable<std::tm> {
public:
  Calendar()
    : TimerOwner(true, Calendar::timerHandler),
      rawSeconds(0)
  {
    startTimer(TICK_DELAY);
  }

  WeekDay getWeekDay() {
    return static_cast<WeekDay>(getDecodedTime().tm_wday);
  }

  std::tm getDecodedTime() {
    std::time_t tmp = rawSeconds;
    std::tm* decodedTime = std::localtime(&tmp);
    return *decodedTime;
  }

  WeekTime getWeekTime() {
      return WeekTime(getWeekDay(), std::chrono::hours(getDecodedTime().tm_hour), std::chrono::minutes(getDecodedTime().tm_min));
  }

  void setEpoch(uint32_t epoch) {
    rawSeconds = epoch;
  }

  uint32_t getEpoch() {
    return rawSeconds;
  }

private:
  static constexpr unsigned int SEC_PER_TICK = 10;
  static constexpr unsigned int TICK_DELAY = APP_TIMER_TICKS(SEC_PER_TICK * 1000);

  uint32_t  rawSeconds;

  static void timerHandler(void* selfPtr) {
    Calendar* self = static_cast<Calendar*>(selfPtr);
    self->rawSeconds += SEC_PER_TICK;
    self->notify( self->getDecodedTime() );
  }
};
