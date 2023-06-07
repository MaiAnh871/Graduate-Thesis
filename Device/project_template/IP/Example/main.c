/********************************************************************************************************/
/*
 * @file    IP/Example/main.c
 * @version $Rev:: 4869         $
 * @date    $Date:: 2020-08-05 #$
 * @brief   Main program.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/
// <<< Use Configuration Wizard in Context Menu >>>

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

#include "ht32_board.h"

#include "stdlib.h"

#include "string.h"

#include <math.h>


/** @addtogroup Project_Template Project Template
 * @{
 */

/** @addtogroup IP_Examples IP
 * @{
 */

/** @addtogroup Example
 * @{
 */

/* Settings ------------------------------------------------------------------------------------------------*/

#define CA_CERT ""
#define CLIENT_CERT ""
#define CLIENT_KEY ""

/* Private types -------------------------------------------------------------------------------------------*/
struct BC660K {
  /* Debug */
  char * log_content;
	
	/* Command */
	vu32 command_timer;
	char *command;
	u16 module_buffer_index;
	char *module_buffer;
} BC660K;

enum StatusType {
		STATUS_SUCCESS = 0,
		STATUS_ERROR,
		STATUS_TIMEOUT,
		STATUS_BAD_PARAMETERS,
		STATUS_UNKNOWN
} StatusType;

/* Private constants ---------------------------------------------------------------------------------------*/
const char *SUCCESS_COMMAND_SIGN[] = { "\r\n\r\n", "OK\r\n" };
const char *ERROR_COMMAND_SIGN[] = { "ERROR" };
#define PI 3.141592653589793238

#define SUCCESS_COMMAND_SIGN_LENGTH sizeof(SUCCESS_COMMAND_SIGN) / sizeof(SUCCESS_COMMAND_SIGN[0])
#define ERROR_COMMAND_SIGN_LENGTH sizeof(ERROR_COMMAND_SIGN) / sizeof(ERROR_COMMAND_SIGN[0])

#define LOG_CONTENT_SIZE 1700
#define SEND_ATTEMPT_DEFAULT 3
#define COMMAND_TIMEOUT_DEFAULT_MS 2000
#define COMMAND_SIZE 1700
#define MODULE_BUFFER_SIZE 100
#define SEND_COMMAND_DELAY_MS 500


/* Private function prototypes -----------------------------------------------------------------------------*/
static void delay_ms(u32 count);
void setup(struct BC660K * self);
void addCA(struct BC660K * self);
void loop(struct BC660K * self);
enum StatusType sendCommand(struct BC660K * self, u8 send_attempt, u32 command_timeout);
void clearModuleBuffer(struct BC660K *self);

/* AT Command functions */
enum StatusType checkModule_AT(struct BC660K *self);
enum StatusType offEcho_ATE0(struct BC660K *self);
enum StatusType getIMEI_AT_CGSN(struct BC660K *self);
enum StatusType getModelID_AT_CGMM(struct BC660K *self);
enum StatusType checkNetworkRegister_AT_CEREG(struct BC660K *self);
enum StatusType getNetworkStatus_AT_QENG(struct BC660K *self);
enum StatusType setAuthentication_AT_QSSLCFG(struct BC660K *self);
enum StatusType setCACert_AT_QSSLCFG(struct BC660K *self);
enum StatusType setClientCert_AT_QSSLCFG(struct BC660K *self);
enum StatusType setClientPrivateKey_AT_QSSLCFG(struct BC660K *self);
enum StatusType enableSSL_AT_QMTCFG(struct BC660K *self);
enum StatusType openMQTT_AT_QMTOPEN(struct BC660K *self);
enum StatusType connectClient_AT_QMTCONN(struct BC660K *self);
enum StatusType publishMessage_AT_QMTPUB(struct BC660K *self);
enum StatusType closeMQTT_AT_QMTCLOSE(struct BC660K *self);
enum StatusType wakeUpModule_AT_QSCLK(struct BC660K *self);

/* ==================== */

/* Debug */
void writeLog(struct BC660K * self);
char *getStatusTypeString(enum StatusType status);

/* UART ports */
void UART0_GNSS_Configuration(void);
void UART0_Receive(void);
void UART0_Read_Block(uint8_t* data);

