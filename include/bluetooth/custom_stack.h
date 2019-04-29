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
    APP_ERROR_CHECK(btLock == BluetoothController::BTLockId::NOT_SET ?
        NRF_SUCCESS : NRF_ERROR_INVALID_STATE);

    bool succ = BluetoothController::getInstance().acquireBluetooth(btLock);
    APP_ERROR_CHECK(succ ? NRF_SUCCESS : NRF_ERROR_INVALID_STATE);
    std::apply([this](auto&&... args) {
            ((args.enable()), ...);
        }, subComponents);
  }

  void disable() {
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
  std::tuple<> components;
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
};
