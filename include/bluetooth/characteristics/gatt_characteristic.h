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

template<uint16_t charUid>
class BTOrgCharType {
public:
  static constexpr uint16_t charUID {charUid};
  template<typename Service>
  void processUidWithSoftDevice(const Service& service, ble_uuid_t& uuid) {
    uuid.type = BLE_UUID_TYPE_BLE;
  }
};

template<uint16_t charUid>
class CustomCharType {
public:
  static constexpr uint16_t charUID {charUid};

  template<typename Service>
  void processUidWithSoftDevice(const Service& service, ble_uuid_t& uuid) {
    ret_code_t err_code = sd_ble_uuid_vs_add(service.getBaseUid(), &uuid.type);
    APP_ERROR_CHECK(err_code);
  }
};

template<typename ValueType>
class CharValueHolder {
protected:
  ValueType value {};

  void setValue(const ValueType& newValue) {
    value = newValue;
  }
};

template<typename CharBaseType,
         typename ValueType,
         template <typename> class NOTIFY_TRAIT = CharNoNotify,
         typename READ_TRAIT = CharNoRead,
         typename WRITE_TRAIT = CharNoWrite>
class GattCharacteristic :
    public CharBaseType,
    public CharValueHolder<ValueType>,
    public NOTIFY_TRAIT<ValueType> {

private:
  using NOTIFY_TRAIT_T = NOTIFY_TRAIT<ValueType>;

public:
  template<typename Service>
  void addToStack(Service& service) {
    //TODO: This method probably can be gratly simplified by using
    //SDK function characteristic_add
    this->processUidWithSoftDevice(service, uuid);
    ret_code_t err_code;
    ble_gatts_attr_md_t attribMetadata{};
    READ_TRAIT::configureAttMeta(attribMetadata);
    WRITE_TRAIT::configureAttMeta(attribMetadata);
    attribMetadata.vloc = BLE_GATTS_VLOC_STACK;

    ble_gatts_attr_t attribCharValue{};
    attribCharValue.p_uuid = &uuid;
    attribCharValue.p_attr_md = &attribMetadata;
    attribCharValue.max_len = sizeof(ValueType);
    attribCharValue.init_len = sizeof(ValueType);
    attribCharValue.p_value = reinterpret_cast<uint8_t*>(&this->value);

    ble_gatts_char_md_t characteristicMetadata{};
    characteristicMetadata.char_props.write = WRITE_TRAIT::CHAR_PROPS_WRITE;
    characteristicMetadata.char_props.read = READ_TRAIT::CHAR_PROPS_READ;
    characteristicMetadata.char_props.notify = NOTIFY_TRAIT_T::CHAR_PROPS_NOTIFY;

    if (NOTIFY_TRAIT_T::CHAR_PROPS_NOTIFY != 0) {
      //TODO: Reconsider if this shouldn't be NOTIFY_TRAIT code
      ble_gatts_attr_md_t clientCharConfMetadata{};
      //this must be readable & writable always (at least it looks like this in tests)
      BLE_GAP_CONN_SEC_MODE_SET_OPEN(&clientCharConfMetadata.read_perm);
      BLE_GAP_CONN_SEC_MODE_SET_OPEN(&clientCharConfMetadata.write_perm);
      clientCharConfMetadata.vloc = BLE_GATTS_VLOC_STACK;
      characteristicMetadata.p_cccd_md = &clientCharConfMetadata;
    }

    err_code = sd_ble_gatts_characteristic_add(service.getHandle(),
                                       &characteristicMetadata,
                                       &attribCharValue,
                                       &handles);
    APP_ERROR_CHECK(err_code);
  }

  void setConnectionHandle(uint16_t handle) {
    connectionHandle = handle;
  }

protected:
    uint16_t connectionHandle{BLE_CONN_HANDLE_INVALID};
    ble_uuid_t uuid {CharBaseType::charUID, 0};
    ble_gatts_char_handles_t handles {};
    //ValueType value {};

    void setValue(const ValueType& newValue) {
      CharValueHolder<ValueType>::setValue(newValue);
      //value = newValue;
      if (handles.value_handle != 0) {
        ble_gatts_value_t updateValueStruct {
          sizeof(ValueType), 0, reinterpret_cast<uint8_t*>(&this->value)
        };
        ret_code_t err_code = sd_ble_gatts_value_set(connectionHandle,
            handles.value_handle, &updateValueStruct);
        APP_ERROR_CHECK(err_code);
      }
      this->btNotify(connectionHandle, handles.value_handle, this->value);
    }
};
