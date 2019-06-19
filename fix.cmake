SET(SDK_SEARCH_DIRECTORIES
  ${NRF5_SDK_PATH}/components/toolchain/cmsis/include
  ${NRF5_SDK_PATH}/modules/nrfx/hal
  ${NRF5_SDK_PATH}/modules/nrfx
  ${NRF5_SDK_PATH}/integration/nrfx
  ${NRF5_SDK_PATH}/integration/nrfx/legacy
  ${NRF5_SDK_PATH}/modules/nrfx/mdk
  ${NRF5_SDK_PATH}/components/libraries/util
  ${NRF5_SDK_PATH}/components/libraries/delay
  ${NRF5_SDK_PATH}/components/boards
  ${NRF5_SDK_PATH}/components/softdevice/s112/headers
  ${NRF5_SDK_PATH}/components/libraries/log
  ${NRF5_SDK_PATH}/components/libraries/log/src
  ${NRF5_SDK_PATH}/components/libraries/memobj
  ${NRF5_SDK_PATH}/components/libraries/balloc
  ${NRF5_SDK_PATH}/components/libraries/experimental_section_vars
  ${NRF5_SDK_PATH}/components/libraries/strerror
  ${NRF5_SDK_PATH}/external/fprintf
  ${NRF5_SDK_PATH}/components/libraries/ringbuf
  ${NRF5_SDK_PATH}/components/libraries/atomic
  ${NRF5_SDK_PATH}/external/segger_rtt
  ${NRF5_SDK_PATH}/components/softdevice/common
  ${NRF5_SDK_PATH}/components/libraries/pwr_mgmt
  ${NRF5_SDK_PATH}/components/libraries/mutex
  ${NRF5_SDK_PATH}/modules/nrfx/drivers/include
  ${NRF5_SDK_PATH}/components/libraries/bsp
  ${NRF5_SDK_PATH}/components/libraries/button
  ${NRF5_SDK_PATH}/components/libraries/timer
  ${NRF5_SDK_PATH}/components/ble/common
  ${NRF5_SDK_PATH}/components/libraries/scheduler
  ${NRF5_SDK_PATH}/components/ble/ble_advertising
  ${NRF5_SDK_PATH}/components/libraries/fds
  ${NRF5_SDK_PATH}/components/ble/peer_manager
  ${NRF5_SDK_PATH}/components/ble/nrf_ble_gatt
  ${NRF5_SDK_PATH}/components/ble/nrf_ble_qwr
  ${NRF5_SDK_PATH}/components/libraries/atomic_flags
  ${NRF5_SDK_PATH}/components/libraries/atomic_fifo
  ${NRF5_SDK_PATH}/components/libraries/fstorage
)

