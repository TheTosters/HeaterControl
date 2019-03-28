extern "C" {
#include "nrf_delay.h"
#include "boards.h"
#include "ds18b20.h"
#include "nrf_log_ctrl.h"
#include "nrf_log.h"
#include "nrf_log_default_backends.h"
#include "app_error.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_pwr_mgmt.h"
}

#include "i2c_bridge.h"
#include "ssd1306.h"

//#include "Adafruit_GFX.h"
//#include "nrf_drv_twi.h"
//#include "SSD1306.h"


#define APP_BLE_OBSERVER_PRIO           3
#define APP_BLE_CONN_CFG_TAG            1

namespace {
I2c_Bridge i2cBridge(nullptr);
}

static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
}

static void bleStackInit(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler,
        NULL);
}

void powerManagementInit() {
  ret_code_t err_code;
  err_code = nrf_pwr_mgmt_init();
  APP_ERROR_CHECK(err_code);
}

extern uint8_t Chewy_Regular_42[];
FontBridge chewyRegularFont { Chewy_Regular_42 };

int main( int argc, const char* argv[] ) {
  NRF_LOG_INIT(NULL);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  NRF_LOG_INFO("Start-3\n");
  NRF_LOG_FLUSH();
  bsp_board_init(BSP_INIT_LEDS);
  powerManagementInit();
  i2cBridge.begin(CONFIG_SDA_PIN, CONFIG_SCL_PIN);
  bleStackInit();
  SSD1306 ssd1306(i2cBridge);
  ssd1306.begin();
  ssd1306.clear();
  ssd1306.setFont( &chewyRegularFont );
  ssd1306.drawString(0, 20, "243");
  ssd1306.updateDisplay();
  //ds18b20_setResolution(0x7F);

  /* Toggle LEDs. */
  while (true) {
    bsp_board_led_invert(0);
    nrf_delay_ms(1000);

//    float temp = ds18b20_get_temp_method_2();
//    NRF_LOG_ERROR("TEMP: %d\n", (int)(temp*10));
    NRF_LOG_FLUSH();
    NRF_LOG_PROCESS();

    //nrf_pwr_mgmt_run();
  }
  return 0;
}
