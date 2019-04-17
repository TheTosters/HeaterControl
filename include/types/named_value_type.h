#pragma once

#include <memory>

template <typename T, typename Tag>
class NamedValueType {
public:
    explicit constexpr NamedValueType(T&& value) : value(std::move(value)) {};

    explicit operator T() {
        return value;
    }

    explicit operator T() const {
        return value;
    }

    const T& get() const noexcept {
      return value;
    }
protected:
    T value;
};
