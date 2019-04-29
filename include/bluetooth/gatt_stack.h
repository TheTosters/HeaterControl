#pragma once

extern "C" {
#include "app_error.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "ble_conn_state.h"
#include "nrf_ble_gatt.h"
#include "nrf_log.h"
}

#include "types/single_instance.h"
#include "bluetooth/subcomponents/btctrl_sub_comp.h"
#include "bluetooth/bluetooth_ctrl.h"
#include "bluetooth/gatt_advertiser.h"
#include "bluetooth/custom_stack.h"
#include "bluetooth/subcomponents/queued_writes_subcomp.h"
#include "bluetooth/subcomponents/conn_params_subcomp.h"
#include "bluetooth/subcomponents/gap_subcomp.h"
#include "bluetooth/subcomponents/peer_mgr_subcomp.h"
#include <tuple>
#include <string>

//https://github.com/NordicPlayground/nrf51-ble-tutorial-service/blob/master/SDK%2015.0.0/nrf5-ble-tutorial-service/main.c
//https://devzone.nordicsemi.com/tutorials/b/bluetooth-low-energy/posts/ble-services-a-beginners-tutorial

template <template <typename> class... Services>
class GattStack :
    public SingleInstance,
    public CustomStack<GAPSubComp,
                       QueuedWritesSubComp,
                       ConnParamsCgfComp,
                       PeerMgrSubComp> {
public:
  GattStack(const std::string& deviceName) : CustomStack() {
    GAPSubComp& gapSub = std::get<GAPSubComp>(subComponents);
    gapSub.deviceName = deviceName;
  }

  GattStack(const GattStack &) = delete;
  GattStack &operator=(const GattStack &) = delete;

  void enable() {
    CustomStack::enable();
    initGATT();

    //Enable services
    std::apply([this](auto&&... args) {
           ((args.enable(*this)), ...);
       }, services);

    advertiser.enable(*this);
  }

  void disable() {
    advertiser.disable(*this);
    std::apply([this](auto&&... args) {
           ((args.disable(*this)), ...);
       }, services);

    CustomStack::disable();
  }

private:
  using ServicesCol = std::tuple<Services<GattStack<Services...>>...>;

  ServicesCol services;
  GattAdvertiser<GattStack> advertiser;
  static nrf_ble_gatt_t gattInstance;

  void initGATT() {
    //Warning: this is compilation time static macro!
    NRF_SDH_BLE_OBSERVER(gattInstanceObs,
                         NRF_BLE_GATT_BLE_OBSERVER_PRIO,
                         nrf_ble_gatt_on_ble_evt, &gattInstance);

    ret_code_t err_code = nrf_ble_gatt_init(&gattInstance, nullptr);
    APP_ERROR_CHECK(err_code);
  }
};

template <template <typename> class... Services>
nrf_ble_gatt_t GattStack<Services...>::gattInstance;
