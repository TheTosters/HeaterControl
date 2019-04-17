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

    struct TemperatureAliases {
        std::string alias;
        TemperatureC temperature;
        TemperatureAliases(const std::string& alias, const TemperatureC& temperature) : alias(alias), temperature(temperature){}
    };

    std::vector<TemperatureAliases> aliases { {DEFAULT_ALIAS, DEFAULT_TEMPERAUTRE} };

    struct TemperaturePeriods {
        WeekTime startTime;
        WeekTime endTime;
        std::string temperatureAlias;
        TemperaturePeriods(const WeekTime& startTime, const WeekTime& endTime, const std::string& temperatureAlias) :
            startTime(startTime), endTime(endTime), temperatureAlias(temperatureAlias){}
    };

    using PeriodVector = std::vector<TemperaturePeriods>;
    PeriodVector periods;

    struct FindPeriodResult {
        PeriodVector::iterator insertionPosition;

        std::optional<PeriodVector::iterator> beginRemovePosition;
        std::optional<PeriodVector::iterator> endRemovePosition;

        std::optional<PeriodVector::iterator> truncateTail;
        std::optional<PeriodVector::iterator> truncateHead;
    };

    FindPeriodResult findPeriodRange(const WeekTime& startTime, const WeekTime& endTime) {

        FindPeriodResult result = {periods.end()};

        for (PeriodVector::iterator iterator = periods.begin(); iterator < periods.end(); iterator++)
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

        if (result.beginRemovePosition.value()->startTime > result.endRemovePosition.value()->startTime)
        {
            result.beginRemovePosition = std::nullopt;
            result.endRemovePosition = std::nullopt;
        }

        return result;
    }

    auto findPeriod(const WeekTime& weekTime) {
        return std::find_if(periods.begin(),periods.end(),
                [&weekTime] (const TemperaturePeriods& periods)
                { return weekTime >= periods.startTime && weekTime <= periods.endTime;});
    }

    auto findAlias(const std::string& alias) {
        return std::find_if(aliases.begin(),aliases.end(),
                [&alias] (const TemperatureAliases& aliases)
                {return aliases.alias == alias;});
    }

public:
    TemperatureSheduler() {}

    TemperatureSheduler(const TemperatureSheduler&) = delete;
    TemperatureSheduler& operator=(const TemperatureSheduler&) = delete;

    TemperatureC getTemperature(const WeekTime& weekTime) {
        auto weekTimeIterator = findPeriod(weekTime);
        return findAlias(weekTimeIterator != periods.end() ? weekTimeIterator->temperatureAlias : DEFAULT_ALIAS)->temperature;
    }

    void setTemperatureAlias(const std::string& alias, const TemperatureC& temperature) {
        auto aliasIterator = findAlias(alias);

        if ( aliasIterator != aliases.end() )
            aliasIterator->temperature = temperature;
        else
            aliases.emplace_back( alias, temperature );
    }

    void removeTemperatureAlias(const std::string& alias) {
        if ( alias == DEFAULT_ALIAS )
            return;

        auto aliasIterator = findAlias(alias);

        if ( aliasIterator != aliases.end() )
        {
            aliases.erase(aliasIterator);
            periods.erase(std::remove_if(periods.begin(), periods.end(),
                    [&alias] (const TemperaturePeriods& periods)
                    {return periods.temperatureAlias == alias;}));
        }
    }

    void updateTemperatureAlias(const std::string& alias, const std::string& newAlias) {
        if ( newAlias == DEFAULT_ALIAS )
            return;

        auto aliasIterator = findAlias(alias);

        if ( aliasIterator != aliases.end() )
        {
            aliasIterator->alias = newAlias;
            std::for_each(periods.begin(), periods.end(),
                    [&alias, &newAlias] (TemperaturePeriods& periods)
                    {if (periods.temperatureAlias == alias) {periods.temperatureAlias = newAlias;}});
        }
    }

    bool setTemperaturePeriod(const WeekTime& startTime, const WeekTime& endTime, const std::string& alias) {
        if ( alias == DEFAULT_ALIAS || startTime >= endTime)
            return false;

        auto aliasIterator = findAlias(alias);

        if ( aliasIterator != aliases.end() )
            return false;

        FindPeriodResult findResult = findPeriodRange(startTime, endTime);
        if ( findResult.truncateHead && findResult.truncateTail && findResult.truncateHead == findResult.truncateTail )
        {
            findResult.insertionPosition = periods.emplace(findResult.insertionPosition,
                    endTime + std::chrono::minutes(1), findResult.truncateHead.value()->endTime, findResult.truncateHead.value()->temperatureAlias);
            findResult.truncateTail.value()->endTime = startTime - std::chrono::minutes(1);
        }
        else
        {
            if (findResult.truncateHead)
            {
                findResult.truncateHead.value()->startTime = endTime + std::chrono::minutes(1);
            }
            if (findResult.truncateTail)
            {
                findResult.truncateTail.value()->endTime = startTime - std::chrono::minutes(1);
            }
            if (findResult.beginRemovePosition)
            {
               periods.erase(findResult.beginRemovePosition.value(), findResult.endRemovePosition.value());
            }
        }

        periods.emplace(findResult.insertionPosition, startTime, endTime, alias);

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
};
