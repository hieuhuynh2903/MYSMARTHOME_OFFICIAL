/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "math.h"

#include "dht11.h"
#include "lcd_i2c.h"
#include "music.h"
#include "servo.h"
#include "hall.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ON GPIO_PIN_SET
#define OFF GPIO_PIN_RESET
#define CLOSE 90
#define OPEN 0

#define BATHROOM_LIGHT_PORT			GPIOC
#define BATHROOM_LIGHT_PIN 			GPIO_PIN_4

#define KITCHEN_LIGHT_PORT			GPIOA
#define KITCHEN_LIGHT_PIN 			GPIO_PIN_0

#define KITCHEN_FAN_PORT				GPIOC
#define KITCHEN_FAN_PIN 				GPIO_PIN_0

#define KITCHEN_WINDOW_PIN 			1

#define BEDROOM_LIGHT_PORT			GPIOA
#define BEDROOM_LIGHT_PIN 			GPIO_PIN_6

#define BEDROOM_FAN_PORT				GPIOC
#define BEDROOM_FAN_PIN 				GPIO_PIN_2

#define BEDROOM_WINDOW_PIN 			2

#define LIVINGROOM_LIGHT_PORT		GPIOA
#define LIVINGROOM_LIGHT_PIN 		GPIO_PIN_2

#define LIVINGROOM_GARAGELIGHT_PORT		GPIOA
#define LIVINGROOM_GARAGELIGHT_PIN 		GPIO_PIN_4

#define BELL_PORT GPIOB
#define BELL_PIN 	GPIO_PIN_15

#define DRYINGPOLE_PIN		4
#define GATE_PIN					3


#define GAS_THRESH 10

#define txBuf_size 30
#define rxBuf_size 50

#define R1_PORT GPIOB
#define R1_PIN 	GPIO_PIN_5

#define R2_PORT GPIOB
#define R2_PIN 	GPIO_PIN_3

#define R3_PORT GPIOD
#define R3_PIN 	GPIO_PIN_6

#define R4_PORT GPIOD
#define R4_PIN 	GPIO_PIN_4

#define C1_PORT GPIOD
#define C1_PIN 	GPIO_PIN_2

#define C2_PORT GPIOD
#define C2_PIN 	GPIO_PIN_0

#define C3_PORT GPIOC
#define C3_PIN 	GPIO_PIN_11

#define C4_PORT GPIOA
#define C4_PIN 	GPIO_PIN_15


char rxBuf[rxBuf_size];


char rxBuf_receive[2];
uint16_t rx_Index = 0;
char temp_receive[rxBuf_size]; 

char data_GAS_char[12];
float gas_in_ppm = 0;

char data_PIR_char[6] ;
char pir_char[6];
bool pir_bool_old = 0;

char data_RAIN_char[7];
char rain_char[7];
bool rain_bool_old = 0;

char data_FLAME_char[8];
char flame_char[8];
bool flame_bool_old = 0;

unsigned char data_dht11[4];
char data_dht11_char[15];

char debug_char[10];


char function_key;
char password_setup[8] = "12345678";
char password_input[8];
char password_new[8];
char password_new_confirm[8];
char password_update_firebase[18];
char password_receive[8];
void change_password();
int i = 0;

uint32_t current_value_main = 0;
char data_HALL_char[20];

long timeout_to_send_gasvalue		= 0;
long timeout_to_send_dht11value = 0;
long timeout_to_send_flamevalue = 0;
long timeout_to_send_rainvalue 	= 0;
long timeout_to_send_pirvalue 	= 0;
long timeout_to_send_hallvalue 	= 0;
long timeout_to_read_keypad 		= 0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

I2C_HandleTypeDef hi2c2;

