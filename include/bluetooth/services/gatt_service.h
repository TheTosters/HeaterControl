#pragma once

extern "C" {
#include "ble_types.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_log.h"
}
#include <vector>
#include <tuple>
#include <functional>
#include <stdint.h>

using ServicesUidsVect = std::vector<ble_uuid_t>;

template<const ble_uuid128_t& UUID_BASE, uint16_t serviceUid>
class CustomServiceBase {
protected:
  ble_uuid128_t baseUid {UUID_BASE};
  ble_uuid_t uuid {serviceUid, 0};

  void addServiceToSoftDevice() {
    ret_code_t err_code = sd_ble_uuid_vs_add(&baseUid, &uuid.type);
    APP_ERROR_CHECK(err_code);
  }
};

template<uint16_t serviceUid>
class BTOrgServiceBase {
protected:
  ble_uuid128_t baseUid {
    {0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  };
  ble_uuid_t uuid {serviceUid, 0};

  void addServiceToSoftDevice() {
    BLE_UUID_BLE_ASSIGN(uuid, serviceUid);
  }
};


template<typename ServiceBase, typename Stack,
typename...CharacteristicTypes>
class GattService : public ServiceBase {
public:
  using SetConnectionHandleDelegate = std::function<void(uint16_t handle)>;
  explicit GattService() = default;

  void enable(Stack& stack) {
    this->addServiceToSoftDevice();

    ret_code_t err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &this->uuid, &handle);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Enabling Service");
    NRF_LOG_INFO("    UUID: 0x%04x", this->uuid.uuid);
    NRF_LOG_INFO("    Type: 0x%02x", this->uuid.type);
    NRF_LOG_INFO("  Handle: 0x%04x", handle);

    std::apply([this](auto&&... args) {
                    ((args.addToStack(*this)), ...);
                }, this->characteristics);

  }

  void disable(Stack& stack) {
    //TODO: Description of this function suggests problems...
//    ret_code_t err_code = sd_ble_uuid_vs_remove(&uuid.type);
//    APP_ERROR_CHECK(err_code);
  }

  void onBtleEvent(ble_evt_t const* event) {
    switch (event->header.evt_id) {
      case BLE_GAP_EVT_CONNECTED:
        if (connHandlerDelegate) {
          connHandlerDelegate(event->evt.gap_evt.conn_handle);
        }
        break;

      case BLE_GAP_EVT_DISCONNECTED:
        if (connHandlerDelegate) {
          connHandlerDelegate(BLE_CONN_HANDLE_INVALID);
        }
        break;

      case BLE_GATTS_EVT_WRITE:
        NRF_LOG_ERROR("Write event, handle:%x, len:%d, offset:%d",
            event->evt.gatts_evt.params.write.handle,
            event->evt.gatts_evt.params.write.len,
            event->evt.gatts_evt.params.write.offset);
        std::apply([this, event](auto&&... args) {
                            ((onWrite(args, event)), ...);
                        }, this->characteristics);
        break;

      default:
        break;
    }
  }

  ble_uuid128_t* getBaseUid() const { return &this->baseUid; }
  uint16_t getHandle() const { return handle; }

  template<typename T>
  T& getCharacteristic() {
    return std::get<T>(characteristics);
  }
protected:
  //ble_uuid128_t baseUid {UUID_BASE};
  uint16_t handle {};
  //ble_uuid_t uuid {serviceUid, 0};
  std::tuple<CharacteristicTypes...> characteristics;
  SetConnectionHandleDelegate connHandlerDelegate;

  template<typename T>
  void onWrite(T& characteristic, ble_evt_t const* event) {
    if (characteristic.getHandle() == event->evt.gatts_evt.params.write.handle) {
      characteristic.refreshFromSoftDevice();
    }
  }
};
