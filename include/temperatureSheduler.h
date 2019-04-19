#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <optional>

#include "types/unit.h"
#include "weekUnit.h"

class TemperatureSheduler {
private:
    static constexpr TemperatureC DEFAULT_TEMPERAUTRE {18.0f};
    static constexpr char DEFAULT_ALIAS[] = "DEFAULT";

    struct TemperatureAlias {
        std::string name;
        TemperatureC temperature;
        TemperatureAlias(const std::string& name,
            const TemperatureC& temperature)
        : name(name), temperature(temperature) {}
    };

    std::vector<TemperatureAlias> aliases { {DEFAULT_ALIAS, DEFAULT_TEMPERAUTRE} };

    struct TemperaturePeriod {
        WeekTime startTime;
        WeekTime endTime;
        std::string temperatureAlias;
        TemperaturePeriod(const WeekTime& startTime,
            const WeekTime& endTime, const std::string& temperatureAlias) :
            startTime(startTime), endTime(endTime),
            temperatureAlias(temperatureAlias){}
    };

    using PeriodVector = std::vector<TemperaturePeriod>;
    PeriodVector periods;

    struct FindPeriodResult {
        PeriodVector::iterator insertionPosition;

        std::optional<PeriodVector::iterator> beginRemovePosition;
        std::optional<PeriodVector::iterator> endRemovePosition;

        std::optional<PeriodVector::iterator> truncateTail;
        std::optional<PeriodVector::iterator> truncateHead;
    };

    FindPeriodResult findPeriodRange(const WeekTime& startTime, const WeekTime& endTime) {

        FindPeriodResult result = {periods.end(), std::nullopt, std::nullopt,
            std::nullopt, std::nullopt};

        for (PeriodVector::iterator iterator = periods.begin();
            iterator < periods.end(); iterator++)
        {
            if (iterator->startTime == startTime)
            {
                result.insertionPosition = iterator;
                result.beginRemovePosition = iterator;
            }
            else if ( iterator->startTime > startTime )
            {
                result.insertionPosition = iterator;
                result.beginRemovePosition = iterator;

                if (startTime <= std::prev(iterator)->endTime)
                {
                    result.truncateTail = std::prev(iterator);
                }
            }

            if (iterator->endTime == endTime)
            {
                result.endRemovePosition = iterator;
                break;
            }
            else if (iterator->endTime > endTime)
            {
                result.endRemovePosition = std::prev(iterator);
                if (endTime >= iterator->startTime)
                {
                    result.truncateHead = iterator;
                }
                break;
            }
        }

        if (static_cast<bool>(result.beginRemovePosition) &&
            static_cast<bool>(result.endRemovePosition) &&
            result.beginRemovePosition.value()->startTime >
            result.endRemovePosition.value()->startTime)
        {
            result.beginRemovePosition = std::nullopt;
            result.endRemovePosition = std::nullopt;
        }

        return result;
    }

    auto findPeriod(const WeekTime& weekTime) {
        return std::find_if(periods.begin(),periods.end(),
                [&weekTime] (const TemperaturePeriod& periods)
                { return weekTime >= periods.startTime && weekTime <= periods.endTime;});
    }

    auto findAlias(const std::string& name) {
        return std::find_if(aliases.begin(),aliases.end(),
                [&name] (const TemperatureAlias& aliases)
                {return aliases.name == name;});
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
        if ( name == DEFAULT_ALIAS )
            return;

        auto aliasIterator = findAlias(name);

        if ( aliasIterator != aliases.end() )
        {
            aliases.erase(aliasIterator);
            auto removeIter = std::remove_if(
                periods.begin(), periods.end(),
                [&name] (const TemperaturePeriod& periods) {
                   return periods.temperatureAlias == name;
            });
            if (removeIter != periods.end()) {
              periods.erase(removeIter);
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
        if ( name == DEFAULT_ALIAS || startTime >= endTime)
            return false;

        auto aliasIterator = findAlias(name);

        if ( aliasIterator == aliases.end() )
            return false;

        FindPeriodResult findResult = findPeriodRange(startTime, endTime);
        if ( static_cast<bool>(findResult.truncateHead) &&
             static_cast<bool>(findResult.truncateTail) &&
             findResult.truncateHead.value() == findResult.truncateTail.value())
        {
            findResult.insertionPosition = periods.emplace(
                findResult.insertionPosition,
                endTime + std::chrono::minutes(1),
                findResult.truncateHead.value()->endTime,
                findResult.truncateHead.value()->temperatureAlias);
            findResult.truncateTail.value()->endTime =
                startTime - std::chrono::minutes(1);
        }
        else
        {
            if (findResult.truncateHead)
            {
                findResult.truncateHead.value()->startTime =
                    endTime + std::chrono::minutes(1);
            }
            if (findResult.truncateTail)
            {
                findResult.truncateTail.value()->endTime =
                    startTime - std::chrono::minutes(1);
            }
            if (findResult.beginRemovePosition)
            {
               periods.erase(findResult.beginRemovePosition.value(),
                   findResult.endRemovePosition.value());
            }
        }

        periods.emplace(findResult.insertionPosition, startTime, endTime, name);

        return true;
    }

    bool removeTemperaturePeriod(const WeekTime& weekTime) {
        auto weekTimeIterator = findPeriod(weekTime);
        if (weekTimeIterator != periods.end())
        {
            return false;
        }

        periods.erase(findPeriod(weekTime));
        return true;
    }

    size_t getPeriodsCount() const {
      return periods.size();
    }

    const TemperaturePeriod& getPeriod(int index) const {
      return periods[index];
    }
};
