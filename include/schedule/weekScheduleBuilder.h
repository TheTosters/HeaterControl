#pragma once

#include <vector>
#include <chrono>
#include <iterator>
#include <ostream>
#include "weekUnit.h"
#include "temperatureScheduler.h"

struct DayPoint {

  WeekTime timePoint{WeekDay::SUNDAY,
                     std::chrono::hours{0},
                     std::chrono::minutes{0}};
  std::string tempAlias;

  operator bool() const {
    WeekTime MAX_ALLOWED{WeekDay::SUNDAY,
                         std::chrono::hours{23},
                         std::chrono::minutes{59}};
    return timePoint <= MAX_ALLOWED;
  }
};

class DayTemplate {
public:
  //This is temperature from 00:00 hour should not be
  //same as TemperatureScheduler::DEFAULT
  std::string startTempAliast;

  void addPeriod(const std::string& alias) {
    const std::chrono::seconds DELTA{3600};

    DayPoint& dp = periods.emplace_back();
    dp.tempAlias = alias;
    if (periods.size() > 1) {
      auto it = periods.end();
      it --;
      dp.timePoint = it->timePoint + DELTA;
    }
  }

  void removePeriod(size_t index) {
    periods.erase(periods.begin() + index);
  }

  DayPoint& getPeriod(size_t index){
    return periods[index];
  }

  size_t size() const {
    return periods.size();
  }

  operator bool() const {
    return periods.empty() ? true :
        (not startTempAliast.empty()) and static_cast<bool>(periods.back());
  }
private:
  std::vector<DayPoint> periods;
};

class WeekScheduleBuilder {
public:
  DayTemplate workday;
  DayTemplate weekend;

  bool buildReadyList(TemperatureScheduler& scheduler) {
    if (workday and weekend) {
      scheduler.clearSchedule();
      emplaceDay(scheduler, WeekDay::SUNDAY, weekend);
      emplaceDay(scheduler, WeekDay::MONDAY, workday);
      emplaceDay(scheduler, WeekDay::TUESDAY, workday);
      emplaceDay(scheduler, WeekDay::WEDNESDAY, workday);
      emplaceDay(scheduler, WeekDay::THURSDAY, workday);
      emplaceDay(scheduler, WeekDay::FRIDAY, workday);
      emplaceDay(scheduler, WeekDay::SATURDAY, weekend);
      return true;
    }
    return false;
  }

  void saveWeekTemplate(std::ostream& ostream) {

  }
  void loadWeekTemplate(std::istream& istream) {

  }
private:
  void emplaceDay(TemperatureScheduler& scheduler,const WeekDay& day,
      DayTemplate& templ) {
    WeekTime start{day, std::chrono::hours{0}, std::chrono::minutes{0}};
    std::string& alias = templ.startTempAliast;
    for(int t = 0; t < templ.size(); t++) {
      auto& p = templ.getPeriod(t);
      scheduler.setTemperaturePeriod(start, p.timePoint, alias);
      start = p.timePoint;
      alias = p.tempAlias;
    }
    //close day
    WeekTime DAY_END{day, std::chrono::hours{23}, std::chrono::minutes{59}};
    scheduler.setTemperaturePeriod(start, DAY_END, alias);
  }
};
