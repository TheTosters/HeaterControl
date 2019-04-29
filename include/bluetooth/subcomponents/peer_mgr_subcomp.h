#pragma once

extern "C" {
#include "app_error.h"
#include "app_util.h"
#include "nrf_log.h"
#include "peer_manager.h"
#include "peer_manager_types.h"
#include "fds.h"
}

#include "bluetooth/subcomponents/btctrl_sub_comp.h"

class PeerMgrSubComp : public BTCtrlSubComp {
public:
  void enable() {
    if (not wasInit) {
      singleEnable();
    }
  }

  void disable() {
  }
private:
  bool wasInit{};

  void singleEnable() {
    NRF_LOG_DEBUG("PeerMgrSubComp singleEnable");
    ret_code_t err_code = pm_init();
    APP_ERROR_CHECK(err_code);

    // Security parameters to be used for all security procedures.
    ble_gap_sec_params_t sec_param{};
    sec_param.bond = 1;
    sec_param.mitm = 0;
    sec_param.lesc = 0;
    sec_param.keypress = 0;
    sec_param.io_caps = BLE_GAP_IO_CAPS_NONE;
    sec_param.oob = 0;
    sec_param.min_key_size = 7;
    sec_param.max_key_size = 16;
    sec_param.kdist_own.enc = 1;
    sec_param.kdist_own.id = 1;
    sec_param.kdist_peer.enc = 1;
    sec_param.kdist_peer.id  = 1;

    err_code = pm_sec_params_set(&sec_param);
    APP_ERROR_CHECK(err_code);

    err_code = pm_register(PeerMgrSubComp::eventHandler);
    APP_ERROR_CHECK(err_code);
  }

  static void eventHandler(pm_evt_t const * p_evt) {
    ret_code_t err_code;

    switch (p_evt->evt_id) {
      case PM_EVT_BONDED_PEER_CONNECTED:
        NRF_LOG_INFO("PeerMgrSubComp: Connected to a previously bonded device.");
      break;

      case PM_EVT_CONN_SEC_SUCCEEDED:
        NRF_LOG_INFO("Connection secured: role: %d, conn_handle: 0x%x, procedure: %d.",
                     ble_conn_state_role(p_evt->conn_handle),
                     p_evt->conn_handle,
                     p_evt->params.conn_sec_succeeded.procedure);
        break;

      case PM_EVT_CONN_SEC_FAILED:
          /* Often, when securing fails, it shouldn't be restarted, for security reasons.
           * Other times, it can be restarted directly.
           * Sometimes it can be restarted, but only after changing some Security Parameters.
           * Sometimes, it cannot be restarted until the link is disconnected and reconnected.
           * Sometimes it is impossible, to secure the link, or the peer device does not support it.
           * How to handle this error is highly application dependent. */
        break;

      case PM_EVT_CONN_SEC_CONFIG_REQ:{
        // Reject pairing request from an already bonded peer.
        pm_conn_sec_config_t conn_sec_config {};
        conn_sec_config.allow_repairing = false;
        pm_conn_sec_config_reply(p_evt->conn_handle, &conn_sec_config);
        break;
      }

      case PM_EVT_STORAGE_FULL:
        // Run garbage collection on the flash.
        err_code = fds_gc();
        if (err_code == FDS_ERR_NO_SPACE_IN_QUEUES) {
            // Retry.
        } else {
            APP_ERROR_CHECK(err_code);
        }
        break;

      case PM_EVT_PEERS_DELETE_SUCCEEDED:
        //TODO: advertising_start(false);
        break;

      case PM_EVT_PEER_DATA_UPDATE_FAILED:
        // Assert.
        APP_ERROR_CHECK(p_evt->params.peer_data_update_failed.error);
        break;

      case PM_EVT_PEER_DELETE_FAILED:
        // Assert.
        APP_ERROR_CHECK(p_evt->params.peer_delete_failed.error);
        break;

      case PM_EVT_PEERS_DELETE_FAILED:
        // Assert.
        APP_ERROR_CHECK(p_evt->params.peers_delete_failed_evt.error);
        break;

      case PM_EVT_ERROR_UNEXPECTED:
        // Assert.
        APP_ERROR_CHECK(p_evt->params.error_unexpected.error);
        break;

      case PM_EVT_CONN_SEC_START:
      case PM_EVT_PEER_DATA_UPDATE_SUCCEEDED:
      case PM_EVT_PEER_DELETE_SUCCEEDED:
      case PM_EVT_LOCAL_DB_CACHE_APPLIED:
      case PM_EVT_LOCAL_DB_CACHE_APPLY_FAILED:
          // This can happen when the local DB has changed.
      case PM_EVT_SERVICE_CHANGED_IND_SENT:
      case PM_EVT_SERVICE_CHANGED_IND_CONFIRMED:
      default:
        break;
    }
  }
};
