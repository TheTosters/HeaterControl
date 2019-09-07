#pragma once

#include "bluetooth/bluetooth_ctrl.h"
#include "bluetooth/subcomponents/btctrl_sub_comp.h"

template <typename... SubComponents>
class CustomStack {
public:
  CustomStack() = default;
  CustomStack(const CustomStack &) = delete;
  CustomStack &operator=(const CustomStack &) = delete;

  void enable() {
    NRF_LOG_DEBUG("CustomStack enable");
    APP_ERROR_CHECK(btLock == BluetoothController::BTLockId::NOT_SET ?
        NRF_SUCCESS : NRF_ERROR_INVALID_STATE);

    bool succ = BluetoothController::getInstance().acquireBluetooth(btLock);
    APP_ERROR_CHECK(succ ? NRF_SUCCESS : NRF_ERROR_INVALID_STATE);
    std::apply([](auto&&... args) {
            ((args.enable()), ...);
        }, subComponents);

    for(int t = 0; t < 12; t++) {
      ble_uuid_t p_uuid;
      int e = sd_ble_gatts_attr_get(t, &p_uuid, nullptr);
      NRF_LOG_ERROR("Att for handle: %x, success = %d, uuid=%x, type=%d", t, e,
          p_uuid.uuid, p_uuid.type);
    }
  }

  void disable() {
    NRF_LOG_DEBUG("CustomStack disable");
    std::apply([this](auto&&... args) {
            ((args.disable()), ...);
        }, subComponents);
    BluetoothController::getInstance().releaseBluetooth(btLock);
  }

protected:
  std::tuple<SubComponents...> subComponents{};
  BluetoothController::BTLockId btLock;
};

template<>
class CustomStack<> {
public:
  void enable() {
    APP_ERROR_CHECK(btLock == BluetoothController::BTLockId::NOT_SET ?
        NRF_SUCCESS : NRF_ERROR_INVALID_STATE);

    bool succ = BluetoothController::getInstance().acquireBluetooth(btLock);
    APP_ERROR_CHECK(succ ? NRF_SUCCESS : NRF_ERROR_INVALID_STATE);
  }
  void disable() {
    BluetoothController::getInstance().releaseBluetooth(btLock);
  }
protected:
  BluetoothController::BTLockId btLock;
  std::tuple<> subComponents;
};
