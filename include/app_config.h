//This are defines for sdk

//Bluetooth magic
#define PEER_MANAGER_ENABLED 1
#define NRF_BLE_QWR_ENABLED 1
#define FDS_ENABLED 1
#define NRF_BLE_GATT_ENABLED 1
#define BLE_ADVERTISING_ENABLED 1
#define NRF_BLE_CONN_PARAMS_ENABLED 1
#define NRF_SDH_BLE_PERIPHERAL_LINK_COUNT 1
#define NRF_BLE_CONN_PARAMS_MAX_SLAVE_LATENCY_DEVIATION 499
#define NRF_BLE_CONN_PARAMS_MAX_SUPERVISION_TIMEOUT_DEVIATION 65535
#define NRF_FSTORAGE_ENABLED 1
#define NRF_FSTORAGE_PARAM_CHECK_DISABLED 0
#define NRF_SDH_BLE_VS_UUID_COUNT 5

#define GPIO_COUNT 2
//BSP support
#define BUTTON_ENABLED 1
#define APP_GPIOTE_ENABLED 1
#define NRFX_GPIOTE_ENABLED 1
#define GPIOTE_ENABLED 1
#define GPIOTE_CONFIG_NUM_OF_LOW_POWER_EVENTS 4

#define APP_FIFO_ENABLED 1
#define APP_SCHEDULER_ENABLED 1

//Timer support
#define APP_TIMER_ENABLED 1
#define APP_TIMER_CONFIG_USE_SCHEDULER 1  //we want timer to use app scheduler
#define NRF_CLOCK_ENABLED 1
#define CLOCK_CONFIG_LF_SRC 1

//Logs
#define NRF_LOG_ENABLED 1
#define NRF_LOG_USES_TIMESTAMP 0
#define NRF_LOG_BACKEND_UART_ENABLED 0
#define NRF_LOG_BACKEND_RTT_ENABLED 1
#define NRF_LOG_DEFERRED 0

//SEGGER RTT
#define NRF_LOG_BACKEND_RTT_TEMP_BUFFER_SIZE 64
#define NRF_LOG_BACKEND_RTT_TX_RETRY_CNT 3
#define NRF_LOG_BACKEND_RTT_TX_RETRY_DELAY_MS 2

#define SEGGER_RTT_CONFIG_BUFFER_SIZE_UP 512
#define SEGGER_RTT_CONFIG_MAX_NUM_UP_BUFFERS 2
#define SEGGER_RTT_CONFIG_BUFFER_SIZE_DOWN 16
#define SEGGER_RTT_CONFIG_MAX_NUM_DOWN_BUFFERS 2
#define SEGGER_RTT_CONFIG_DEFAULT_MODE 0

//SoftDevice
#define NRF_SDH_BLE_ENABLED 1
#define NRF_SDH_ENABLED 1
#define NRF_SDH_SOC_ENABLED 1
#define NRF_SDH_DISPATCH_MODEL NRF_SDH_DISPATCH_MODEL_APPSH

//Power management
#define NRF_PWR_MGMT_ENABLED 1

//App Scheduler
#define APP_SCHEDULER_ENABLED 1
#define APP_SCHEDULER_WITH_PAUSE 0
#define APP_SCHEDULER_WITH_PROFILER 0

//TWI
#define TWI_ENABLED 1
#define TWI0_ENABLED 1
#define TWI0_USE_EASY_DMA 1

//#define NRFX_TWIM_CONFIG_LOG_ENABLED 1
//#define TWI_CONFIG_LOG_ENABLED 1
//#define TWI_CONFIG_LOG_LEVEL 4
