#pragma once

extern "C" {
#include "ble_types.h"
}
#include <vector>

using ServicesUidsVect = std::vector<ble_uuid_t>;

template<typename Stack>
class GattService {
public:
  void enable(Stack& stack) { }

  void disable(Stack& stack) { }

  ble_uuid128_t getBaseUid() { return ble_uuid128_t{}; }
private:
};
