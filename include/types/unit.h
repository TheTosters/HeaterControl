#pragma once

#include "types/named_value_type.h"
#include <stdint.h>
#include <cstddef>
#include <string>

template<typename T>
struct default_trait
{
    constexpr static T epsilon = 0.2f;
};

template <typename T, typename Tag, typename Traits = default_trait<T>>
class unit : public NamedValueType<T, Tag>{
private:
    constexpr static T epsilon = Traits::epsilon;

public:
    explicit constexpr unit(T value)
      : NamedValueType<T, Tag>(std::move(value)) {};

    std::string toString() const {
      if constexpr (std::is_floating_point<T>::value) {
          T frac, integral;
          frac = modf(this->value, &integral);
          frac = std::abs(frac);
          return std::to_string( static_cast<int>(integral)) + '.' +
                 std::to_string( static_cast<int>(frac * 100));

      } else {
          return std::to_string(this->value);
      }
    }

    bool operator ==(const unit& rhs) const noexcept {

        if constexpr (std::is_floating_point<T>::value)
            return (*this-rhs).abs() < unit(epsilon);
        else
            return this->value == rhs.value;
    }

    bool operator !=(const unit& rhs) const noexcept {
        return not (*this == rhs) ;
    }

    bool operator <(const unit& rhs) const noexcept {
        return this->value < rhs.value;
    }

    bool operator >=(const unit& rhs) const noexcept {
        return not (*this < rhs);
    }

    bool operator <=(const unit& rhs) const noexcept {
        return *this < rhs || *this == rhs;
    }

    bool operator >(const unit& rhs) const noexcept {
        return *this >= rhs && *this != rhs;
    }

    unit operator +(const unit& rhs) const noexcept {
        return unit(this->value + rhs.value);
    }

    unit operator -(const unit& rhs) const noexcept {
        return unit(this->value - rhs.value);
    }

    unit operator *(const int& rhs) const noexcept {
        return unit(this->value * rhs);
    }

    unit operator *(const float& rhs) const noexcept {
        return unit(this->value * rhs);
    }

    unit abs() const noexcept {
        return unit(std::abs(this->value));
    }
};

using TemperatureC = unit<float, struct TemperatureC_tag>;
using RelativeHumidity = unit<int, struct RelativeHumidity_tag>;
