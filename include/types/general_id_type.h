#pragma once

#include "types/named_value_type.h"
#include <limits>

template<typename T, typename Tag>
class GeneralIdType : public NamedValueType<T, Tag> {
public:
  static constexpr GeneralIdType NOT_SET =
      GeneralIdType{std::numeric_limits<T>::max()};

  constexpr GeneralIdType() : NamedValueType<T,Tag>(NOT_SET) {};

  explicit constexpr GeneralIdType(T value)
    : NamedValueType<T, Tag>(std::move(value)) {};

  bool operator ==(const GeneralIdType& rhs) const noexcept {
    static_assert(std::is_integral<T>::value);
    return this->value == rhs.value;
  }

  bool operator !=(const GeneralIdType& rhs) const noexcept {
      return not (*this == rhs) ;
  }
};

template<typename Tag>
using TinyIdType = GeneralIdType<uint8_t, Tag>;