SET(EXTRA_SDK_SOURCE_FILES
  ${NRF5_SDK_PATH}/components/boards/boards.c
  ${NRF5_SDK_PATH}/components/libraries/log/src/nrf_log_frontend.c
  ${NRF5_SDK_PATH}/components/libraries/atomic/nrf_atomic.c
  ${NRF5_SDK_PATH}/components/libraries/memobj/nrf_memobj.c
  ${NRF5_SDK_PATH}/components/libraries/ringbuf/nrf_ringbuf.c
  ${NRF5_SDK_PATH}/components/libraries/util/app_util_platform.c
  ${NRF5_SDK_PATH}/components/libraries/balloc/nrf_balloc.c
  ${NRF5_SDK_PATH}/components/libraries/log/src/nrf_log_default_backends.c
  ${NRF5_SDK_PATH}/components/libraries/log/src/nrf_log_backend_rtt.c
  ${NRF5_SDK_PATH}/components/libraries/log/src/nrf_log_backend_serial.c
  ${NRF5_SDK_PATH}/external/segger_rtt/SEGGER_RTT.c
  ${NRF5_SDK_PATH}/components/libraries/log/src/nrf_log_str_formatter.c
  ${NRF5_SDK_PATH}/external/fprintf/nrf_fprintf.c
  ${NRF5_SDK_PATH}/external/fprintf/nrf_fprintf_format.c
  ${NRF5_SDK_PATH}/components/softdevice/common/nrf_sdh.c
  ${NRF5_SDK_PATH}/components/libraries/util/app_error.c
  ${NRF5_SDK_PATH}/components/softdevice/common/nrf_sdh_ble.c
  ${NRF5_SDK_PATH}/components/libraries/experimental_section_vars/nrf_section_iter.c
  ${NRF5_SDK_PATH}/components/libraries/util/app_error_weak.c
  ${NRF5_SDK_PATH}/components/libraries/strerror/nrf_strerror.c
  ${NRF5_SDK_PATH}/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c
  ${NRF5_SDK_PATH}/integration/nrfx/legacy/nrf_drv_twi.c
  ${NRF5_SDK_PATH}/modules/nrfx/drivers/src/nrfx_twi.c
  ${NRF5_SDK_PATH}/modules/nrfx/drivers/src/nrfx_twim.c
  ${NRF5_SDK_PATH}/modules/nrfx/drivers/src/prs/nrfx_prs.c
  ${NRF5_SDK_PATH}/components/libraries/bsp/bsp.c
  ${NRF5_SDK_PATH}/components/libraries/timer/app_timer.c
  ${NRF5_SDK_PATH}/components/libraries/button/app_button.c
  ${NRF5_SDK_PATH}/modules/nrfx/drivers/src/nrfx_gpiote.c
  ${NRF5_SDK_PATH}/components/libraries/gpiote/app_gpiote.c
  ${NRF5_SDK_PATH}/components/ble/common/ble_advdata.c
  ${NRF5_SDK_PATH}/integration/nrfx/legacy/nrf_drv_clock.c
  ${NRF5_SDK_PATH}/modules/nrfx/drivers/src/nrfx_clock.c
  ${NRF5_SDK_PATH}/components/libraries/scheduler/app_scheduler.c
  #${NRF5_SDK_PATH}/components/libraries/scheduler/app_scheduler_serconn.c
  ${NRF5_SDK_PATH}/components/ble/nrf_ble_qwr/nrf_ble_qwr.c
  ${NRF5_SDK_PATH}/components/ble/common/ble_conn_params.c
  ${NRF5_SDK_PATH}/components/ble/peer_manager/peer_manager.c
  ${NRF5_SDK_PATH}/components/ble/common/ble_conn_state.c
  ${NRF5_SDK_PATH}/components/libraries/fds/fds.c
  ${NRF5_SDK_PATH}/components/ble/nrf_ble_gatt/nrf_ble_gatt.c
  ${NRF5_SDK_PATH}/components/ble/ble_advertising/ble_advertising.c
  ${NRF5_SDK_PATH}/components/libraries/atomic_flags/nrf_atflags.c
  ${NRF5_SDK_PATH}/components/ble/common/ble_srv_common.c
  ${NRF5_SDK_PATH}/components/ble/peer_manager/id_manager.c
  ${NRF5_SDK_PATH}/components/ble/peer_manager/security_manager.c
  ${NRF5_SDK_PATH}/components/ble/peer_manager/gatt_cache_manager.c
  ${NRF5_SDK_PATH}/components/ble/peer_manager/peer_data_storage.c
  ${NRF5_SDK_PATH}/components/ble/peer_manager/peer_database.c
  ${NRF5_SDK_PATH}/components/ble/peer_manager/security_dispatcher.c
  ${NRF5_SDK_PATH}/components/ble/peer_manager/gatts_cache_manager.c
  ${NRF5_SDK_PATH}/components/libraries/fstorage/nrf_fstorage.c
  ${NRF5_SDK_PATH}/components/libraries/atomic_fifo/nrf_atfifo.c
  ${NRF5_SDK_PATH}/components/libraries/fstorage/nrf_fstorage_sd.c
  ${NRF5_SDK_PATH}/components/ble/peer_manager/peer_id.c
  ${NRF5_SDK_PATH}/components/ble/peer_manager/pm_buffer.c
  ${NRF5_SDK_PATH}/modules/nrfx/drivers/src/nrfx_saadc.c
)

