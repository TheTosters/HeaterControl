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
    public CustomStack<GAPSubComp,
                       QueuedWritesSubComp,
                       ConnParamsCgfComp,
                       PeerMgrSubComp
                       > {

private:
  using GattStackType = CustomStack<GAPSubComp,
      QueuedWritesSubComp,
      ConnParamsCgfComp,
      PeerMgrSubComp
      >;

public:
  using ServicesCol = std::tuple<Services<GattStack<Services...>>...>;

  explicit GattStack(const std::string& deviceName) {
    GAPSubComp& gapSub = std::get<GAPSubComp>(subComponents);
    gapSub.deviceName = deviceName;

    BluetoothController::getInstance().addObserver([this](BleEventPtr event){
      std::apply([event](auto&&... args) {
                      ((args.onBtleEvent(event)), ...);
                  }, services);
      if (event->header.evt_id == BLE_GAP_EVT_DISCONNECTED) {
        this->advertiser.startAdv();
      }
    });
  }

  GattStack(const GattStack &) = delete;
  GattStack &operator=(const GattStack &) = delete;

  void enable() {
    GattStackType::enable();
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

    GattStackType::disable();
  }

  void collectServicesUids(ServicesUidsVect& collection) {
    std::apply([&collection](auto&&... args) {
               ((args.collectServicesUids(collection)), ...);
           }, services);
  }

  template<template <typename> class T>
  T<GattStack>& getService() {
    return std::get<T<GattStack>>(services);
  }
private:
  ServicesCol services;
  GattAdvertiser<GattStack> advertiser;
  static nrf_ble_gatt_t gattInstance;

  void initGATT() {
    ret_code_t err_code = nrf_ble_gatt_init(&gattInstance, nullptr);
    APP_ERROR_CHECK(err_code);
  }
};

template <template <typename> class... Services>
nrf_ble_gatt_t GattStack<Services...>::gattInstance;