/* USART0 ports */
void USART0_MODULE_Configuration(void);
void USART0_Send_Char(u16 Data);
void USART0_Send(char * input_string);
enum StatusType USART0_Receive(struct BC660K *self);

/* USART1 ports */
void USART1_DEBUG_Configuration(void);
void USART1_Send_Char(u16 Data);
void USART1_Send(char * input_string);
void USART1_Receive(void);

void LED_Init(void);
void Toggle_LED_1(void);
void Toggle_LED_2(void);
void Toggle_LED_3(void);

/* Private macro -------------------------------------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------------------------------------*/
struct BC660K BC660K_h_h;
vu32 utick;
uint8_t data[100];
uint8_t* check = NULL;
uint8_t GPS_raw[100];
float latitude;
float longitude;
float current_lat;
float current_lon;
int16_t Ax = 0;
int16_t Ay = 0;
int16_t Az = 0;

/* Global functions ----------------------------------------------------------------------------------------*/
void clear(uint8_t *input_string);
bool getRawGPS(void);
bool checkValidGPS(uint8_t *raw_GPS);
void printBool(bool b);
void extractMainData(void);
void USART1_Send_Float(float f);
void updatePosition(void);
float calculateDistance(void);
void packMsg(void);
void USART1_Send_Int16(int16_t value);
/********************************************************************************************************/
/*
 * @brief  Main program.
 * @retval None
 ***********************************************************************************************************/
int main(void) {
//  setup(&BC660K_h_h);
	LED_Init();

  while (1) {
//    loop(&BC660K_h_h);
		Toggle_LED_1();
		Toggle_LED_2();
		delay_ms(100);
  }
}

/********************************************************************************************************/
/*
 * @brief  Main program.
 * @retval None
 ***********************************************************************************************************/
void setup(struct BC660K * self) {
  /* Initialize system tick */
  SysTick_Config(SystemCoreClock / 1000);
	
	/* Initialize LED */
	LED_Init();

  /* Initialize UART ports */
  UART0_GNSS_Configuration();
  USART0_MODULE_Configuration();
  USART1_DEBUG_Configuration();
	
	/* Initialize I2C and Acce */
	I2C_Configuration();
	MC3416_Init();

  /* Initialize BC660K_handler */
  self->log_content = (char * ) malloc(LOG_CONTENT_SIZE * sizeof(char));
  if (!self -> log_content) {
    Toggle_LED_1();
    while (1);
  }
	
  self->command = (char * ) malloc(COMMAND_SIZE * sizeof(char));
  if (!self -> command) {
    Toggle_LED_1();
    while (1);
  }
	
  self->module_buffer = (char * ) malloc(MODULE_BUFFER_SIZE * sizeof(char));
  if (!self -> module_buffer) {
    Toggle_LED_1();
    while (1);
  }

  sprintf(self -> log_content, "Setup successfully!\n");
  writeLog(self);
	
	addCA(self);
}

void addCA(struct BC660K * self) {
	checkModule_AT(self);
	offEcho_ATE0(self);
	setAuthentication_AT_QSSLCFG(self);
	setCACert_AT_QSSLCFG(self);
	setClientCert_AT_QSSLCFG(self);
	setClientPrivateKey_AT_QSSLCFG(self);
	enableSSL_AT_QMTCFG(self);
}

void loop(struct BC660K * self) {
	MC3416_Read_Accel(&Ax, &Ay, &Az);

	
	getRawGPS();
//	USART1_Send((char*) data);
//	printBool(getRawGPS());
	extractMainData();
//	USART1_Send_Float(latitude);
//	USART1_Send_Float(longitude);
//	USART1_Send_Float(calculateDistance());
	
//	printf("Ax = %d, Ay = %d, Az = %d\r\n", Ax, Ay, Az);
//	USART1_Send_Int16(Ax);
//	USART1_Send_Int16(Ay);
//	USART1_Send_Int16(Az);
	


//	UART0_Receive();
		
//		offEcho_ATE0(self);
//		getIMEI_AT_CGSN(self);
//		setAuthentication_AT_QSSLCFG(self);
//		setCACert_AT_QSSLCFG(self);
//		setClientCert_AT_QSSLCFG(self);
//		setClientPrivateKey_AT_QSSLCFG(self);
//		getModelID_AT_CGMM(self);
//		checkNetworkRegister_AT_CEREG(self);
//		getNetworkStatus_AT_QENG(self);
//		checkModule_AT(self);
//		closeMQTT_AT_QMTCLOSE(self);

	wakeUpModule_AT_QSCLK(self);
	openMQTT_AT_QMTOPEN(self);
	connectClient_AT_QMTCONN(self);
	publishMessage_AT_QMTPUB(self);
	publishMessage_AT_QMTPUB(self);
	publishMessage_AT_QMTPUB(self);
	closeMQTT_AT_QMTCLOSE(self);

	delay_ms(1000);
}
	
