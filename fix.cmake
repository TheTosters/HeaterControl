SET(SDK_SEARCH_DIRECTORIES
  ${SDK_ROOT}/components/toolchain/cmsis/include
  ${SDK_ROOT}/modules/nrfx/hal
  ${SDK_ROOT}/modules/nrfx
  ${SDK_ROOT}/integration/nrfx
  ${SDK_ROOT}/integration/nrfx/legacy
  ${SDK_ROOT}/modules/nrfx/mdk
  ${SDK_ROOT}/components/libraries/util
  ${SDK_ROOT}/components/libraries/delay
  ${SDK_ROOT}/components/boards
  ${SDK_ROOT}/components/softdevice/s112/headers
  ${SDK_ROOT}/components/libraries/log
  ${SDK_ROOT}/components/libraries/log/src
  ${SDK_ROOT}/components/libraries/memobj
  ${SDK_ROOT}/components/libraries/balloc
  ${SDK_ROOT}/components/libraries/experimental_section_vars
  ${SDK_ROOT}/components/libraries/strerror
  ${SDK_ROOT}/external/fprintf
  ${SDK_ROOT}/components/libraries/ringbuf
  ${SDK_ROOT}/components/libraries/atomic
  ${SDK_ROOT}/external/segger_rtt
  ${SDK_ROOT}/components/softdevice/common
  ${SDK_ROOT}/components/libraries/pwr_mgmt
  ${SDK_ROOT}/components/libraries/mutex
  ${SDK_ROOT}/modules/nrfx/drivers/include
  ${SDK_ROOT}/components/libraries/bsp
  ${SDK_ROOT}/components/libraries/button
  ${SDK_ROOT}/components/libraries/timer
  ${SDK_ROOT}/components/ble/common
  ${SDK_ROOT}/components/libraries/scheduler
  ${SDK_ROOT}/components/ble/ble_advertising
  ${SDK_ROOT}/components/libraries/fds
  ${SDK_ROOT}/components/ble/peer_manager
  ${SDK_ROOT}/components/ble/nrf_ble_gatt
  ${SDK_ROOT}/components/ble/nrf_ble_qwr
  ${SDK_ROOT}/components/libraries/atomic_flags
  ${SDK_ROOT}/components/libraries/atomic_fifo
  ${SDK_ROOT}/components/libraries/fstorage
  
  
  ${SDK_ROOT}/components/libraries/bootloader
  ${SDK_ROOT}/components/libraries/bootloader/ble_dfu
  ${SDK_ROOT}/components/libraries/bootloader/dfu
  ${SDK_ROOT}/components/libraries/svc
  
  
)

