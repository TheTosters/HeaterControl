#pragma once

extern "C" {
#include <components/libraries/scheduler/app_scheduler.h>
#include <components/libraries/util/app_error.h>
#include <components/libraries/util/app_util_platform.h>
#include <components/libraries/log/nrf_log.h>
#include <integration/nrfx/nrfx_glue.h>
}
#include <stdint.h>
#include <math.h>

template<int queueSize, typename... DataTypes>
class EventsDispatcher {
public:
  EventsDispatcher() {
    constexpr size_t maxItemSize = std::max({sizeof(DataTypes)...});
    APP_SCHED_INIT(maxItemSize, queueSize);
  }

  void process() {
    app_sched_execute();
  }
};

template<typename Data>
void dispatchOnMainThread(Data data, app_sched_event_handler_t handler) {
  app_sched_event_put(&data, sizeof(data), handler);
}

template<typename Data>
Data* extractDispatchedData(void* pBuf, size_t pBufSize) {
  APP_ERROR_CHECK( sizeof(Data) == pBufSize ?
      NRFX_SUCCESS :
      NRFX_ERROR_INVALID_LENGTH);
  return reinterpret_cast<Data*>(pBuf);
}

void ensureMainThread();