enum StatusType sendCommand(struct BC660K * self, u8 send_attempt, u32 command_timeout) {
		enum StatusType output_status = STATUS_UNKNOWN;
		if (send_attempt <= 0) {
				send_attempt = SEND_ATTEMPT_DEFAULT;
		}
		u8 count = send_attempt;
		
//		char *command;
//		command = (char * ) malloc(COMMAND_SIZE * sizeof(char));
//		if (!command) {
//			Toggle_LED_1();
//			while (1);
//		}
		
//		strcpy(command, self->command);
		
		while (count--){
				
				sprintf(self->log_content, "\n=== SENDING <%s> | ATTEMPT %u/%u ===\n", self->command, (send_attempt-count), send_attempt);
				writeLog(self);
			
				clearModuleBuffer(self);
				
				
				USART0_Send(self->command);
				USART0_Send((char *)"\r\n");

				self->command_timer = utick;
				while(utick - self->command_timer <= command_timeout) {
						output_status = USART0_Receive(self);
				}
				
				sprintf(self->log_content, "%s\n\n", self->module_buffer);
				writeLog(self);
				clearModuleBuffer(self);
				sprintf(self->log_content, "Command status: %s\n", getStatusTypeString(output_status));
				writeLog(self);
				sprintf(self->log_content, "==========\n");
				writeLog(self);
				
				delay_ms(SEND_COMMAND_DELAY_MS);
				
				if (output_status == STATUS_SUCCESS) {
						break;
				}
		}
		
		return output_status;
}

void clearModuleBuffer(struct BC660K *self) {
		for (self->module_buffer_index = 0; self->module_buffer_index < MODULE_BUFFER_SIZE; self->module_buffer_index++) {
				self->module_buffer[self->module_buffer_index] = 0;
		}
		self->module_buffer_index = 0;
}