IWDG_HandleTypeDef hiwdg;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim9;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityLow1,
  .stack_size = 256 * 4
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .priority = (osPriority_t) osPriorityLow2,
  .stack_size = 128 * 4
};
/* Definitions for myTask03 */
osThreadId_t myTask03Handle;
const osThreadAttr_t myTask03_attributes = {
  .name = "myTask03",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 256 * 4
};
/* Definitions for myTask04 */
osThreadId_t myTask04Handle;
const osThreadAttr_t myTask04_attributes = {
  .name = "myTask04",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for myTask05 */
osThreadId_t myTask05Handle;
const osThreadAttr_t myTask05_attributes = {
  .name = "myTask05",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for myTask06 */
osThreadId_t myTask06Handle;
const osThreadAttr_t myTask06_attributes = {
  .name = "myTask06",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM9_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_IWDG_Init(void);
void StartDefaultTask(void *argument);
void StartTask02Function(void *argument);
void StartTask03Function(void *argument);
void StartTask04Function(void *argument);
void StartTask05Function(void *argument);
void StartTask06Function(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void serial_send_cmd(char data[]){
		char line[txBuf_size];
		snprintf(line, sizeof(line), "%s\r\n", data);
		HAL_UART_Transmit(&huart1, (uint8_t*)line, strlen(line) , 100);
}

char* serial_read(){
	strcpy(temp_receive, rxBuf);
	return temp_receive;
}

void control_light	(char room[], bool state){
	if(strcmp(room,"Bathroom") == 0){
		if(state == ON){
			HAL_GPIO_WritePin(BATHROOM_LIGHT_PORT, BATHROOM_LIGHT_PIN, ON);
		}else if(state == OFF){
			HAL_GPIO_WritePin(BATHROOM_LIGHT_PORT, BATHROOM_LIGHT_PIN, OFF);
		}
	}
	if(strcmp(room,"Kitchen") == 0){
		if(state == ON){
			HAL_GPIO_WritePin(KITCHEN_LIGHT_PORT, KITCHEN_LIGHT_PIN, ON);
		}else if(state == OFF){
			HAL_GPIO_WritePin(KITCHEN_LIGHT_PORT, KITCHEN_LIGHT_PIN, OFF);
		}	
	}
	if(strcmp(room,"Bedroom") == 0){
		if(state == ON){
			HAL_GPIO_WritePin(BEDROOM_LIGHT_PORT, BEDROOM_LIGHT_PIN, ON);
		}else if(state == OFF){
			HAL_GPIO_WritePin(BEDROOM_LIGHT_PORT, BEDROOM_LIGHT_PIN, OFF);
		}	
	}
	if(strcmp(room,"Livingroom") == 0){
		if(state == ON){
			HAL_GPIO_WritePin(LIVINGROOM_LIGHT_PORT, LIVINGROOM_LIGHT_PIN, ON);
		}else if(state == OFF){
			HAL_GPIO_WritePin(LIVINGROOM_LIGHT_PORT, LIVINGROOM_LIGHT_PIN, OFF);
		}	
	}	
	if(strcmp(room,"Garage") == 0){
		if(state == ON){
			HAL_GPIO_WritePin(LIVINGROOM_GARAGELIGHT_PORT, LIVINGROOM_GARAGELIGHT_PIN, ON);
		}else if(state == OFF){
			HAL_GPIO_WritePin(LIVINGROOM_GARAGELIGHT_PORT, LIVINGROOM_GARAGELIGHT_PIN, OFF);
		}	
	}	
}

void control_fan		(char room[], bool state){
	if(strcmp(room,"Bedroom") == 0){
		if(state == ON){
			HAL_GPIO_WritePin(BEDROOM_FAN_PORT, BEDROOM_FAN_PIN, ON);
		}else if(state == OFF){
			HAL_GPIO_WritePin(BEDROOM_FAN_PORT, BEDROOM_FAN_PIN, OFF);
		}
	}
	else if(strcmp(room,"Kitchen") == 0){
		if(state == ON){
			HAL_GPIO_WritePin(KITCHEN_FAN_PORT, KITCHEN_FAN_PIN, ON);
		}else if(state == OFF){
			HAL_GPIO_WritePin(KITCHEN_FAN_PORT, KITCHEN_FAN_PIN, OFF);
		}	
	}
}

void control_window	(char room[], bool state){
	if(strcmp(room,"Bedroom") == 0){
		if(state == ON){
			servo_position(BEDROOM_WINDOW_PIN, OPEN);
			HAL_Delay(500);
		}else if(state == OFF){
			servo_position(BEDROOM_WINDOW_PIN, CLOSE);
			HAL_Delay(500);
		}
	}
	else if(strcmp(room,"Kitchen") == 0){
		if(state == ON){
			servo_position(KITCHEN_WINDOW_PIN, OPEN);
		}else if(state == OFF){
			servo_position(KITCHEN_WINDOW_PIN, CLOSE);
		}
	}	
}

void control_homescreen(char device[], bool state){
	if(strcmp(device,"bell") == 0){
		if(state == ON){
			music_play(500);
		}else if(state == OFF){
			music_stop();
		}
	}
	if(strcmp(device,"dryingpole") == 0){
		if(state == ON){
			servo_position(DRYINGPOLE_PIN, 90);
		}else if(state == OFF){
			servo_position(DRYINGPOLE_PIN, 0);
		}
	}
	if(strcmp(device,"gate") == 0){
		if(state == ON){
			servo_position(GATE_PIN, OPEN);
		}else if(state == OFF){
			servo_position(GATE_PIN, CLOSE);
		}
	}
	if(strcmp(device,"smartlight") == 0){
		if(state == ON){
			control_light("Livingroom",ON);
		}else if(state == OFF){
			control_light("Livingroom",OFF);
		}
	}

}

void control_home(){
	char temp[2];
	serial_read();

	if(strncmp(temp_receive,"Bathroom/light",14) == 0){
		temp[0] = temp_receive[15];
		if(temp[0] == 'T'){
			control_light("Bathroom",ON);
		}else if(temp[0] == 'F'){
			control_light("Bathroom", OFF);	
		}
	}
	if(strncmp(temp_receive,"Bedroom/light",13) == 0){
		temp[0] = temp_receive[14];
		if(temp[0] == 'T'){
			control_light("Bedroom",ON);
		}else if(temp[0] == 'F'){
			control_light("Bedroom", OFF);	
		}
	}
	if(strncmp(temp_receive,"Bedroom/window",14) == 0){
		temp[0] = temp_receive[15];
		if(temp[0] == 'T'){
			control_window("Bedroom",ON);
		}else if(temp[0] == 'F'){
			control_window("Bedroom",OFF);
		}
	}
	if(strncmp(temp_receive,"Bedroom/fan",11) == 0){
		temp[0] = temp_receive[12];
		if(temp[0] == 'T'){
			control_fan("Bedroom",ON);
		}else if(temp[0] == 'F'){
			control_fan("Bedroom",OFF);
		}
	}
	if(strncmp(temp_receive,"Homescreen/bell",15) == 0){
		temp[0] = temp_receive[16];
		if(temp[0] == 'T'){
			control_homescreen("bell",ON);
		}else if(temp[0] == 'F'){
			control_homescreen("bell",OFF);
		}
	}
	if(strncmp(temp_receive,"Homescreen/dryingpole",21) == 0){
		temp[0] = temp_receive[22];
		if(temp[0] == 'T'){
			control_homescreen("dryingpole",ON);
			strcpy(rxBuf,"");
		}else if(temp[0] == 'F'){
			control_homescreen("dryingpole",OFF);
			strcpy(rxBuf,"");
		}
	}
	if(strncmp(temp_receive,"Homescreen/gate",15) == 0){
		temp[0] = temp_receive[16];
		if(temp[0] == 'T'){
			control_homescreen("gate",ON);
		}else if(temp[0] == 'F'){
			control_homescreen("gate",OFF);
		}
	}
	if(strncmp(temp_receive,"Homescreen/smartlight",21) == 0){
		temp[0] = temp_receive[22];
		if(temp[0] == 'T'){
			control_homescreen("smartlight",ON);
			
			strcpy(rxBuf,"");
		}else if(temp[0] == 'F'){
			control_homescreen("smartlight",OFF);
			
			strcpy(rxBuf,"");
		}
	}
	if(strncmp(temp_receive,"Kitchen/fan",11) == 0){
		temp[0] = temp_receive[12];
		if(temp[0] == 'T'){
			control_fan("Kitchen",ON);
		}else if(temp[0] == 'F'){
			control_fan("Kitchen",OFF);
		}
	}
	if(strncmp(temp_receive,"Kitchen/light",13) == 0){
		temp[0] = temp_receive[14];
		if(temp[0] == 'T'){
			control_light("Kitchen",ON);
		}else if(temp[0] == 'F'){
			control_light("Kitchen",OFF);
		}
	}
	if(strncmp(temp_receive,"Kitchen/window",14) == 0){
		temp[0] = temp_receive[15];
		if(temp[0] == 'T'){
			control_window("Kitchen",ON);
		}else if(temp[0] == 'F'){
			control_window("Kitchen",OFF);
		}
	}
	if(strncmp(temp_receive,"Livingroom/garagelight",22) == 0){
		temp[0] = temp_receive[23];
		if(temp[0] == 'T'){
			control_light("Garage",ON);
		}else if(temp[0] == 'F'){
			control_light("Garage",OFF);
		}
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){
  /* Prevent unused argument(s) compilation warning */
	if(UartHandle->Instance==USART6){
		if (rx_Index == 0){
			for(int i = 0; i < rxBuf_size; i++){
				rxBuf[i] = 0;
			}
			//strcpy(rxBuf,"");
		}
		if (rxBuf_receive[0] != '\n'){
			rxBuf[rx_Index]=rxBuf_receive[0];
			rx_Index++;
		}
		else{
			strcpy(temp_receive,rxBuf);
			HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
			rx_Index = 0;
		}
	}
  HAL_UART_Receive_IT(&huart6 , (uint8_t*)rxBuf_receive, 1);
}

/***********************FUNCTION FOR READING SENSOR*******************/
float read_gas_ppm(){	
	float gas_value_inVolts = 0.0;
	double ppm_in_log;
	float ppm;
	
	HAL_ADC_Start(&hadc1);
	HAL_Delay(100);
	gas_value_inVolts = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	gas_value_inVolts = 5*gas_value_inVolts/4095;
	
	ppm_in_log = (log10(((5 - gas_value_inVolts)/gas_value_inVolts)/0.1260) - 1.2908)/(-0.461);
	
	ppm = pow(10,ppm_in_log);
	
	return ppm;
}

int read_HALL(){
	
	uint16_t HALL_value; 
	
	HAL_ADC_Start(&hadc2);
	HAL_Delay(100);
	HALL_value = HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	
	return HALL_value;
	
}
char* read_rain_char(){
	bool rain_bool;
	rain_bool = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
	snprintf(rain_char,sizeof(rain_char),"RAIN %d",rain_bool);

  return rain_char; //IT'S CAN RETURN GLOBAL VARIABLE ONLY
}
char* read_pir_char(){
	bool pir_bool;
	pir_bool = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
	snprintf(pir_char,sizeof(pir_char),"PIR %d",pir_bool);

  return pir_char; //IT'S CAN RETURN GLOBAL VARIABLE ONLY
}
char* read_flame_char(){
	bool flame_bool;
	flame_bool = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
	snprintf(flame_char,sizeof(flame_char),"FLAME %d",flame_bool);

  return flame_char; //IT'S CAN RETURN GLOBAL VARIABLE ONLY
}
char read_keypad (void){
	 /*Configure GPIO pin : PA15 */
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);	

  /*Configure GPIO pin : PC11 */
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);	

  /*Configure GPIO pins : PD0 PD2 */
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);	

	/* Make ROW 1 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_RESET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High
	
	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
		HAL_Delay(5);
		if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN))){
			while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));   // wait till the button is pressed
			return '1';		
		}
	}
	
	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));   // wait till the button is pressed
		return '2';
	}
	
	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));   // wait till the button is pressed
		return '3';
	}
	
	if (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))   // if the Col 4 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)));   // wait till the button is pressed
		return 'A';
	}
	
	/* Make ROW 2 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_RESET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High
	
	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));   // wait till the button is pressed
		return '4';
	}
	
	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));   // wait till the button is pressed
		return '5';
	}
	
	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));   // wait till the button is pressed
		return '6';
	}
	
	if (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))   // if the Col 4 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)));   // wait till the button is pressed
		return 'B';
	}
	
	
	/* Make ROW 3 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_RESET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High
	
	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));   // wait till the button is pressed
		return '7';
	}
	
	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));   // wait till the button is pressed
		return '8';
	}
	
	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));   // wait till the button is pressed
		return '9';
	}
	
	if (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))   // if the Col 4 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)));   // wait till the button is pressed
		return 'C';
	}
	
		
	/* Make ROW 4 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_RESET);  // Pull the R4 High
	
	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))   // if the Col 1 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));   // wait till the button is pressed
		return '*';
	}
	
	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))   // if the Col 2 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));   // wait till the button is pressed
		return '0';
	}
	
	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))   // if the Col 3 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));   // wait till the button is pressed
		return '#';
	}
	
	if (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))   // if the Col 4 is low
	{
		HAL_Delay(5);
		while (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)));   // wait till the button is pressed
		return 'D';
	}
	return NULL;
}

void verify_password(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	char password_key;
	password_key = read_keypad();
	HAL_Delay(5);
	if(password_key == 'D'){
		LCD_Clear();
		LCD_SetCursor(2,1);
		LCD_Send_String("   ENTER PASSWORD   ",STR_NOSLIDE);
		LCD_SetCursor(3,6);
		i = 0;
		password_key = NULL;
	}
	
	if(password_key != 'D' && password_key != 'B' && password_key != 'A' && password_key != 'C'  ){
		if(password_key){
			password_input[i] = password_key;
			LCD_Send_String("*",STR_NOSLIDE);
			i++;
		}
	}

	if(i == 8){
		if(strncmp(password_input, password_setup, 8) == 0){
			LCD_Clear();
			LCD_SetCursor(2,4);
			LCD_Send_String("ACCESS GRANTED",STR_NOSLIDE);
			serial_send_cmd("Homescreen gate ON");
			HAL_Delay(2000);
			servo_position(GATE_PIN,OPEN);
			LCD_Clear();
			LCD_SetCursor(2,1);
			LCD_Send_String("   Good day Hieu!   ",STR_NOSLIDE);
			HAL_Delay(1000);
			LCD_SetCursor(3,6);
			i = 0;
		}
		else {
			LCD_Clear();
			LCD_SetCursor(2,4);
			LCD_Send_String("ACCESS DENIED!",STR_NOSLIDE);
			servo_position(GATE_PIN,CLOSE);
			serial_send_cmd("Homescreen gate OFF");
			HAL_Delay(1000);
			LCD_Clear();
			LCD_SetCursor(2,1);
			LCD_Send_String("   ENTER PASSWORD   ",STR_NOSLIDE);
			LCD_SetCursor(3,6);
			i = 0;
		}
	}
}

void change_password(){
	
	int j = 0;
	LCD_Clear();
	LCD_SetCursor(2,1);
	LCD_Send_String(" ENTER OLD PASSWORD ",STR_NOSLIDE);
	LCD_SetCursor(3,6);
	
	while( j < 8){
		
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		/*Configure GPIO pin : PA15 */
		GPIO_InitStruct.Pin = GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		/*Configure GPIO pin : PC11 */
		GPIO_InitStruct.Pin = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		/*Configure GPIO pins : PD0 PD2 */
		GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		
		char passwordkey_change =  read_keypad();
		HAL_Delay(10);
		if (passwordkey_change && passwordkey_change != 'B' && passwordkey_change != 'A' && passwordkey_change != 'D' && passwordkey_change != 'C'){
			password_input[j] = passwordkey_change;
			LCD_Send_String("*",STR_NOSLIDE);
			j++;
		}
		passwordkey_change = 0;
	}
	if(strncmp(password_input,password_setup,8) == 0){
		j = 0;
		LCD_Clear();
		LCD_SetCursor(2,1);
		LCD_Send_String(" ENTER NEW PASSWORD ",STR_NOSLIDE);
		LCD_SetCursor(3,6);
		while (j < 8){
			
			GPIO_InitTypeDef GPIO_InitStruct = {0};
			/*Configure GPIO pin : PA15 */
			GPIO_InitStruct.Pin = GPIO_PIN_15;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			/*Configure GPIO pin : PC11 */
			GPIO_InitStruct.Pin = GPIO_PIN_11;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
			/*Configure GPIO pins : PD0 PD2 */
			GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
			
			char passwordkey_change =  read_keypad();
			HAL_Delay(10);
			if(passwordkey_change && passwordkey_change != 'B' && passwordkey_change != 'A' && passwordkey_change != 'D' && passwordkey_change != 'C'){
				password_new[j] = passwordkey_change;
				LCD_Send_String("*",STR_NOSLIDE);
				j++;
			}
		}
		j = 0;
		LCD_Clear();
		LCD_SetCursor(2,1);
		LCD_Send_String("CONFIRM NEW PASSWORD",STR_NOSLIDE);
		LCD_SetCursor(3,6);
		while (j < 8){
			
			GPIO_InitTypeDef GPIO_InitStruct = {0};
			/*Configure GPIO pin : PA15 */
			GPIO_InitStruct.Pin = GPIO_PIN_15;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			/*Configure GPIO pin : PC11 */
			GPIO_InitStruct.Pin = GPIO_PIN_11;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
			/*Configure GPIO pins : PD0 PD2 */
			GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2;
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
			
			char passwordkey_change =  read_keypad();
			HAL_Delay(10);
			if(passwordkey_change && passwordkey_change != 'B' && passwordkey_change != 'A' && passwordkey_change != 'D' && passwordkey_change != 'C'){
				password_new_confirm[j] = passwordkey_change;
				LCD_Send_String("*",STR_NOSLIDE);
				j++;
			}
		}
		if(strncmp(password_new,password_new_confirm,8) == 0){
			strcpy(password_setup,password_new);
			
			snprintf(password_update_firebase,sizeof(password_update_firebase),"password %s",password_setup);
			serial_send_cmd(password_update_firebase);
			
			LCD_Clear();
			LCD_SetCursor(2,1);
			LCD_Send_String("  PASSWORD CHANGED  ",STR_NOSLIDE);	
			HAL_Delay(2000);
			
			LCD_Clear();
			LCD_SetCursor(2,1);
			LCD_Send_String("   ENTER PASSWORD  ",STR_NOSLIDE);
			LCD_SetCursor(3,6);			
		}else{
			LCD_Clear();
			LCD_SetCursor(2,1);
			LCD_Send_String("PASSWORD NOT MATCHED",STR_NOSLIDE);
			HAL_Delay(2000);
			
			LCD_Clear();
			LCD_SetCursor(2,1);
			LCD_Send_String("   ENTER PASSWORD  ",STR_NOSLIDE);
			LCD_SetCursor(3,6);	
		}
	}
	else{
		LCD_Clear();
		LCD_SetCursor(2,1);
		LCD_Send_String("   WRONG PASSWORD   ",STR_NOSLIDE);
		LCD_SetCursor(3,1);
		LCD_Send_String("     TRY AGAIN!     ",STR_NOSLIDE);
		HAL_Delay(2000);
		
		LCD_Clear();
		LCD_SetCursor(2,1);
		LCD_Send_String("   ENTER PASSWORD  ",STR_NOSLIDE);
		LCD_SetCursor(3,6);
	}
}
void automation_home(){
	bool pir_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
	bool rain_state = !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
	

	//PIR Sensor 
	if(pir_state == true){
		control_light("Livingroom",ON);

		serial_send_cmd("Homescreen smartlight ON");
		HAL_Delay(500);
	}
	
	//RAIN Sensor 
	if (rain_state == true){
		servo_position(DRYINGPOLE_PIN, 0);
		
		serial_send_cmd("Homescreen dryingpole OFF");
		HAL_Delay(10);
	}
	
	//GAS and FLAME Sensor 
	if ( (read_gas_ppm() > GAS_THRESH) || (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)) ){
		
		music_play(850);
		HAL_Delay(500);
		control_window("Kitchen", ON);	
		control_fan("Kitchen",ON);
		servo_position(GATE_PIN,OPEN);
		
		serial_send_cmd("Kitchen window ON");
		HAL_Delay(10);
		serial_send_cmd("Kitchen fan ON");
		HAL_Delay(10);
		serial_send_cmd("Homescreen gate ON");
		HAL_Delay(10);
		serial_send_cmd("Homescreen bell ON");
		
		if(read_gas_ppm() > GAS_THRESH){
			gas_in_ppm = read_gas_ppm();
			snprintf(data_GAS_char,sizeof(data_GAS_char),"GAS %4.2f",gas_in_ppm);
			serial_send_cmd(data_GAS_char);
			HAL_Delay(500);
		}
		if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)){
			serial_send_cmd("FLAME 1");
			HAL_Delay(500);
		}
	}else {
		if(HAL_GPIO_ReadPin(BELL_PORT,BELL_PIN)){
			HAL_Delay(500);
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)){
				music_stop();
				serial_send_cmd("Homescreen bell OFF");
				HAL_Delay(50);
			}
		}		
	}
}