SET(EXTRA_SDK_SOURCE_FILES
  ${SDK_ROOT}/components/boards/boards.c
  ${SDK_ROOT}/components/libraries/log/src/nrf_log_frontend.c
  ${SDK_ROOT}/components/libraries/atomic/nrf_atomic.c
  ${SDK_ROOT}/components/libraries/memobj/nrf_memobj.c
  ${SDK_ROOT}/components/libraries/ringbuf/nrf_ringbuf.c
  ${SDK_ROOT}/components/libraries/util/app_util_platform.c
  ${SDK_ROOT}/components/libraries/balloc/nrf_balloc.c
  ${SDK_ROOT}/components/libraries/log/src/nrf_log_default_backends.c
  ${SDK_ROOT}/components/libraries/log/src/nrf_log_backend_rtt.c
  ${SDK_ROOT}/components/libraries/log/src/nrf_log_backend_serial.c
  ${SDK_ROOT}/external/segger_rtt/SEGGER_RTT.c
  ${SDK_ROOT}/components/libraries/log/src/nrf_log_str_formatter.c
  ${SDK_ROOT}/external/fprintf/nrf_fprintf.c
  ${SDK_ROOT}/external/fprintf/nrf_fprintf_format.c
  ${SDK_ROOT}/components/softdevice/common/nrf_sdh.c
  ${SDK_ROOT}/components/libraries/util/app_error.c
  ${SDK_ROOT}/components/softdevice/common/nrf_sdh_ble.c
  ${SDK_ROOT}/components/libraries/experimental_section_vars/nrf_section_iter.c
  ${SDK_ROOT}/components/libraries/util/app_error_weak.c
  ${SDK_ROOT}/components/libraries/strerror/nrf_strerror.c
  ${SDK_ROOT}/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c
  ${SDK_ROOT}/integration/nrfx/legacy/nrf_drv_twi.c
  ${SDK_ROOT}/modules/nrfx/drivers/src/nrfx_twi.c
  ${SDK_ROOT}/modules/nrfx/drivers/src/nrfx_twim.c
  ${SDK_ROOT}/modules/nrfx/drivers/src/prs/nrfx_prs.c
  ${SDK_ROOT}/components/libraries/bsp/bsp.c
  ${SDK_ROOT}/components/libraries/timer/app_timer.c
  ${SDK_ROOT}/components/libraries/button/app_button.c
  ${SDK_ROOT}/modules/nrfx/drivers/src/nrfx_gpiote.c
  ${SDK_ROOT}/components/libraries/gpiote/app_gpiote.c
  ${SDK_ROOT}/components/ble/common/ble_advdata.c
  ${SDK_ROOT}/integration/nrfx/legacy/nrf_drv_clock.c
  ${SDK_ROOT}/modules/nrfx/drivers/src/nrfx_clock.c
  ${SDK_ROOT}/components/libraries/scheduler/app_scheduler.c
  #${SDK_ROOT}/components/libraries/scheduler/app_scheduler_serconn.c
  ${SDK_ROOT}/components/ble/nrf_ble_qwr/nrf_ble_qwr.c
  ${SDK_ROOT}/components/ble/common/ble_conn_params.c
  ${SDK_ROOT}/components/ble/peer_manager/peer_manager.c
  ${SDK_ROOT}/components/ble/common/ble_conn_state.c
  ${SDK_ROOT}/components/libraries/fds/fds.c
  ${SDK_ROOT}/components/ble/nrf_ble_gatt/nrf_ble_gatt.c
  ${SDK_ROOT}/components/ble/ble_advertising/ble_advertising.c
  ${SDK_ROOT}/components/libraries/atomic_flags/nrf_atflags.c
  ${SDK_ROOT}/components/ble/common/ble_srv_common.c
  ${SDK_ROOT}/components/ble/peer_manager/id_manager.c
  ${SDK_ROOT}/components/ble/peer_manager/security_manager.c
  ${SDK_ROOT}/components/ble/peer_manager/gatt_cache_manager.c
  ${SDK_ROOT}/components/ble/peer_manager/peer_data_storage.c
  ${SDK_ROOT}/components/ble/peer_manager/peer_database.c
  ${SDK_ROOT}/components/ble/peer_manager/security_dispatcher.c
  ${SDK_ROOT}/components/ble/peer_manager/gatts_cache_manager.c
  ${SDK_ROOT}/components/libraries/fstorage/nrf_fstorage.c
  ${SDK_ROOT}/components/libraries/atomic_fifo/nrf_atfifo.c
  ${SDK_ROOT}/components/libraries/fstorage/nrf_fstorage_sd.c
  ${SDK_ROOT}/components/ble/peer_manager/peer_id.c
  ${SDK_ROOT}/components/ble/peer_manager/pm_buffer.c
  ${SDK_ROOT}/modules/nrfx/drivers/src/nrfx_saadc.c
  
  
  ${SDK_ROOT}/components/ble/ble_services/ble_dfu/ble_dfu.c
  ${SDK_ROOT}/components/ble/ble_services/ble_dfu/ble_dfu_bonded.c
  ${SDK_ROOT}/components/ble/ble_services/ble_dfu/ble_dfu_unbonded.c
  ${SDK_ROOT}/components/libraries/bootloader/dfu/nrf_dfu_svci.c
)