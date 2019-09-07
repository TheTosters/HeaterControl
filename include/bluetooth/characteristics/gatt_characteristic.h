#pragma once

extern "C" {
#include "ble_gatts.h"
#include "ble.h"
#include "app_error.h"
}
#include "bluetooth/characteristics/security_polices.h"
#include "bluetooth/characteristics/charcteristic_traits.h"
#include <stdint.h>
#include <string>

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

  uint8_t* rawValuePtr() {
    return reinterpret_cast<uint8_t*>(&value);
  }

  void setRawValue(const ble_gatts_value_t& valueStruct) {
    if (valueStruct.len == maxValueLen() && valueStruct.offset == 0) {
      value = *reinterpret_cast<ValueType*>(valueStruct.p_value);

    } else {
      NRF_LOG_ERROR("Unsupported write, len:%d, expected:%d, offset:%d",
          valueStruct.len, maxValueLen(), valueStruct.offset);
    }
  }

  const uint16_t maxValueLen() const {
    return sizeof(ValueType);
  }

  const uint16_t valueLen() const {
    return sizeof(ValueType);
  }

  void setMaxValueLen(uint16_t size) {
    APP_ERROR_CHECK(NRF_ERROR_NOT_SUPPORTED);
  }
};

template<>
class CharValueHolder<std::string> {
protected:
  std::string value {};
  uint16_t maxLen {20};

  void setValue(const std::string& newValue) {
    value = newValue;
  }

  void setRawValue(const ble_gatts_value_t& valueStruct) {
    if (valueStruct.offset == 0) {
      value = std::string(reinterpret_cast<const char*>(valueStruct.p_value),
        valueStruct.len);
    } else {
      NRF_LOG_ERROR("Unsupported write(String), len:%d, expected:%d, offset:%d",
                valueStruct.len, maxValueLen(), valueStruct.offset);
    }
  }

  uint8_t* rawValuePtr() {
    const uint8_t* tmp = reinterpret_cast<const uint8_t*>(value.c_str());
    return const_cast<uint8_t*>(tmp);
  }

  const uint16_t maxValueLen() const {
    return maxLen;
  }

  const uint16_t valueLen() const {
    return value.length() > maxLen ? maxLen : value.length();
  }

  void setMaxValueLen(uint16_t size) {
    maxLen = size;
  }
};

template<typename CharBaseType,
         typename ValueType,
         typename NOTIFY_TRAIT = CharNoNotify,
         typename READ_TRAIT = CharNoRead,
         typename WRITE_TRAIT = CharNoWrite>
class GattCharacteristic :
    public CharBaseType,
    public CharValueHolder<ValueType>,
    public NOTIFY_TRAIT {

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
    attribCharValue.max_len = this->maxValueLen();
    attribCharValue.init_len = this->valueLen();
    attribCharValue.p_value = this->rawValuePtr();

    ble_gatts_char_md_t characteristicMetadata{};
    characteristicMetadata.char_props.write = WRITE_TRAIT::CHAR_PROPS_WRITE;
    characteristicMetadata.char_props.read = READ_TRAIT::CHAR_PROPS_READ;
    characteristicMetadata.char_props.notify = NOTIFY_TRAIT::CHAR_PROPS_NOTIFY;

    if (NOTIFY_TRAIT::CHAR_PROPS_NOTIFY != 0) {
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

  void setMaxLen(int size) {
    CharValueHolder<ValueType>::setMaxValueLen(size);
  }

  void refreshFromSoftDevice() {
    if (handles.value_handle != 0) {
      uint16_t buffSize = CharValueHolder<ValueType>::maxValueLen();
      uint8_t buff[buffSize + 1];
      ble_gatts_value_t valueStruct{
        buffSize, 0, &buff[0]
      };
      ret_code_t err_code = sd_ble_gatts_value_get(connectionHandle,
          handles.value_handle, &valueStruct);
      if (err_code == NRF_SUCCESS) {
        CharValueHolder<ValueType>::setRawValue(valueStruct);
      }
    }
  }

  uint16_t getHandle() const {
    return handles.value_handle;
  }

protected:
    uint16_t connectionHandle{BLE_CONN_HANDLE_INVALID};
    ble_uuid_t uuid {CharBaseType::charUID, 0};
    ble_gatts_char_handles_t handles {};

    const ValueType getValue() {
      return this->value;
    }

    void setValue(const ValueType& newValue) {
      CharValueHolder<ValueType>::setValue(newValue);
      if (handles.value_handle != 0) {
        ble_gatts_value_t updateValueStruct {
          this->valueLen(), 0, this->rawValuePtr()
        };
        ret_code_t err_code = sd_ble_gatts_value_set(connectionHandle,
            handles.value_handle, &updateValueStruct);
        APP_ERROR_CHECK(err_code);

        this->btNotify(connectionHandle, handles.value_handle, updateValueStruct);
      }
    }
};
