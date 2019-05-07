#pragma once

extern "C" {
#include "ble_gatts.h"
#include "app_error.h"
}

#include "bluetooth/characteristics/security_polices.h"
#include <stdint.h>

template <typename ValueType>
class CharNoNotify {
public:
  static constexpr int CHAR_PROPS_NOTIFY = 0;
  void btNotify(uint16_t connHandle, uint16_t valueHandle,
      const ValueType& value) {
    //Just empty
  }
};

template <typename ValueType>
class CharValChangeNotify {
public:
  static constexpr int CHAR_PROPS_NOTIFY = 1;
  void btNotify(uint16_t connHandle, uint16_t valueHandle,
      const ValueType& value) {
    if (connHandle != BLE_CONN_HANDLE_INVALID) {
      uint16_t len = sizeof(ValueType);
      ble_gatts_hvx_params_t hvx_params{};

      hvx_params.handle = valueHandle;
      hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
      hvx_params.offset = 0;
      hvx_params.p_len  = &len;
      hvx_params.p_data = reinterpret_cast<const uint8_t*>(&value);

      ret_code_t err_code = sd_ble_gatts_hvx(connHandle, &hvx_params);
      NRF_LOG_ERROR("sd_ble_gatts_hvx: %d", err_code);
    }
  }
};

class CharNoRead {
public:
  static constexpr int CHAR_PROPS_READ = 0;
  static void configureAttMeta(ble_gatts_attr_md_t& attribMeta) {
    PropNoSecureNoAccess::applySecurePolicy(attribMeta.read_perm);
  }

  static void configureClientCharConfMeta(ble_gatts_attr_md_t& confClientMeta) {
    PropNoSecureNoAccess::applySecurePolicy(confClientMeta.read_perm);
  }
};

template <typename SECURE_POLICY = PropSecureNone>
class CharReadable {
public:
  static constexpr int CHAR_PROPS_READ = 1;
  static void configureAttMeta(ble_gatts_attr_md_t& attribMeta) {
    SECURE_POLICY::applySecurePolicy(attribMeta.read_perm);
  }

  static void configureClientCharConfMeta(ble_gatts_attr_md_t& confClientMeta) {
    SECURE_POLICY::applySecurePolicy(confClientMeta.read_perm);
  }
};


class CharNoWrite {
public:
  static constexpr int CHAR_PROPS_WRITE = 0;
  static void configureAttMeta(ble_gatts_attr_md_t& attribMeta) {
    PropNoSecureNoAccess::applySecurePolicy(attribMeta.write_perm);
  }

  static void configureClientCharConfMeta(ble_gatts_attr_md_t& confClientMeta) {
    PropNoSecureNoAccess::applySecurePolicy(confClientMeta.write_perm);
  }
};

template <typename SECURE_POLICY = PropSecureNone>
class CharWritable {
public:
  static constexpr int CHAR_PROPS_WRITE = 1;
  static void configureAttMeta(ble_gatts_attr_md_t& attribMeta) {
    SECURE_POLICY::applySecurePolicy(attribMeta.write_perm);
  }

  static void configureClientCharConfMeta(ble_gatts_attr_md_t& confClientMeta) {
    SECURE_POLICY::applySecurePolicy(confClientMeta.write_perm);
  }
};
