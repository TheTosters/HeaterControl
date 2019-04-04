#pragma once

extern "C" {
#include "app_timer.h"
}

#include "timer_owner.h"
#include "observable.h"
#include <functional>
#include <stdint.h>
#include <ctime>

using CalendarObserver = std::function<void(std::tm)>;

class Calendar : TimerOwner, public Observable<CalendarObserver> {
public:
  enum Day {
    SUNDAY = 0,
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6
  };

  Calendar()
    : TimerOwner(true, Calendar::timerHandler),
      rawSeconds(0)
  {
    startTimer(TICK_DELAY);
  }

  Day getDay() {
    return static_cast<Day>(getDecodedTime().tm_wday);
  }

  std::tm getDecodedTime() {
    std::time_t tmp = rawSeconds;
    std::tm* decodedTime = std::localtime(&tmp);
    return *decodedTime;
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