enum StatusType checkModule_AT(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT");
		output_status = sendCommand(self, SEND_ATTEMPT_DEFAULT, COMMAND_TIMEOUT_DEFAULT_MS);
	
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType wakeUpModule_AT_QSCLK(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+QSCLK=0");
		output_status = sendCommand(self, SEND_ATTEMPT_DEFAULT, COMMAND_TIMEOUT_DEFAULT_MS);
	
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType offEcho_ATE0(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "ATE0");
		output_status = sendCommand(self, SEND_ATTEMPT_DEFAULT, COMMAND_TIMEOUT_DEFAULT_MS);
	
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType getIMEI_AT_CGSN(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+CGSN");
		output_status = sendCommand(self, SEND_ATTEMPT_DEFAULT, COMMAND_TIMEOUT_DEFAULT_MS);
	
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType getModelID_AT_CGMM(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+CGMM");
		output_status = sendCommand(self, SEND_ATTEMPT_DEFAULT, COMMAND_TIMEOUT_DEFAULT_MS);
	
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType checkNetworkRegister_AT_CEREG(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+CEREG?");
		output_status = sendCommand(self, SEND_ATTEMPT_DEFAULT, COMMAND_TIMEOUT_DEFAULT_MS);
	
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType getNetworkStatus_AT_QENG(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+QENG=0");
		output_status = sendCommand(self, SEND_ATTEMPT_DEFAULT, COMMAND_TIMEOUT_DEFAULT_MS);
	
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType openMQTT_AT_QMTOPEN(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+QMTOPEN=0,\"a2ht7rbdkt6040-ats.iot.ap-northeast-2.amazonaws.com\",8883");
		output_status = sendCommand(self, SEND_ATTEMPT_DEFAULT, COMMAND_TIMEOUT_DEFAULT_MS + 9000);
	
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}
enum StatusType connectClient_AT_QMTCONN(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+QMTCONN=0,\"anhttm8client\"");
		output_status = sendCommand(self, SEND_ATTEMPT_DEFAULT, COMMAND_TIMEOUT_DEFAULT_MS + 8000);
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}

		return output_status;
}

enum StatusType publishMessage_AT_QMTPUB(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		
		/* Write Command */
		sprintf(self->command, "AT+QMTPUB=0,0,0,0,\"tracker/data\",300");
		sprintf(self->log_content, "\n=== SENDING <%s> ===\n", self->command);
		writeLog(self);
		clearModuleBuffer(self);
	
		USART0_Send(self->command);
		USART0_Send((char *)"\r\n");

		self->command_timer = utick;
		while(utick - self->command_timer <= COMMAND_TIMEOUT_DEFAULT_MS) {
				output_status = USART0_Receive(self);
		}
		
		sprintf(self->log_content, "%s", self->module_buffer);
		writeLog(self);
		clearModuleBuffer(self);
		

		sprintf(self->command, "{\"message\":{\"time\":\"15-05-2023 15:11:35\",\"acce_x\":\"%hd\",\"acce_y\":\"%hd\",\"acce_z\":\"%hd\",\"lat\":\"%f\",\"long\":\"%f\"}}", Ax, Ay, Az, latitude, longitude);
		USART0_Send(self->command);
		USART0_Send((char *)"\r\n");
	
		self->command_timer = utick;
		while(utick - self->command_timer <= (COMMAND_TIMEOUT_DEFAULT_MS + 2000)) {
				output_status = USART0_Receive(self);
		}
		
		sprintf(self->log_content, "%s\n\n", self->module_buffer);
		writeLog(self);
		clearModuleBuffer(self);
		sprintf(self->log_content, "Command status: %s\n", getStatusTypeString(output_status));
		writeLog(self);
		sprintf(self->log_content, "==========\n");
		writeLog(self);
		
		delay_ms(SEND_COMMAND_DELAY_MS);
		
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType closeMQTT_AT_QMTCLOSE(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+QMTCLOSE=0");
		output_status = sendCommand(self, 2, COMMAND_TIMEOUT_DEFAULT_MS);
	
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType setAuthentication_AT_QSSLCFG(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+QSSLCFG=0,0,\"seclevel\",2");
		output_status = sendCommand(self, SEND_ATTEMPT_DEFAULT, COMMAND_TIMEOUT_DEFAULT_MS);
	
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType setCACert_AT_QSSLCFG(struct BC660K *self)  {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+QSSLCFG=0,0,\"cacert\"");
		sprintf(self->log_content, "\n=== SENDING <%s> ===\n", self->command);
		writeLog(self);
		clearModuleBuffer(self);
	
		USART0_Send(self->command);
		USART0_Send((char *)"\r\n");

		self->command_timer = utick;
		while(utick - self->command_timer <= COMMAND_TIMEOUT_DEFAULT_MS) {
				output_status = USART0_Receive(self);
		}
		
		sprintf(self->log_content, "|%s|", self->module_buffer);
		writeLog(self);
		clearModuleBuffer(self);
		
		sprintf(self->command, CA_CERT);
		USART0_Send(self->command);
		USART0_Send((char *)"\r\n");
		delay_ms(100);
		USART0_Send_Char(26);
	
		self->command_timer = utick;
		while(utick - self->command_timer <= (COMMAND_TIMEOUT_DEFAULT_MS + 2000)) {
				output_status = USART0_Receive(self);
		}
		
		sprintf(self->log_content, "%s\n\n", self->module_buffer);
		writeLog(self);
		clearModuleBuffer(self);
		sprintf(self->log_content, "Command status: %s\n", getStatusTypeString(output_status));
		writeLog(self);
		sprintf(self->log_content, "==========\n");
		writeLog(self);
		
		delay_ms(SEND_COMMAND_DELAY_MS);
		
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType setClientCert_AT_QSSLCFG(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+QSSLCFG=0,0,\"clientcert\"");
		sprintf(self->log_content, "\n=== SENDING <%s> ===\n", self->command);
		writeLog(self);
		clearModuleBuffer(self);
	
		USART0_Send(self->command);
		USART0_Send((char *)"\r\n");

		self->command_timer = utick;
		while(utick - self->command_timer <= COMMAND_TIMEOUT_DEFAULT_MS) {
				output_status = USART0_Receive(self);
		}
		
		sprintf(self->log_content, "%s", self->module_buffer);
		writeLog(self);
		clearModuleBuffer(self);
		
		sprintf(self->command, CLIENT_CERT);
		USART0_Send(self->command);
		USART0_Send((char *)"\r\n");
		delay_ms(100);
		USART0_Send_Char(26);
	
		self->command_timer = utick;
		while(utick - self->command_timer <= (COMMAND_TIMEOUT_DEFAULT_MS + 2000)) {
				output_status = USART0_Receive(self);
		}
		
		sprintf(self->log_content, "%s\n\n", self->module_buffer);
		writeLog(self);
		clearModuleBuffer(self);
		sprintf(self->log_content, "Command status: %s\n", getStatusTypeString(output_status));
		writeLog(self);
		sprintf(self->log_content, "==========\n");
		writeLog(self);
		
		delay_ms(SEND_COMMAND_DELAY_MS);
		
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType setClientPrivateKey_AT_QSSLCFG(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+QSSLCFG=0,0,\"clientkey\"");
		sprintf(self->log_content, "\n=== SENDING <%s> ===\n", self->command);
		writeLog(self);
		clearModuleBuffer(self);
	
		USART0_Send(self->command);
		USART0_Send((char *)"\r\n");

		self->command_timer = utick;
		while(utick - self->command_timer <= COMMAND_TIMEOUT_DEFAULT_MS) {
				output_status = USART0_Receive(self);
		}
		
		sprintf(self->log_content, "%s", self->module_buffer);
		writeLog(self);
		clearModuleBuffer(self);
		
		sprintf(self->command, CLIENT_KEY);
		USART0_Send(self->command);
		USART0_Send((char *)"\r\n");
		delay_ms(100);
		USART0_Send_Char(26);
	
		self->command_timer = utick;
		while(utick - self->command_timer <= (COMMAND_TIMEOUT_DEFAULT_MS + 2000)) {
				output_status = USART0_Receive(self);
		}
		
		sprintf(self->log_content, "%s\n\n", self->module_buffer);
		writeLog(self);
		clearModuleBuffer(self);
		sprintf(self->log_content, "Command status: %s\n", getStatusTypeString(output_status));
		writeLog(self);
		sprintf(self->log_content, "==========\n");
		writeLog(self);
		
		delay_ms(SEND_COMMAND_DELAY_MS);
		
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}

enum StatusType enableSSL_AT_QMTCFG(struct BC660K *self) {
		/* Initialize status */
		enum StatusType output_status = STATUS_UNKNOWN;
		
		/* Write Command */
		sprintf(self->command, "AT+QMTCFG=\"ssl\",0,1,0,0");
		output_status = sendCommand(self, SEND_ATTEMPT_DEFAULT, COMMAND_TIMEOUT_DEFAULT_MS);
	
		/* Actions with status */
		switch(output_status){
			
			case STATUS_SUCCESS:
					/* Do something */
					break;

			case STATUS_ERROR:
					/* Do something */
					break;
			
			case STATUS_TIMEOUT:
					/* Do something */
					break;
			
			case STATUS_BAD_PARAMETERS:
					/* Do something */
					break;
			
			default:
					/* Do something */
					break;
		}
		
		return output_status;
}


/* Debug */
void writeLog(struct BC660K * self) {
  USART1_Send(self -> log_content);
}

char *getStatusTypeString(enum StatusType status) {
		switch(status) {
			case STATUS_SUCCESS:
					return "SUCCESS";
			case STATUS_ERROR:
					return "ERROR";
			case STATUS_TIMEOUT:
					return "TIMEOUT";
			case STATUS_BAD_PARAMETERS:
					return "PARAMETERS";
			case STATUS_UNKNOWN:
					return "UNKNOWN";
			default:
					return "UNSUPPORTED STATUS";
		}
}

/*************************************************************************************************************
 * @brief  Configure the UART0 for GNSS
 * @retval None
 ***********************************************************************************************************/
void UART0_GNSS_Configuration(void) {
  CKCU_PeripClockConfig_TypeDef CKCUClock; // Set all the fields to zero, which means that no peripheral clocks are enabled by default.

  {
    /* Enable peripheral clock of AFIO, UxART                                                                 */
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PB = 1;
    CKCUClock.Bit.UART0 = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  /* Turn on UxART Rx internal pull up resistor to prevent unknow state                                     */
  GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_8, GPIO_PR_UP);

  /* Config AFIO mode as UxART function.                                                                    */
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_7, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(GPIO_PB, AFIO_PIN_8, AFIO_FUN_USART_UART);

  {
    /* UxART configured as follow:
          - BaudRate = 115200 baud
          - Word Length = 8 Bits
          - One Stop Bit
          - None parity bit
    */

    /* !!! NOTICE !!!
       Notice that the local variable (structure) did not have an initial value.
       Please confirm that there are no missing members in the parameter settings below in this function.
    */
    USART_InitTypeDef USART_InitStructure = {
      0
    };
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
    USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_NO;
    USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
    USART_Init(HT_UART0, & USART_InitStructure);
  }

  /* Enable UxART Tx and Rx function                                                                        */
  USART_TxCmd(HT_UART0, ENABLE);
  USART_RxCmd(HT_UART0, ENABLE);
}

/*************************************************************************************************************
 * @brief  Configure the USART0
 * @retval None
 ***********************************************************************************************************/
void USART0_MODULE_Configuration(void) {
  CKCU_PeripClockConfig_TypeDef CKCUClock; // Set all the fields to zero, which means that no peripheral clocks are enabled by default.

  {
    /* Enable peripheral clock of AFIO, UxART                                                                 */
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PA = 1;
    CKCUClock.Bit.USART0 = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  /* Turn on UxART Rx internal pull up resistor to prevent unknow state                                     */
  GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_3, GPIO_PR_UP);

  /* Config AFIO mode as UxART function.                                                                    */
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_3, AFIO_FUN_USART_UART);

  {
    /* UxART configured as follow:
          - BaudRate = 115200 baud
          - Word Length = 8 Bits
          - One Stop Bit
          - None parity bit
    */

    /* !!! NOTICE !!!
       Notice that the local variable (structure) did not have an initial value.
       Please confirm that there are no missing members in the parameter settings below in this function.
    */
    USART_InitTypeDef USART_InitStructure = {
      0
    };
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
    USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_NO;
    USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
    USART_Init(HT_USART0, & USART_InitStructure);
  }

  /* Enable UxART Tx and Rx function                                                                        */
  USART_TxCmd(HT_USART0, ENABLE);
  USART_RxCmd(HT_USART0, ENABLE);
}

/********************************************************************************************************/
/*
 * @brief  Configure the USART1
 * @retval None
 ***********************************************************************************************************/
void USART1_DEBUG_Configuration(void) {
  CKCU_PeripClockConfig_TypeDef CKCUClock; // Set all the fields to zero, which means that no peripheral clocks are enabled by default.

  {
    /* Enable peripheral clock of AFIO, UxART                                                                 */
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PA = 1;
    CKCUClock.Bit.USART1 = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  /* Turn on UxART Rx internal pull up resistor to prevent unknow state                                     */
  GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_5, GPIO_PR_UP);

  /* Config AFIO mode as UxART function.                                                                    */
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_4, AFIO_FUN_USART_UART);
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_USART_UART);

  {
    /* UxART configured as follow:
          - BaudRate = 115200 baud
          - Word Length = 8 Bits
          - One Stop Bit
          - None parity bit
    */

    /* !!! NOTICE !!!
       Notice that the local variable (structure) did not have an initial value.
       Please confirm that there are no missing members in the parameter settings below in this function.
    */
    USART_InitTypeDef USART_InitStructure = {
      0
    };
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
    USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
    USART_InitStructure.USART_Parity = USART_PARITY_NO;
    USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
    USART_Init(HT_USART1, & USART_InitStructure);
  }

  /* Enable UxART Tx and Rx function                                                                        */
  USART_TxCmd(HT_USART1, ENABLE);
  USART_RxCmd(HT_USART1, ENABLE);
}

/********************************************************************************************************/
/*
 * @brief  UxART0 Tx Send Byte.
 * @param  Data: the data to be transmitted.
 * @retval None
 ***********************************************************************************************************/
void USART0_Send_Char(u16 Data) {
  while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET);
  USART_SendData(HT_USART0, Data);
}