void receive_password_from_firebase(){
	serial_read();
	if(strncmp(temp_receive,"password",8) == 0){
		strncpy(password_setup,&temp_receive[9],8);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM9_Init();
  MX_USART6_UART_Init();
  MX_USART1_UART_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
	music_Init();
	servo_Init();
	
	LCD_Init();
	LCD_i2cDeviceCheck();
	LCD_Clear();
	LCD_BackLight(LCD_BL_ON);
	LCD_SetCursor(1,1);
	LCD_Send_String("SMARTHOME PROJECT",STR_NOSLIDE);
	LCD_SetCursor(2,1);
	LCD_Send_String("SVTH: H.T.HIEU",STR_NOSLIDE);
	LCD_SetCursor(3,1);
	LCD_Send_String("MSSV: 1711277",STR_NOSLIDE);
	LCD_SetCursor(4,1);
	LCD_Send_String("GVHD: ThS. V.T.THONG",STR_NOSLIDE);

	HAL_UART_Receive_IT(&huart6 , (uint8_t*)rxBuf_receive, 1);
	
	control_window("Bedroom",OFF);
	control_window("Kitchen",OFF);
	servo_position(GATE_PIN, CLOSE);
	servo_position(DRYINGPOLE_PIN, 0);
	serial_send_cmd("Homescreen bell OFF");

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02Function, NULL, &myTask02_attributes);

  /* creation of myTask03 */
  myTask03Handle = osThreadNew(StartTask03Function, NULL, &myTask03_attributes);

  /* creation of myTask04 */
  myTask04Handle = osThreadNew(StartTask04Function, NULL, &myTask04_attributes);

  /* creation of myTask05 */
  myTask05Handle = osThreadNew(StartTask05Function, NULL, &myTask05_attributes);

  /* creation of myTask06 */
  myTask06Handle = osThreadNew(StartTask06Function, NULL, &myTask06_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 90;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
  hiwdg.Init.Reload = 1499;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 900-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 900-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 692-1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 255-1;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim9, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */
  HAL_TIM_MspPostInit(&htim9);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_4|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC2 PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA2 PA4 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA3 PA5 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PB13 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 
                           PD4 PD6 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_4|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
		control_home();
		HAL_IWDG_Refresh(&hiwdg);

    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartTask02Function */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02Function */
void StartTask02Function(void *argument)
{
  /* USER CODE BEGIN StartTask02Function */
  /* Infinite loop */
  for(;;)
  {
		if(HAL_GetTick() - timeout_to_send_dht11value >= 30000){
			DHT11_Init();
			read_DHT11(data_dht11);
			sprintf(data_dht11_char,"DHT11 %d.%d %d.%d",data_dht11[2],data_dht11[3],data_dht11[0],data_dht11[1]);			
			serial_send_cmd(data_dht11_char);
			HAL_Delay(100);
			timeout_to_send_dht11value = HAL_GetTick();
		}
		
		if(HAL_GetTick() - timeout_to_send_gasvalue >= 20000){
			gas_in_ppm = read_gas_ppm();
			snprintf(data_GAS_char,sizeof(data_GAS_char),"GAS %4.2f",gas_in_ppm);
			serial_send_cmd(data_GAS_char);	
			HAL_Delay(100);
			if(gas_in_ppm >= GAS_THRESH){
				music_play(500);
				HAL_Delay(200);
				music_play(650);
				HAL_Delay(200);
			}
			else{
				music_stop();
			}	
			timeout_to_send_gasvalue = HAL_GetTick();
		}
		
    osDelay(50);
  }
  /* USER CODE END StartTask02Function */
}

/* USER CODE BEGIN Header_StartTask03Function */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03Function */
void StartTask03Function(void *argument)
{
  /* USER CODE BEGIN StartTask03Function */

  /* Infinite loop */
  for(;;)
  {
		receive_password_from_firebase();

		verify_password();
		
		function_key = read_keypad();
		HAL_Delay(25);
		if(function_key == 'B'){
			change_password();
		}			
		
    osDelay(10);
  }
  /* USER CODE END StartTask03Function */
}

/* USER CODE BEGIN Header_StartTask04Function */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04Function */
void StartTask04Function(void *argument)
{
  /* USER CODE BEGIN StartTask04Function */
  /* Infinite loop */
  for(;;)
  {
		automation_home();
		if(HAL_GetTick() - timeout_to_send_pirvalue >= 5500){ 
			strcpy(data_PIR_char,read_pir_char());
			serial_send_cmd(data_PIR_char);
			HAL_Delay(100);
			
			timeout_to_send_pirvalue = HAL_GetTick();
		}	
		
		if(HAL_GetTick() - timeout_to_send_rainvalue >= 7500){
			strcpy(data_RAIN_char,read_rain_char());
			
			serial_send_cmd(data_RAIN_char);
			HAL_Delay(100);
			
			timeout_to_send_rainvalue = HAL_GetTick();		
		}
		
		if(HAL_GetTick() - timeout_to_send_flamevalue >= 4500){
			strcpy(data_FLAME_char,read_flame_char());
			serial_send_cmd(data_FLAME_char);
			HAL_Delay(100);
			
			timeout_to_send_flamevalue = HAL_GetTick();
		}
		
    osDelay(20);
  }
  /* USER CODE END StartTask04Function */
}

/* USER CODE BEGIN Header_StartTask05Function */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask05Function */
void StartTask05Function(void *argument)
{
  /* USER CODE BEGIN StartTask05Function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask05Function */
}

/* USER CODE BEGIN Header_StartTask06Function */
/**
* @brief Function implementing the myTask06 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask06Function */
void StartTask06Function(void *argument)
{
  /* USER CODE BEGIN StartTask06Function */
  /* Infinite loop */
  for(;;)
  {
		if(HAL_GetTick() - timeout_to_send_hallvalue >= 3500){ 
			current_value_main = read_Current();
			if(current_value_main >=100){
				snprintf(data_HALL_char,sizeof(data_HALL_char),"HALL 1");
				serial_send_cmd(data_HALL_char);
				HAL_Delay(100);	
			}else{
				snprintf(data_HALL_char,sizeof(data_HALL_char),"HALL 0");
				serial_send_cmd(data_HALL_char);
				HAL_Delay(100);
			}
			
			timeout_to_send_hallvalue = HAL_GetTick();
		}	
		
    osDelay(30);
  }
  /* USER CODE END StartTask06Function */
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM10 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM10) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
