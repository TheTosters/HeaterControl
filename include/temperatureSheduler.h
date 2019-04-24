#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>

#include "types/unit.h"
#include "weekUnit.h"

struct TemperaturePeriod : public WeekTimeRange {
        std::string temperatureAlias;
    TemperaturePeriod(
        const WeekTime& startTime,
        const WeekTime& endTime,
        const std::string& temperatureAlias)
    : WeekTimeRange(startTime, endTime),
      temperatureAlias(temperatureAlias){}
};

struct TemperatureAlias {
    std::string name;
    TemperatureC temperature;
    TemperatureAlias(const std::string& name,
        const TemperatureC& temperature)
    : name(name), temperature(temperature) {}
};

class TemperatureSheduler {
private:
    using PeriodVector = std::vector<TemperaturePeriod>;
    static constexpr TemperatureC DEFAULT_TEMPERAUTRE {18.0f};
    static constexpr char DEFAULT_ALIAS[] = "DEFAULT";
    static constexpr std::chrono::seconds MINUTE{60};

    std::vector<TemperatureAlias> aliases {
      {DEFAULT_ALIAS, DEFAULT_TEMPERAUTRE}
    };

    PeriodVector periods;

    auto findInsertionPoint(const WeekTime& time) {
      return std::find_if(periods.begin(), periods.end(),
          [=](const TemperaturePeriod& period) {
        return period.startTime > time;
      });
    }

    auto findPeriod(const WeekTime& weekTime) {
        return std::find_if(periods.begin(),periods.end(),
                [&weekTime] (const TemperaturePeriod& periods)
                {
                  return weekTime >= periods.startTime and
                         weekTime <= periods.endTime;
                });
    }

    auto findAlias(const std::string& name) {
        return std::find_if(aliases.begin(),aliases.end(),
                [&name] (const TemperatureAlias& aliases)
                {return aliases.name == name;});
    }

    bool periodsTouches(const WeekTimeRange& first, const WeekTimeRange& second){
      return first.endTime == (second.startTime - MINUTE);
    }

    bool singleMergeInRange(const WeekTimeRange& range) {
      auto lastIter = findPeriod(range.endTime);
      if (lastIter != periods.end()) {
        lastIter = std::next(lastIter);
      }
      auto startIter = findPeriod(range.startTime);
      if (startIter != periods.begin()) {
        startIter = std::prev(startIter);
      }
      for(;startIter != lastIter; startIter++) {

        auto nextIter = std::next(startIter);
        if (nextIter == periods.end()){
          break;
        }

        if (startIter->temperatureAlias == nextIter->temperatureAlias and
            periodsTouches(*startIter, *nextIter)) {
          startIter->endTime = nextIter->endTime;
          periods.erase(nextIter);
          return true;
        }
      }
      return false;
    }

public:
    TemperatureSheduler() {}

    TemperatureSheduler(const TemperatureSheduler&) = delete;
    TemperatureSheduler& operator=(const TemperatureSheduler&) = delete;

    TemperatureC getTemperature(const WeekTime& weekTime) {
        auto weekTimeIterator = findPeriod(weekTime);
        return findAlias(weekTimeIterator != periods.end() ?
            weekTimeIterator->temperatureAlias : DEFAULT_ALIAS)->temperature;
    }

    const size_t getTemperatureAliasCount() const {
      return aliases.size();
    }

    const TemperatureAlias& getTemperatureAlias(int index) const {
      return aliases[index];
    }

    void addTemperatureAlias(const std::string& name,
                             const TemperatureC& temperature) {
        auto aliasIterator = findAlias(name);

        if ( aliasIterator != aliases.end() )
            aliasIterator->temperature = temperature;
        else
            aliases.emplace_back( name, temperature );
    }

    void removeTemperatureAlias(const std::string& name) {
      if ( name == DEFAULT_ALIAS ) {
        return;
      }

      auto aliasIterator = findAlias(name);

      if (aliasIterator != aliases.end()) {
        aliases.erase(aliasIterator);
        auto removeIter = std::remove_if(
          periods.begin(), periods.end(),
          [&name] (const TemperaturePeriod& period) {
            return period.temperatureAlias == name;
        });
        if (removeIter != periods.end()) {
          periods.erase(removeIter, periods.end());
        }
      }
    }

    void updateTemperatureAlias(const std::string& name,
                                const std::string& newName,
                                const TemperatureC& newTemperature) {
        if ( newName == DEFAULT_ALIAS || name == DEFAULT_ALIAS )
            return;

        auto aliasIterator = findAlias(name);

        if ( aliasIterator != aliases.end() )
        {
          aliasIterator->temperature = newTemperature;
          if (name != newName) {
            aliasIterator->name = newName;
            std::for_each(periods.begin(), periods.end(),
                    [&name, &newName] (TemperaturePeriod& period) {
                      if (period.temperatureAlias == name) {
                        period.temperatureAlias = newName;
                      }
                    });
          }
        }
    }

