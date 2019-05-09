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
#include "calendar.h"
#include "resources/xbm_icons.h"
#include "screens/default_screen.h"
#include "screens/status_screen.h"
#include "screens/time_setup_screen.h"
#include "screens/screens_stack.h"
#include "screens/remote_config_screen.h"
#include "observable.h"
#include "events_dispatcher.h"
#include "schedule/temperatureScheduler.h"
#include "schedule/weekScheduleBuilder.h"
#include "heating_model.h"
#include "types/hardware_pin.h"
#include "bluetooth/bluetooth_mode_selector.h"
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
    ButtonId, app_timer_event_t, BtleTransmiter*, BleEventPtr,
    BluetoothModeSelector::Event> dispatcher;
  Display display{i2cBridge};
  OnBoardSensor sensors = instantiateSensor(i2cBridge);

  BluetoothModeSelector btSelector{sensors};
  btSelector.setMode(BluetoothModeSelector::Mode::BROADCAST);

  Buttons buttons;
  Calendar calendar;
  TemperatureScheduler tempScheduler;
  HeatingModel heatingModel{tempScheduler};
  sensors.addObserver([&heatingModel](TemperatureC t, RelativeHumidity h, BatteryPrc  b) {
    heatingModel.setTempAndHum(t, h);
  });
  /*
  calendar.addObserver([&heatingModel](DecodedTime t) {
    heatingModel.setTime(t);
  });
*/
  BluetoothController::getInstance();
  /*
  BtleTransmiter btleTransmiter{sensors};
  using GattStackType = GattStack<RoomStateService>;
  GattStackType gattStack{"PioPio"};
  gattStack.enable();
  auto& roomServ = gattStack.getService<RoomStateService>();
  auto& tempChar = roomServ.getCharacteristic<TemperatureCharacteristic>();
  auto& humChar = roomServ.getCharacteristic<HumidityCharacteristic>();
  sensors.addObserver([&tempChar, &humChar](TemperatureC t, RelativeHumidity h) {
    tempChar.setValue(t);
    humChar.setValue(h);
  });

//  ScreensStack stack{display};
//  DefaultScreen& screen = stack.add( DefaultScreen{display} );
  stack.add( RemoteConfigScreen{display, btSelector});

//  sensors.addObserver([&screen](TemperatureC t, RelativeHumidity h, BatteryPrc  b) {
//    screen.setTempAndHum(t, h);
//    //TODO: Add battery to screen?
//  });
//  calendar.addObserver([&screen](DecodedTime t) {screen.setTime(t);});
//  heatingModel.addObserver(
//      [&screen](bool h, TemperatureC t, HeatingPlan p) {
//        screen.setHeatingStatus(h, t, p);
//  });

//  StatusScreen stScr = stack.add( StatusScreen{display} );
//
//  stack.add( TimeSetupScreen{display, calendar});
//
//  buttons.addObserver([&stack](ButtonId event) {stack.onButtonEvent(event);});
//
//  stack.selectScreen(SelectedScreen::DEFAULT);
//
//  display.sustainOn();
//  stack.render();

  uint32_t myTimeStamp = millis();

//  sensors.addObserver([](float t, int h){
//  NRF_LOG_INFO("Temp: %d; Hum: %d\n", static_cast<int>(t*100), h);});
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