/********************************************************************************************************/
/*
 * @brief  UxART Tx Send Byte.
 * @param  Data: the data to be transmitted.
 * @retval None
 ***********************************************************************************************************/
void USART1_Send_Char(u16 Data) {
  while (USART_GetFlagStatus(HT_USART1, USART_FLAG_TXC) == RESET);
  USART_SendData(HT_USART1, Data);
}

/********************************************************************************************************/
/*
 * @brief  UxART Tx Test.
 * @retval None
 ***********************************************************************************************************/
void USART0_Send(char * input_string) {
  int i;
  /* Send a buffer from UxART to terminal                                                                   */
  for (i = 0; i < strlen(input_string); i++) {
    USART0_Send_Char(input_string[i]);
  }

  /* Send to USART1 what sent to USART0 */
  USART1_Send(input_string);
}

/********************************************************************************************************/
/*
 * @brief  UxART Tx Test.
 * @retval None
 ***********************************************************************************************************/
void USART1_Send(char * input_string) {
  int i;
  /* Send a buffer from UxART to terminal                                                                   */
  for (i = 0; i < strlen(input_string); i++) {
    USART1_Send_Char(input_string[i]);
  }
}

void UART0_Receive(void) {
  u16 uData;

  /* Waits until the Rx FIFO/DR is not empty then get data from them                                        */
  if (USART_GetFlagStatus(HT_UART0, USART_FLAG_RXDR) == SET) {
    uData = USART_ReceiveData(HT_UART0);

    #if 1 // Loop back Rx data to Tx for test
    USART1_Send_Char(uData);
    #endif
  }
}

