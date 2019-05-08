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
#include "sdk_config.h"
}

#include "bridges/i2c_bridge.h"
#include "buttons.h"
#include "display.h"
#include "sensors/sensor_factory.h"
#include "bluetooth/btle_transmiter.h"
#include "calendar.h"
#include "resources/xbm_icons.h"
#include "screens/default_screen.h"
#include "screens/status_screen.h"
#include "screens/time_setup_screen.h"
#include "screens/screens_stack.h"
#include "observable.h"
#include "events_dispatcher.h"
#include "temperatureSheduler.h"
#include "heating_model.h"
#include "types/hardware_pin.h"
#include "bluetooth/gatt_stack.h"
#include "bluetooth/services/gatt_service.h"
#include "bluetooth/services/room_state_service.h"
#include <stdint.h>

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

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info) {
  NRF_BREAKPOINT_COND;
}

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name) {
  NRF_BREAKPOINT_COND;
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
  APP_ERROR_CHECK(err_code);

  I2c_Bridge i2cBridge{HardwarePin{CONFIG_SDA_PIN},
    HardwarePin{CONFIG_SCL_PIN}, nullptr};
  //All data types which are used by classes which uses dispatching into main
  //thread should be placed here!
  EventsDispatcher<10,
    ButtonId, app_timer_event_t, BtleTransmiter*, BleEventPtr> dispatcher;
  Display display{i2cBridge};
  OnBoardSensor sensors = instantiateSensor(i2cBridge);

  Buttons buttons;
  Calendar calendar;
  TemperatureSheduler tempScheduler;
  HeatingModel heatingModel{tempScheduler};
  sensors.addObserver([&heatingModel](TemperatureC t, RelativeHumidity h) {
    heatingModel.setTempAndHum(t, h);
  });
  calendar.addObserver([&heatingModel](DecodedTime t) {
    heatingModel.setTime(t);
  });

//  BtleTransmiter btleTransmiter{sensors};
//  btleTransmiter.enable();
//  BluetoothController::getInstance();
  using GattStackType = GattStack<RoomStateService>;
  GattStackType gattStack{"PioPio"};
  gattStack.enable();
  auto& roomServ = gattStack.getService<RoomStateService>();
  auto& tempChar = roomServ.getCharacteristic<TemperatureCharacteristic>();
  sensors.addObserver([&tempChar](TemperatureC t, RelativeHumidity h) {
    float fTemp = static_cast<float>(t);
    uint16_t v = static_cast<int16_t>(10 * fTemp);
    NRF_LOG_ERROR("Temp: %d", v);
    tempChar.setValue(v);
  });


  ScreensStack stack{display};
  DefaultScreen& screen = stack.add( DefaultScreen{display} );

  sensors.addObserver([&screen](TemperatureC t, RelativeHumidity h) {
    screen.setTempAndHum(t, h);
  });
  calendar.addObserver([&screen](DecodedTime t) {screen.setTime(t);});
  heatingModel.addObserver(
      [&screen](bool h, TemperatureC t, HeatingPlan p) {
        screen.setHeatingStatus(h, t, p);
  });

  StatusScreen stScr = stack.add( StatusScreen{display} );

  stack.add( TimeSetupScreen{display, calendar});

  buttons.addObserver([&stack](ButtonId event) {stack.onButtonEvent(event);});

  stack.selectScreen(SelectedScreen::DEFAULT);

  display.sustainOn();
  stack.render();

  uint32_t myTimeStamp = millis();

//  sensors.addObserver([](float t, int h){
//  NRF_LOG_INFO("Temp: %d; Hum: %d\n", static_cast<int>(t*100), h);});

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
