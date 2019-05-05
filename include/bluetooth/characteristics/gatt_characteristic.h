#pragma once

extern "C" {
#include "ble_gatts.h"
#include "ble.h"
#include "app_error.h"
}

#include <stdint.h>

//https://devzone.nordicsemi.com/tutorials/b/bluetooth-low-energy/posts/ble-characteristics-a-beginners-tutorial
//https://github.com/NordicPlayground/nrf5-ble-tutorial-characteristic/blob/master/our_service.c

template<uint16_t charUID, typename ValueType>
class GattCharacteristic {
public:
  GattCharacteristic(GattServiceBase& service) : service(service) {}

  void setValue(const ValueType& newValue) {
    value = newValue;
//    uint16_t len = sizeof(ValueType);
//    charAbilities.btNotify(reinterpret_cast<const uint8_t*>(&value), len);
    /*if (service.getConnectionHandle() != BLE_CONN_HANDLE_INVALID) {
      uint16_t len = sizeof(ValueType);
      ble_gatts_hvx_params_t hvx_params{};

      hvx_params.handle = this->handles.value_handle;
      hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
      hvx_params.offset = 0;
      hvx_params.p_len  = &len;
      hvx_params.p_data = reinterpret_cast<const uint8_t*>(&value);

      sd_ble_gatts_hvx(service.getConnectionHandle(), &hvx_params);
    }*/
  }

  template<typename Service>
  void addToStack(Service& service) {
    ret_code_t err_code = sd_ble_uuid_vs_add(service.getBaseUid(), &uuid.type);
    APP_ERROR_CHECK(err_code);

    ble_gatts_attr_md_t confClientCharMetadata{};
    ble_gatts_attr_md_t attribMetadata{};
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attribMetadata.read_perm);
    attribMetadata.vloc = BLE_GATTS_VLOC_STACK;

    ble_gatts_attr_t attribCharValue{};
    attribCharValue.p_uuid = &uuid;
    attribCharValue.p_attr_md = &attribMetadata;
    attribCharValue.max_len = sizeof(ValueType);
    attribCharValue.init_len = sizeof(ValueType);
    attribCharValue.p_value = reinterpret_cast<const uint8_t*>(&value);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&confClientCharMetadata.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&confClientCharMetadata.write_perm);
    confClientCharMetadata.vloc = BLE_GATTS_VLOC_STACK;

    ble_gatts_char_md_t characteristicMetadata{};
    characteristicMetadata.char_props.read = 1;
    characteristicMetadata.char_props.notify = 1;
    characteristicMetadata.p_cccd_md = &confClientCharMetadata;

    err_code = sd_ble_gatts_characteristic_add(service.service_handle,
                                       &characteristicMetadata,
                                       &attribCharValue,
                                       &handles);
    APP_ERROR_CHECK(err_code);
  }
protected:
    GattServiceBase& service;
    ble_uuid_t uuid {charUID, 0};
    ble_gatts_char_handles_t handles {};
    ValueType value {};
};
