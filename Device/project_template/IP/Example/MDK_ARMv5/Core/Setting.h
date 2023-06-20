#ifndef SETTINGS_
#define SETTINGS_

/* Math */
#define PI 3.141592653589793238

/* System */
#define CURRENT_TICK xTaskGetTickCount()

/* Debugging */
static
const char * SUCCESS_COMMAND_SIGN[] = {
  "\r\n\r\n",
  "OK\r\n"
};
static
const char * ERROR_COMMAND_SIGN[] = {
  "ERROR"
};
#define SUCCESS_COMMAND_SIGN_LENGTH sizeof(SUCCESS_COMMAND_SIGN) / sizeof(SUCCESS_COMMAND_SIGN[0])
#define ERROR_COMMAND_SIGN_LENGTH sizeof(ERROR_COMMAND_SIGN) / sizeof(ERROR_COMMAND_SIGN[0])

/* CA */
#define CA_CERT ""
#define CLIENT_CERT ""
#define CLIENT_KEY ""

/* Utilities settings */
#define MAXIMUM_TOKEN_COUNT 15

/* BOARD871 settings */
#define BAUD_RATE_LOG 115200
#define BOARD871_LOG_CONTENT 1700
#define KEY_MAX_LENGTH 30

/* BC660K settings */
#define BC660K_LOG_CONTENT_SIZE 500
#define BC660K_SEND_ATTEMPT_DEFAULT 3
#define BC660K_COMMAND_TIMEOUT_DEFAULT_MS 2000
#define BC660K_COMMAND_SIZE 500
#define BC660K_RECEIVE_BUFFER_SIZE 500
#define BC660K_SEND_COMMAND_DELAY_MS 500

/* LC76F settings */
#define LC76F_LOG_CONTENT_SIZE 300
#define LC76F_GPS_DATA_SIZE 100

/* MC3416 settings */
#define MC3416_LOG_CONTENT_SIZE 300

#define MC3416_ADDR 0x4C
#define MC3416_HTCFG_I2C_MASTER_CLK(CKCUClock) CKCUClock.Bit.I2C0
#define MC3416_HTCFG_I2C_MASTER_SCL_GPIO_ID GPIO_PB
#define MC3416_HTCFG_I2C_MASTER_SCL_AFIO_PIN AFIO_PIN_1
#define MC3416_HTCFG_I2C_MASTER_SDA_GPIO_ID GPIO_PB
#define MC3416_HTCFG_I2C_MASTER_SDA_AFIO_PIN AFIO_PIN_0
#define MC3416_I2C_MASTER_ADDRESS 0
#define MC3416_ClockSpeed 100000
#define MC3416_HTCFG_I2C_MASTER_PORT HT_I2C0

#endif /* SETTINGS_ */