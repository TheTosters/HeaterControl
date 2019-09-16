extern "C" {
#include <components/libraries/delay/nrf_delay.h>
#include <components/boards/boards.h>
#include <components/libraries/log/nrf_log_ctrl.h>
#include <components/libraries/log/nrf_log.h>
#include <components/libraries/log/nrf_log_default_backends.h>
#include <components/libraries/util/app_error.h>
#include <components/softdevice/common/nrf_sdh.h>
#include <components/softdevice/common/nrf_sdh_ble.h>
#include <components/libraries/pwr_mgmt/nrf_pwr_mgmt.h>
#include <components/libraries/bsp/bsp.h>
#include <components/libraries/timer/app_timer.h>
#include <integration/nrfx/legacy/nrf_drv_clock.h>
#include <config/nrf52840/config/sdk_config.h>
}

#include "bridges/i2c_bridge.h"
#include "buttons.h"
#include "display.h"
#include "sensors/sensor_factory.h"
//#include "bluetooth/btle_transmiter.h"
#include "calendar.h"
#include "resources/xbm_icons.h"
#include "screens/default_screen.h"
#include "screens/status_screen.h"
#include "screens/time_setup_screen.h"
#include "screens/screens_stack.h"
#include "observable.h"
#include "events_dispatcher.h"
#include "schedule/temperatureScheduler.h"
#include "schedule/weekScheduleBuilder.h"
#include "heating_model.h"
#include "types/hardware_pin.h"
//#include "bluetooth/gatt_stack.h"
//#include "bluetooth/services/gatt_service.h"
#include "communication.h"
#include <stdint.h>

#define APP_BLE_OBSERVER_PRIO           3
#define APP_BLE_CONN_CFG_TAG            1

extern void setupScreens();

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

uint32_t millis(void)
{
  return(app_timer_cnt_get() / 32.768);
}
#define OVERFLOW_1 ((uint32_t)(0xFFFFFFFF/32.768))

uint32_t compareMillis(uint32_t previousMillis, uint32_t currentMillis)
{
  if(currentMillis < previousMillis) {
    return currentMillis + OVERFLOW_1 + 1 - previousMillis;
  } else {
    return currentMillis - previousMillis;
  }
}

int main( int argc, const char* argv[] ) {
  NRF_LOG_INIT(NULL);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  NRF_LOG_INFO("Start\n");
  NRF_LOG_FLUSH();
  bsp_board_init(BSP_INIT_LEDS);
  powerManagementInit();
  initLowFreqClock();

  uint32_t err_code = app_timer_init();
  //All data types which are used by classes which uses dispatching into main
  //thread should be placed here!
  EventsDispatcher<10,
    ButtonId, app_timer_event_t, BtleTransmiter*, BleEventPtr> dispatcher;

  TemperatureScheduler tempScheduler;
  HeatingModel heatingModel{tempScheduler};
  sensors().addObserver([&heatingModel](TemperatureC t, RelativeHumidity h, BatteryPrc  b) {
    heatingModel.setTempAndHum(t, h);
  });
  /*
  calendar.addObserver([&heatingModel](DecodedTime t) {
    heatingModel.setTime(t);
  });
*/

  Communication::build();
  Communication::start();

  setupScreens();

  uint32_t myTimeStamp = millis();

  sensors().addObserver([](TemperatureC t, RelativeHumidity h, BatteryPrc  b){
    NRF_LOG_INFO("Temp: %s; Hum: %s\n", t.toString().c_str(), h.toString().c_str() );
  });
  NRF_LOG_INFO("Loop\n");

  while (true) {
    if(compareMillis(myTimeStamp, millis()) > 1000) {
      myTimeStamp = millis();
      bsp_board_led_invert(0);
    }
    dispatcher.process();
    nrf_pwr_mgmt_run();
  }
  return 0;
}
