#pragma once

#include "sensors/sensor_factory.h"
#include "bluetooth/btle_transmiter.h"
#include "bluetooth/gatt_stack.h"
#include "bluetooth/services/gatt_service.h"
#include "bluetooth/services/room_state_service.h"
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
        gattStack.getService<RoomStateService>().getConnectionHandle()
          != BLE_CONN_HANDLE_INVALID;
  }

private:
  using GattStackType = GattStack<RoomStateService>;

  class SensorToGattBridge {
  public:
    bool enabled {false};

    explicit SensorToGattBridge(GattStackType& gattStack) :
      roomServ( gattStack.getService<RoomStateService>() ),
      tempChar( roomServ.getCharacteristic<TemperatureCharacteristic>() ),
      humChar( roomServ.getCharacteristic<HumidityCharacteristic>() )
    {
    }

    void linkToSensors(OnBoardSensor& sensors) {
      sensors.addObserver([this](TemperatureC t, RelativeHumidity h) {
        onSensorValueChange(t, h);
      });
    }
  private:
    RoomStateService<GattStackType>& roomServ;
    TemperatureCharacteristic& tempChar;
    HumidityCharacteristic& humChar;

    void onSensorValueChange(TemperatureC t, RelativeHumidity h) {
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
        sensorBridge.enabled = true;
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
        sensorBridge.enabled = false;
        gattStack.disable();
        break;

      default:
      case Mode::DISABLED:
        break;
    }
  }
};
