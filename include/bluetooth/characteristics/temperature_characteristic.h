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
    template<typename Stack>
    void addToStack(GattService<Stack>& service) {
      ble_gatts_char_md_t characteristicMetadata{};
      ble_gatts_attr_md_t confClientCharMetadata{};
      ble_gatts_attr_md_t attribMetadata{};
      attribMetadata.vloc = BLE_GATTS_VLOC_STACK;

      ble_gatts_attr_t attribCharValue{};
      attribCharValue.p_uuid = &charUuid;
      attribCharValue.p_attr_md = &attribMetadata;

      ble_uuid128_t base_uuid {service.getBaseUid()};
      ret_code_t err_code = sd_ble_uuid_vs_add(&base_uuid, &charUuid.type);
      APP_ERROR_CHECK(err_code);
    }
  private:

};