void UART0_Read_Block(uint8_t  *data)
{
	uint8_t index = 0;
	
	do
	{
  /* Waits until the Rx FIFO/DR is not empty then get data from them                                        */
  while (USART_GetFlagStatus(HT_UART0, USART_FLAG_RXDR) == RESET);
	data[index] = (uint8_t)USART_ReceiveData(HT_UART0);
	}
	while ((data[index] != 0x0A) && (index++ != 99));
}

void clear(uint8_t *input_string)
{
	uint16_t count = 0;
	for (count = 0; count < 100; count++)
	{
		input_string[count] = 0;
	}
}

bool getRawGPS(void)
{
	extern uint8_t data[100];
	uint8_t* check = NULL;
	uint8_t GPS_raw[100];
	
	while (check == NULL)
	{
		clear(GPS_raw);
		UART0_Read_Block(GPS_raw);
		check = strstr(GPS_raw, "$GNRMC");
	}
	strcpy(data, GPS_raw);
	return checkValidGPS(GPS_raw);
}

bool checkValidGPS(uint8_t *raw_GPS)
{
	bool valid = false;
	uint8_t* check = NULL;
	
	check = strtok(GPS_raw, ",");
	check = strtok(NULL, ",");
	check = strtok(NULL, ",");
	
	if (strcmp(check, "A") == 0)
	{
		valid = true;
	}
	
	return valid;
}

