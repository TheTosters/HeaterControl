#pragma once

#include <stdint.h>
#include <cstddef>

struct episolon02 {
    constexpr static float epsilon = 0.2f;
};

template<typename T>
struct default_trait
{
    constexpr static T epsilon = std::numeric_limits<T>::epsilon();
};

template <typename T, typename Tag, typename Traits = default_trait<T>>
class unit {
private:
    constexpr static T epsilon = Traits::epsilon;
    T value;

public:
    explicit unit(T value) : value(value) {};

    explicit operator T() {
        return value;
    }

    explicit operator T() const {
        return value;
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

using TemperatureC = unit<float, struct TemperatureC_tag, episolon02>;
using RelativeHumidity = unit<int, struct RelativeHumidity_tag>;
