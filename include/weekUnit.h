#pragma once

#include <chrono>
#include <algorithm>

enum class WeekDay {
      SUNDAY = 0,
      MONDAY = 1,
      TUESDAY = 2,
      WEDNESDAY = 3,
      THURSDAY = 4,
      FRIDAY = 5,
      SATURDAY = 6
    };

class WeekTime {
private:
    using stdChronoDays = std::chrono::duration<int64_t, std::ratio<86400>>;

    std::chrono::seconds weekTime{0};

    WeekTime(std::chrono::seconds weekTime) : weekTime(weekTime) {};
public:
    WeekTime(WeekDay weekDay, std::chrono::hours hours, std::chrono::minutes minutes) :
        weekTime(stdChronoDays(static_cast<int>(weekDay)) + hours + minutes) {}

    WeekTime operator +(const WeekTime& rhs) const noexcept {
        return WeekTime(this->weekTime + rhs.weekTime);
    }

    WeekTime operator -(const WeekTime& rhs) const noexcept {
        return WeekTime(this->weekTime - rhs.weekTime);
    }

    WeekTime operator +(const std::chrono::seconds& rhs) const noexcept {
        return WeekTime(this->weekTime + rhs);
    }

    WeekTime operator -(const std::chrono::seconds& rhs) const noexcept {
        return WeekTime(this->weekTime - rhs);
    }

    bool operator ==(const WeekTime& rhs) const noexcept {
            return this->weekTime == rhs.weekTime;
    }

    bool operator !=(const WeekTime& rhs) const noexcept {
        return not (*this == rhs) ;
    }

    bool operator <(const WeekTime& rhs) const noexcept {
        return this->weekTime < rhs.weekTime;
    }

    bool operator >=(const WeekTime& rhs) const noexcept {
        return not (*this < rhs);
    }

    bool operator <=(const WeekTime& rhs) const noexcept {
        return *this < rhs || *this == rhs;
    }

    bool operator >(const WeekTime& rhs) const noexcept {
        return *this >= rhs && *this != rhs;
    }

    operator std::chrono::seconds() const noexcept {
      return weekTime;
    }
};
