#pragma once

#include "weekUnit.h"

#include <chrono>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

struct DecodedTime {
  using ChronoDays = std::chrono::duration<int64_t, std::ratio<86400>>;

  static const std::vector<std::string> dayNames;

  std::chrono::hours hour;
  std::chrono::minutes minute;
  std::chrono::seconds second;
  ChronoDays dayOfWeek;
  std::string dayName;

  DecodedTime(
      const ChronoDays dayOfWeek = ChronoDays{0},
      const std::chrono::hours = std::chrono::hours{0},
      std::chrono::minutes minute = std::chrono::minutes{0},
      std::chrono::seconds second = std::chrono::seconds{0})
  : hour(hour), minute(minute), second(second),
    dayOfWeek(dayOfWeek), dayName(DecodedTime::dayNames[dayOfWeek.count()])
  { }

  DecodedTime(
      const WeekDay day,
      const std::chrono::hours = std::chrono::hours{0},
      std::chrono::minutes minute = std::chrono::minutes{0},
      std::chrono::seconds second = std::chrono::seconds{0})
  : hour(hour), minute(minute), second(second),
    dayOfWeek(static_cast<int>(day)),
    dayName(DecodedTime::dayNames[dayOfWeek.count()])
  { }

  DecodedTime(std::chrono::seconds rawSeconds) {
      using namespace std::chrono;
      dayOfWeek = duration_cast<DecodedTime::ChronoDays>(rawSeconds);
      hour = duration_cast<hours>(rawSeconds);
      hour = hour % 24;
      minute = duration_cast<minutes>(rawSeconds);
      minute = minute % 60;
      second = rawSeconds;
      second = second % 60;
      dayName = DecodedTime::dayNames[dayOfWeek.count()];
  }

  operator WeekTime() const {
    return WeekTime{static_cast<WeekDay>(dayOfWeek.count()), hour, minute};
  }

  template<typename ChronoDurationType>
  explicit operator ChronoDurationType() const noexcept {
    return std::chrono::duration_cast<ChronoDurationType>(
        dayOfWeek + hour + minute + second);
  }

  explicit operator std::string() const noexcept {
    std::stringstream s;
    s << dayName << ' ' <<
        std::setfill('0') << std::setw(2) << hour.count() << ':' <<
        std::setfill('0') << std::setw(2) << minute.count() << ':' <<
        std::setfill('0') << std::setw(2) << second.count();
    return s.str();
  }
};

const std::vector<std::string> DecodedTime::dayNames {
  "Niedziela", "Poniedzialek", "Wtorek", "Sroda", "Czwartek", "Piatek",
  "Sobota"};
