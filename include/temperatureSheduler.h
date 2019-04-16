#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>

#include "sensors/unit.h"
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

    std::vector<TemperaturePeriods> periods;

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

        auto startPeriod = findPeriod(startTime);
        if (endTime == startPeriod->endTime)
        {
            //|--------old--------|
            //|---old---|---new---|
            startPeriod->endTime = startTime - std::chrono::minutes(1); //old
            periods.emplace(std::next(startPeriod),startTime, endTime, alias);  //new
        }
        else if (endTime < startPeriod->endTime)
        {
            //|-------------old----------------|
            //|---old---|---new---|---newOld---|
            periods.emplace(std::next(startPeriod),endTime + std::chrono::minutes(1), startPeriod->endTime, startPeriod->temperatureAlias); //newOld
            startPeriod->endTime = startTime - std::chrono::minutes(1); //old
            periods.emplace(std::next(startPeriod),startTime, endTime, alias); //new
        }
        else if (endTime > startPeriod->endTime)
        {
            auto endPeriod = findPeriod(endTime);
            //|---old---|---old1---|---old2---|---old3---|
            //|---old-|-----------new-----------|-old3---|
            startPeriod->endTime = startTime - std::chrono::minutes(1); //old
            endPeriod->startTime = endTime+std::chrono::minutes(1); //old3
            periods.erase(std::next(startPeriod),std::prev(endPeriod)); //old1,old2
            periods.emplace(std::next(startPeriod),startTime, endTime, alias); //new
        }

        return true;
    }
};
