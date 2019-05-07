#pragma once

extern "C" {
#include "ble_gatts.h"
#include "ble.h"
#include "app_error.h"
}
#include "bluetooth/characteristics/security_polices.h"
#include "bluetooth/characteristics/charcteristic_traits.h"
#include <stdint.h>

//https://devzone.nordicsemi.com/tutorials/b/bluetooth-low-energy/posts/ble-characteristics-a-beginners-tutorial
//https://github.com/NordicPlayground/nrf5-ble-tutorial-characteristic/blob/master/our_service.c

template<uint16_t charUID,
         typename ValueType,
         template <typename> class NOTIFY_TRAIT = CharNoNotify,
         typename READ_TRAIT = CharNoRead,
         typename WRITE_TRAIT = CharNoWrite>
class GattCharacteristic :
    public NOTIFY_TRAIT<ValueType> {

private:
  using NOTIFY_TRAIT_T = NOTIFY_TRAIT<ValueType>;

public:
  void setValue(const ValueType& newValue) {
    value = newValue;
    this->btNotify();
  }

  template<typename Service>
  void addToStack(Service& service) {
    ret_code_t err_code = sd_ble_uuid_vs_add(service.getBaseUid(), &uuid.type);
    NRF_LOG_ERROR("AddToStack sd_ble_uuid_vs_add: %d", err_code);
    APP_ERROR_CHECK(err_code);

    ble_gatts_attr_md_t attribMetadata{};
    READ_TRAIT::configureAttMeta(attribMetadata);
    WRITE_TRAIT::configureAttMeta(attribMetadata);
    attribMetadata.vloc = BLE_GATTS_VLOC_STACK;

    ble_gatts_attr_t attribCharValue{};
    attribCharValue.p_uuid = &uuid;
    attribCharValue.p_attr_md = &attribMetadata;
    attribCharValue.max_len = sizeof(ValueType);
    attribCharValue.init_len = sizeof(ValueType);
    attribCharValue.p_value = reinterpret_cast<uint8_t*>(&value);

    //TODO: Enabling cccd causes apperror 7 -> Invalid params, why?
    ble_gatts_attr_md_t clientCharConfMetadata{};
    memset(&clientCharConfMetadata, 0, sizeof(clientCharConfMetadata));
    READ_TRAIT::configureClientCharConfMeta(clientCharConfMetadata);
    WRITE_TRAIT::configureClientCharConfMeta(clientCharConfMetadata);
    clientCharConfMetadata.vloc = BLE_GATTS_VLOC_STACK;

    ble_gatts_char_md_t characteristicMetadata{};
    characteristicMetadata.char_props.write = WRITE_TRAIT::CHAR_PROPS_WRITE;
    characteristicMetadata.char_props.read = READ_TRAIT::CHAR_PROPS_READ;
    characteristicMetadata.char_props.notify = NOTIFY_TRAIT_T::CHAR_PROPS_NOTIFY;
    //characteristicMetadata.p_cccd_md = &clientCharConfMetadata;

    err_code = sd_ble_gatts_characteristic_add(service.getHandle(),
                                       &characteristicMetadata,
                                       &attribCharValue,
                                       &handles);
    NRF_LOG_ERROR("AddToStack sd_ble_gatts_characteristic_add: %d", err_code);
    APP_ERROR_CHECK(err_code);
  }

  void setConnectionHandle(uint16_t handle) {
    connectionHandle = handle;
  }

protected:
    uint16_t connectionHandle{BLE_CONN_HANDLE_INVALID};
    ble_uuid_t uuid {charUID, 0};
    ble_gatts_char_handles_t handles {};
    ValueType value {};
};
