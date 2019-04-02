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
)

