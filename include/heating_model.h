#pragma once

#include "observable.h"
#include "schedule/temperatureScheduler.h"
#include "calendar.h"

enum class HeatingPlan {
  SCHEDULE, PRESELECTED
};

/* How to bind things...
 *
  TemperatureScheduler tempScheduler;
  HeatingModel heatingModel{tempScheduler};
  sensors().addObserver([&heatingModel](TemperatureC t, RelativeHumidity h, BatteryPrc  b) {
    heatingModel.setTempAndHum(t, h);
  });
    calendar.addObserver([&heatingModel](DecodedTime t) {
    heatingModel.setTime(t);
  });

 */

class HeatingModel : public Observable<bool, TemperatureC, HeatingPlan>{
public:
  HeatingPlan mode{HeatingPlan::SCHEDULE};

  HeatingModel(TemperatureScheduler& tempSch)
  : temperatureSheduler(tempSch) {
  }

  void setTime(const DecodedTime time) {
    TemperatureC newExpTemp = temperatureSheduler.getTemperature(time);
    this->time = time;
    bool newHeating = newExpTemp > temperature;;

    if (expectedTemperature != newExpTemp) {
      expectedTemperature = newExpTemp;
      lastHeating = newHeating;
      notify(newHeating, expectedTemperature, mode);
    }
  }

  void setTempAndHum(TemperatureC temperature, RelativeHumidity humidity) {
    UNUSED_PARAMETER(humidity);
    this->temperature = temperature;
    bool newHeating = getExpectedTemperature() > temperature;;

    if (lastHeating != newHeating) {
      lastHeating = newHeating;
      notify(newHeating, getExpectedTemperature(), mode);
    }
  }

  TemperatureC getExpectedTemperature() {
    if (mode == HeatingPlan::SCHEDULE) {
      return temperatureSheduler.getTemperature(time);
    }
    //TODO
    return TemperatureC{0};
  }

private:
  TemperatureScheduler& temperatureSheduler;
  TemperatureC temperature{0};
  DecodedTime time{};

  bool lastHeating{false};
  TemperatureC expectedTemperature{0};
};
