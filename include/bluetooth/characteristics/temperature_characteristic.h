#pragma once

extern "C" {
#include "ble_gatts.h"
#include "ble.h"
#include "app_error.h"
}
#include "bluetooth/characteristics/gatt_characteristic.h"
#include "bluetooth/services/gatt_service.h"

//https://devzone.nordicsemi.com/tutorials/b/bluetooth-low-energy/posts/ble-characteristics-a-beginners-tutorial
//https://github.com/NordicPlayground/nrf5-ble-tutorial-characteristic/blob/master/our_service.c


//0x2A1F -> Temperature Celsius org.bluetooth.characteristic.temperature_celsius
class TemperatureCharacteristic : public GattCharacteristic<0x2A1F> {
  public:
    template<typename Service>
    void addToStack(Service& service) {
      ret_code_t err_code = sd_ble_uuid_vs_add(service.getBaseUid(), &uuid.type);
      APP_ERROR_CHECK(err_code);

      ble_gatts_char_md_t characteristicMetadata{};
      characteristicMetadata.char_props.read = 1;

      ble_gatts_attr_md_t confClientCharMetadata{};
      ble_gatts_attr_md_t attribMetadata{};
      BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attribMetadata.read_perm);
      attribMetadata.vloc = BLE_GATTS_VLOC_STACK;

      ble_gatts_attr_t attribCharValue{};
      attribCharValue.p_uuid = &uuid;
      attribCharValue.p_attr_md = &attribMetadata;
      //TODO: extract this to template and make generic
      attribCharValue.max_len = 4;
      attribCharValue.init_len = 4;
      uint8_t value[4] = {0x12,0x34,0x56,0x78};
      attribCharValue.p_value = value;  //TODO: This suck, zombie ref


      BLE_GAP_CONN_SEC_MODE_SET_OPEN(&confClientCharMetadata.read_perm);
      BLE_GAP_CONN_SEC_MODE_SET_OPEN(&confClientCharMetadata.write_perm);
      confClientCharMetadata.vloc = BLE_GATTS_VLOC_STACK;
      characteristicMetadata.p_cccd_md = &confClientCharMetadata;
      characteristicMetadata.char_props.notify = 1;

      err_code = sd_ble_gatts_characteristic_add(service.service_handle,
                                         &characteristicMetadata,
                                         &attribCharValue,
                                         &handles);
      APP_ERROR_CHECK(err_code);
    }

    //TODO: make generic, template based
    void setValue(int value) {
//      if (connHandle != BLE_CONN_HANDLE_INVALID) {
//        uint16_t len = 4; //TODO: length should be template dependent
//        ble_gatts_hvx_params_t hvx_params{};
//
//        hvx_params.handle = this->handles.value_handle;
//        hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
//        hvx_params.offset = 0;
//        hvx_params.p_len  = &len;
//        hvx_params.p_data = reinterpret_cast<const uint8_t*>(&value);
//
//        sd_ble_gatts_hvx(connHandle, &hvx_params);
//      }
    }
  private:

};
