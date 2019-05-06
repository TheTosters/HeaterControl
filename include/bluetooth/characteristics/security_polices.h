#pragma once

extern "C" {
#include "ble_gap.h"
}

class PropNoSecureNoAccess {
public:
  static void applySecurePolicy(ble_gap_conn_sec_mode_t& mode) {
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&mode);
  }
};

class PropSecureNone {
public:
  static void applySecurePolicy(ble_gap_conn_sec_mode_t& mode) {
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&mode);
  }
};

class PropSecureEncryptNoMitm {
public:
  static void applySecurePolicy(ble_gap_conn_sec_mode_t& mode) {
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&mode);
  }
};

class PropSecureEncryptMitm {
public:
  static void applySecurePolicy(ble_gap_conn_sec_mode_t& mode) {
    BLE_GAP_CONN_SEC_MODE_SET_ENC_WITH_MITM(&mode);
  }
};

class PropSecureLescMitm {
public:
  static void applySecurePolicy(ble_gap_conn_sec_mode_t& mode) {
    BLE_GAP_CONN_SEC_MODE_SET_LESC_ENC_WITH_MITM(&mode);
  }
};

class PropSecureSignedNoMitm {
public:
  static void applySecurePolicy(ble_gap_conn_sec_mode_t& mode) {
    BLE_GAP_CONN_SEC_MODE_SET_SIGNED_NO_MITM(&mode);
  }
};

class PropSecureSignedMitm {
public:
  static void applySecurePolicy(ble_gap_conn_sec_mode_t& mode) {
    BLE_GAP_CONN_SEC_MODE_SET_SIGNED_WITH_MITM(&mode);
  }
};
