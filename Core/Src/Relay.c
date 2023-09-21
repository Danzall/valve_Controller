/*
 * Relay.c
 *
 *  Created on: Apr 26, 2019
 *      Author: Jeefo
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
//#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "GSM.h"
#include "motor.h"
/* USER CODE END Includes */

void RelayOn(){
	//HAL_GPIO_WritePin(Relay_Close_GPIO_Port, Relay_Close_Pin, GPIO_PIN_SET);
	HAL_Delay(50);
	//HAL_GPIO_WritePin(Relay_Close_GPIO_Port, Relay_Close_Pin, GPIO_PIN_RESET);

}

void RelayOff(){
	//HAL_GPIO_WritePin(Relay_Open_GPIO_Port, Relay_Open_Pin, GPIO_PIN_SET);
	HAL_Delay(50);
	//HAL_GPIO_WritePin(Relay_Open_GPIO_Port, Relay_Open_Pin, GPIO_PIN_RESET);
	//HAL_GPIO_TogglePin(Relay_Open_GPIO_Port, Relay_Open_Pin);
}

