

extern "C" {
#include "nrf_delay.h"
#include "boards.h"
#include "nrf_log_ctrl.h"
#include "nrf_log.h"
#include "nrf_log_default_backends.h"
#include "app_error.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_pwr_mgmt.h"
#include "bsp.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
}

#include "i2c_bridge.h"
#include "buttons.h"
#include "display.h"
#include "sensors.h"
#include "btle_transmiter.h"
//#include "calendar.h"
#include <sstream>

#define APP_BLE_OBSERVER_PRIO           3
#define APP_BLE_CONN_CFG_TAG            1

void powerManagementInit() {
  ret_code_t err_code;
  err_code = nrf_pwr_mgmt_init();
  APP_ERROR_CHECK(err_code);
}

void initLowFreqClock() {
  ret_code_t err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);
  nrf_drv_clock_lfclk_request(NULL);
}

int main( int argc, const char* argv[] ) {
  NRF_LOG_INIT(NULL);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  NRF_LOG_INFO("Start-3\n");
  NRF_LOG_FLUSH();
  bsp_board_init(BSP_INIT_LEDS);
  powerManagementInit();
  initLowFreqClock();

  uint32_t err_code = app_timer_init();
  I2c_Bridge i2cBridge{CONFIG_SDA_PIN, CONFIG_SCL_PIN, nullptr};
  Display display{i2cBridge};
  Sensors sensors{i2cBridge};
  Buttons buttons;
  //BtleTransmiter btleTransmiter{sensors};
  //btleTransmiter.begin();

  /* Toggle LEDs. */
  while (true) {
    bsp_board_led_invert(0);
    nrf_delay_ms(1000);
    float tempF = sensors.temperature;
    int integral = (int)tempF;
    int fract = (int)(tempF*100) - integral * 100;
    std::stringstream s;
    s.precision(2);
    s << integral << '.' << fract;
    display.clear();
    display.drawString(0, 10, s.str());
    display.update();
    //nrf_pwr_mgmt_run();
  }
  return 0;
}