void printBool(bool b) 
{
  char buf[6]; // enough to hold "false" or "true"
  sprintf(buf, "%s\r\n", b ? "Valid\r\n" : "Invalid\r\n");
  USART1_Send(buf);
}

void extractMainData(void)
{
	extern uint8_t data[100];
	extern float latitude;
	extern float longitude;
	uint8_t* check = NULL;
	int b = 0;
	float a = 0.0;
	float c = 0.0;
	
	check = strtok(data, ",");
	check = strtok(NULL, ",");
	check = strtok(NULL, ",");
	
	check = strtok(NULL, ",");
	a = atof(check);
	b = a/100;
	c  = a-b*100;
	latitude = (b + (c/60));
	check = strtok(NULL, ",");
	if (strcmp(check, "S") == 0)
	{
		latitude = latitude*(-1);
	}
	
	check = strtok(NULL, ",");
	a = atof(check);
	b = a/100;
	c  = a-b*100;
	longitude = (b + (c/60));
	check = strtok(NULL, ",");
	if (strcmp(check, "S") == 0)
	{
		longitude = longitude*(-1);
	}
}

void USART1_Send_Float(float f) {
  char buffer[16]; // adjust buffer size as needed
  sprintf(buffer, "%.6f\r\n", f); // convert float to string with 6 decimal places
  USART1_Send(buffer); // send string over USART1
}

void updatePosition(void)
{
	extern float latitude, current_lat;
	extern float longitude, current_lon;
	current_lat = latitude;
	current_lon = longitude;
}

