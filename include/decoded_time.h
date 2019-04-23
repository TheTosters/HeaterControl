#pragma once

#include "weekUnit.h"

#include <chrono>
#include <string>
#include <vector>

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

  operator WeekTime() const {
    return WeekTime{static_cast<WeekDay>(dayOfWeek.count()), hour, minute};
  }

  template<typename ChronoDurationType>
  explicit operator ChronoDurationType() const {
    return std::chrono::duration_cast<ChronoDurationType>(
        dayOfWeek + hour + minute + second);
  }
};

const std::vector<std::string> DecodedTime::dayNames {
  "Niedziela", "Poniedzialek", "Wtorek", "Sroda", "Czwartek", "Piatek",
  "Sobota"};