    bool setTemperaturePeriod(const WeekTime& startTime,
                              const WeekTime& endTime,
                              const std::string& name) {
      if ( name == DEFAULT_ALIAS || startTime >= endTime) {
          return false;
      }

      WeekTimeRange cleanupRange{startTime, endTime};
      removePeriodsInRange(cleanupRange);
      splitPeriodByRange(cleanupRange);
      truncateRange(cleanupRange);
      periods.emplace(findInsertionPoint(endTime), startTime, endTime, name);
      mergeInRange(cleanupRange);
      return true;
    }

    void mergeInRange(const WeekTimeRange& range) {
      while( singleMergeInRange(range) ) {
        //nothing
      }
    }

    /**
     * Removes period which contains given weekTime
     * (period startTime and endTime are inclusive)
     */
    bool removePeriodAt(const WeekTime& weekTime) {
      auto weekTimeIterator = findPeriod(weekTime);
      if (weekTimeIterator == periods.end()) {
          return false;
      }
      periods.erase(weekTimeIterator);
      return true;
    }

    /**
     * Removes all periods which are whole in range given range. If some
     * period is covered partially by range it's stay untouched.
     * @param range Range in which periods should be wiped out
     */
    void removePeriodsInRange(const WeekTimeRange& range) {
      auto removeIter = std::remove_if(
          periods.begin(), periods.end(),
          [=] (const TemperaturePeriod& period) {
             return (period.startTime >= range.startTime) and
                    (period.endTime <= range.endTime);
      });
      if (removeIter != periods.end()) {
        periods.erase(removeIter, periods.end());
      }
    }

    /**
     * If range is inside one period, then this period is split into two periods
     * leaving empty space in given range.
     * @param range Splitting range.
     */
    void splitPeriodByRange(const WeekTimeRange& range) {
      auto periodIter = findPeriod(range.startTime);
      if (periodIter != periods.end()) {
        auto endIter = findPeriod(range.endTime);
        if (periodIter == endIter) {

          //split only if we can separate in two non zero len periods
          //we DO NOT want single end truncation!
          if (periodIter->startTime < range.startTime and
              periodIter->endTime > range.endTime) {
            WeekTime newPeriodStartTime = periodIter->endTime;
            periodIter->endTime = range.startTime - MINUTE;
            periods.emplace(std::next(periodIter),
                            range.endTime + MINUTE,
                            newPeriodStartTime,
                            periodIter->temperatureAlias);
          }
        }
      }
    }

    /**
     * Equivalent to truncateTail on startTime and truncateHead on endTime.
     * Only border periods are touched, all periods inside range are not changed
     */
    void truncateRange(const WeekTimeRange& range) {
      auto startIter = findPeriod(range.startTime);
      auto endIter = findPeriod(range.endTime);
      if (startIter == endIter and endIter != periods.end()) {
        if (startIter->startTime == range.startTime) {
          truncatePeriodHead(range.endTime);

        } else {
          truncatePeriodTail(range.startTime);
        }

      } else {
        truncatePeriodTail(range.startTime);
        truncatePeriodHead(range.endTime);
      }
    }

    /**
     * If endBefore points inside known period then:
     * 1) If endBefore equals period startTime whole period is removed.
     * 2) If endBefore points after period startTime, then length of period is
     * set to one minute before endBefore.
     */
    void truncatePeriodTail(const WeekTime& endBefore) {
      auto iter = findPeriod(endBefore);
      if (iter != periods.end()) {
        if (iter->startTime == endBefore) {
          periods.erase(iter);

        } else {
          iter->endTime = endBefore - MINUTE;
        }
      }
    }

    /**
    * If startAfter points inside known period then:
    * 1) If startAfter equals period endTime whole period is removed.
    * 2) If startAfter points after period startTime, then period startTime is
    * set to one minute after startAfter argument.
    */
    void truncatePeriodHead(const WeekTime& startAfter) {
      auto iter = findPeriod(startAfter);
      if (iter != periods.end()) {
        if (iter->endTime == startAfter) {
          periods.erase(iter);

        } else {
          iter->startTime = startAfter + MINUTE;
        }
      }
    }

    size_t getPeriodsCount() const {
      return periods.size();
    }

    const TemperaturePeriod& getPeriod(int index) const {
      return periods[index];
    }
};