float calculateDistance(void)
{
	extern float latitude, current_lat;
	extern float longitude, current_lon;
	double theta1, theta2, delta_lat, delta_lon;
	double a,c,d;
	
	theta1 = current_lat * PI / 180;
	theta2 = latitude * PI / 180;
	
	delta_lat = (latitude - current_lat) * PI / 360;
	delta_lon = (longitude - current_lon) * PI / 360;
	
	a = sin(delta_lat)*sin(delta_lat) + cos(theta1)*cos(theta2)*sin(delta_lon)*sin(delta_lon);
	c = asin(sqrt(a));
	d = 2*6378137*c;
	
	return d;
}

void packMsg()
{
	char buffer[120]; // adjust buffer size as needed
  sprintf(buffer, "{\"message\":{\"time\":\"15-05-2023 15:11:35\",\"acce_x\":\"%hd\",\"acce_y\":\"%hd\",\"acce_z\":\"%hd\",\"lat\":\"%f\",\"long\":\"%f\"}}\r\n", Ax, Ay, Az, latitude, longitude); // convert float to string with 6 decimal places
  USART1_Send(buffer); // send string over USART1
}

enum StatusType USART0_Receive(struct BC660K *self) {
		enum StatusType output_status = STATUS_TIMEOUT;
		u16 uData;
		u8 index;
		char *ptr;

		/* Waits until the Rx FIFO/DR is not empty then get data from them                                        */
		if (USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR) == SET) {
			uData = USART_ReceiveData(HT_USART0);
			self->module_buffer[self->module_buffer_index] = uData;
			self->module_buffer_index++;
		}

		for (index = 0; index < SUCCESS_COMMAND_SIGN_LENGTH; index++){
				ptr = strstr(self->module_buffer, SUCCESS_COMMAND_SIGN[index]);
				if (ptr) {
						output_status = STATUS_SUCCESS;
						return output_status;
				}
		}
		
		for (index = 0; index < ERROR_COMMAND_SIGN_LENGTH; index++){
				ptr = strstr(self->module_buffer, ERROR_COMMAND_SIGN[index]);
				if (ptr) {
						output_status = STATUS_ERROR;
						return output_status;
				}
		}
		
		return output_status;
}

void USART1_Send_Int16(int16_t value) {
  char buffer[6]; // adjust buffer size as needed
  sprintf(buffer, "%d\r\n", value); // convert uint16_t to string
  USART1_Send(buffer); // send string over USART1
}

void USART1_Receive(void) {
  u16 uData;

  /* Waits until the Rx FIFO/DR is not empty then get data from them                                        */
  if (USART_GetFlagStatus(HT_USART1, USART_FLAG_RXDR) == SET) {
    uData = USART_ReceiveData(HT_USART1);

    #if 1 // Loop back Rx data to Tx for test
    USART1_Send_Char(uData);
    #endif
  }
}

void LED_Init() {
  HT32F_DVB_LEDInit(HT_LED1);
  HT32F_DVB_LEDInit(HT_LED2);
  HT32F_DVB_LEDInit(HT_LED3);
  HT32F_DVB_LEDOff(HT_LED1);
  HT32F_DVB_LEDOff(HT_LED2);
  HT32F_DVB_LEDOff(HT_LED3);
}

void Toggle_LED_1() {
    HT32F_DVB_LEDToggle(HT_LED1);
}

void Toggle_LED_2() {
    HT32F_DVB_LEDToggle(HT_LED2);
}

void Toggle_LED_3() {
    HT32F_DVB_LEDToggle(HT_LED3);
}

#if(HT32_LIB_DEBUG == 1)
/********************************************************************************************************/
/*
 * @brief  Report both the error name of the source file and the source line number.
 * @param  filename: pointer to the source file name.
 * @param  uline: error line source number.
 * @retval None
 ***********************************************************************************************************/
void assert_error(u8 * filename, u32 uline) {
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
     Example: printf("Parameter Error: file %s on line %d\r\n", filename, uline);
  */

  while (1) {}
}
#endif

/* Private functions ---------------------------------------------------------------------------------------*/
/********************************************************************************************************/
/*
 * @brief  delay function
 * @param  count: delay count for loop
 * @retval None
 ***********************************************************************************************************/
static void delay_ms(u32 ms) {
  uint32_t i, j;
  for (i = 0; i < ms; i++) {
    for (j = 0; j < 33132; j++) {
      __NOP();
    }
  }
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
