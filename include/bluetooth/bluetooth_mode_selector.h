#pragma once

#include "sensors/sensor_factory.h"
#include "bluetooth/btle_transmiter.h"
#include "bluetooth/gatt_stack.h"
#include "bluetooth/services/gatt_service.h"
#include "bluetooth/services/environmental_sensing_service.h"
#include "bluetooth/services/battery_service.h"
#include "bluetooth/services/current_time_service.h"
#include "bluetooth/services/device_information_service.h"
#include "events_dispatcher.h"

class BluetoothModeSelector {
public:
  enum class Mode {DISABLED, BROADCAST, GATT};
  struct Event {
    Mode newMode;
    BluetoothModeSelector& ref;
    explicit Event(Mode mode, BluetoothModeSelector& ref)
    : newMode(mode), ref(ref) {}
  };

  explicit BluetoothModeSelector(OnBoardSensor& sensors)
  : btleTransmiter{sensors} {
    sensorBridge.linkToSensors(sensors);
  }

  void setMode(Mode mode) {
    Event event{mode, *this};
    dispatchOnMainThread(event, BluetoothModeSelector::mainThreadExecutor);
  }

  Mode getMode() {
    return mode;
  }

  bool isClientConnected() {
    return mode != Mode::GATT ?
        false :
        gattStack.getService<EnvironmentalSensingService>().getConnectionHandle()
          != BLE_CONN_HANDLE_INVALID;
  }

private:
  using GattStackType = GattStack<EnvironmentalSensingService,
                                  DeviceInformationService,
                                  BatteryService,
                                  CurrentTimeService>;

  class SensorToGattBridge {
  public:
    explicit SensorToGattBridge(GattStackType& gattStack) :
      roomServ( gattStack.getService<EnvironmentalSensingService>() ),
      tempChar( roomServ.getCharacteristic<TemperatureCharacteristic>() ),
      humChar( roomServ.getCharacteristic<HumidityCharacteristic>() )
    {
    }

    void linkToSensors(OnBoardSensor& sensors) {
      sensors.addObserver([this](TemperatureC t, RelativeHumidity h) {
        onSensorValueChange(t, h);
      });
    }

    void setEnabled(bool b) {
      enabled = b;
      if (enabled) {
        tempChar.setValue(lastTemp);
        humChar.setValue(lastHum);
      }
    }
  private:
    bool enabled {false};
    TemperatureC lastTemp {0.0f};
    RelativeHumidity lastHum {0};
    EnvironmentalSensingService<GattStackType>& roomServ;
    TemperatureCharacteristic& tempChar;
    HumidityCharacteristic& humChar;

    void onSensorValueChange(TemperatureC t, RelativeHumidity h) {
      lastTemp = t;
      lastHum = h;
      if (enabled) {
        tempChar.setValue(t);
        humChar.setValue(h);
      }
    }
  };

  GattStackType gattStack{"PioPio"};
  SensorToGattBridge sensorBridge {gattStack};
  BtleTransmiter btleTransmiter;
  Mode mode{Mode::DISABLED};

  static void mainThreadExecutor(void* p_event_data, uint16_t event_size) {
    auto data = extractDispatchedData<BluetoothModeSelector::Event>(
        p_event_data, event_size);

    if (data->ref.mode != data->newMode) {
      data->ref.disableCurrent();
      data->ref.mode = data->newMode;
      data->ref.enableSelected();
    }
  }

  void enableSelected() {
    switch(mode) {
      case Mode::BROADCAST:
        btleTransmiter.enable();
        break;

      case Mode::GATT:
        gattStack.enable();
        sensorBridge.setEnabled(true);
        break;

      default:
      case Mode::DISABLED:
        break;
    }
  }

  void disableCurrent() {
    switch(mode) {
      case Mode::BROADCAST:

        btleTransmiter.disable();
        break;

      case Mode::GATT:
        sensorBridge.setEnabled(false);
        gattStack.disable();
        break;

      default:
      case Mode::DISABLED:
        break;
    }
  }
};
