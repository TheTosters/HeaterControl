#pragma once

extern "C" {
#include "boards.h"
#include "app_timer.h"
}

#include <string>
#include <chrono>

#include "timer_owner.h"
#include "observable.h"
#include "weekUnit.h"
#include "decoded_time.h"

class Calendar : TimerOwner, public Observable<DecodedTime> {
public:
  Calendar()
    : TimerOwner(true, Calendar::timerHandler),
      rawSeconds(0)
  {
    startTimer(TICK_DELAY);
  }

  WeekDay getWeekDay() {
    return static_cast<WeekDay>(getDecodedTime().dayOfWeek.count());
  }

  DecodedTime getDecodedTime() {
    DecodedTime result;
    using namespace std::chrono;
    result.dayOfWeek = duration_cast<DecodedTime::ChronoDays>(rawSeconds);
    result.hour = duration_cast<hours>(rawSeconds);
    result.hour = result.hour % 24;
    result.minute = duration_cast<minutes>(rawSeconds);
    result.minute = result.minute % 60;
    result.second = rawSeconds;
    result.second = result.second % 60;
    result.dayName = DecodedTime::dayNames[result.dayOfWeek.count()];
    return result;
  }

  WeekTime getWeekTime() {
      return WeekTime(getWeekDay(), getDecodedTime().hour,
          getDecodedTime().minute);
  }

  void setRawTime(std::chrono::seconds epoch) {
    rawSeconds = epoch;
  }

  std::chrono::seconds getRawTime() {
    return rawSeconds;
  }

private:
  static constexpr std::chrono::seconds WEEK_SECONDS{7 * 24 * 3600};
  static constexpr unsigned int SEC_PER_TICK{10};
  static constexpr unsigned int TICK_DELAY{APP_TIMER_TICKS(SEC_PER_TICK * 1000)};

  std::chrono::seconds rawSeconds;

  static void timerHandler(void* selfPtr) {
    Calendar* self = static_cast<Calendar*>(selfPtr);
    self->rawSeconds += std::chrono::seconds{SEC_PER_TICK};
    //we measure only in range of week, so this is not a real epoch
    self->rawSeconds = self->rawSeconds % WEEK_SECONDS;
    self->notify( self->getDecodedTime() );
  }
};

Calendar& calendar();
