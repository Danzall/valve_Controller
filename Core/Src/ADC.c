/*
 * ADC.c
 *
 *  Created on: Jun 24, 2019
 *      Author: Jeefo
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
//#include "cmsis_os.h"
#include <stdio.h>

/* USER CODE BEGIN Includes */
//#include "GSM.h"
//#include "motor.h"
//#include "RTC.h"
#include "ADC.h"
#include "stdlib.h"
#include "string.h"
#include "motor.h"
/* USER CODE END Includes */
extern ADC_HandleTypeDef hadc;
uint32_t adc[20];
uint32_t AC_I;
uint32_t AC_I_real;
uint32_t M_I;
uint32_t Vin;
uint32_t Vsys;
char temp2[90];
extern valveData valve;

void ADC_Service(){
	//Debug_Send("ADCR start\r\n");
	HAL_ADC_Start_DMA(&hadc, adc, 3);
	//Debug_Send("ADCR stop\r\n");
	//HAL_DMA_Start_IT(&hadc.*DMA_Handle, (uint32_t)&hadc.*Instance.DR, adc, 6);
	//HAL_DMA_PollForTransfer(&hadc.DMA_Handle, 1, 100);

	char temp1[10];
	//sprintf(temp2, "1:%i  2:%i  3:%i  4:%i  5:%i  6:%i  7:%i  8:%i  9:%i\r\n",adc[0],adc[1],adc[2],adc[3],adc[4],adc[ValveLimit],adc[6],adc[7],adc[8]);
	sprintf(temp2, "1:%i  2:%i  3:%i\r\n",adc[Pressure],adc[Vsupply],adc[ValveLimit]);
	//Debug_Send("Here\r\n");
	/*AC_I = GetVolt(adc[ValveLimit]);
	AC_I_real = GetCurr(AC_I);
	Vin = GetVolt(adc[1]);
	Vin *= 11;
	Vsys = GetVolt(adc[0]);
	M_I = GetVolt(adc[6]);*/
	//sprintf(temp, "I:%i  AC:%i A real:%i\r\n",adc[6], adc[5], AC_I);
	//strcpy(temp, "Motor I:"); //myStrLong(char *p_str,char p_base)
	//itoa(AC_I,temp1,10);
	//strcat(temp,temp1);
	//strcat(temp,"  AC:");
	//itoa(AC_I,temp1,10);
	//strcat(temp,temp1);
	//strcat(temp,"  Motor I:");
	//itoa(adc[5],temp1,10);
	//itoa(GetVolt(adc[5]),temp1,10);
	//strcat(temp,temp1);
	//strcat(temp,"  Vsystem:");
	//itoa(Vsys,temp1,10);
	//strcat(temp,temp1);
	//strcat(temp,"  AC:");
	//strcat(temp,"\r\n");
	//while (HAL_ADC_PollForConversion(&hadc, 10) != HAL_OK);
	//adcval = HAL_ADC_GetValue(&hadc);
	//sprintf(temp1, "2:%i\r\n",adcval);
	//strcat(temp, temp1);
	//Debug_Send(temp2);
	/*if (HAL_GPIO_ReadPin(AC_Voltage_GPIO_Port, AC_Voltage_Pin) == GPIO_PIN_SET){
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		//Debug_Send("AC voltage off\r\n");
	}
	else{
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		//Debug_Send("AC voltage on\r\n");
	}*/
}

uint32_t GetADC(){
	return AC_I_real;
	return adc;
}

uint32_t* GetADC1(){
	return &adc;
}
uint32_t getADC(ADC_H val){
	uint32_t ret;
	ret = adc[val];
	  sprintf(temp2, "Return val: :%i\r\n",(int)ret );
	  Debug_Send(temp2);
	return ret;
}

uint32_t GetVolt(uint32_t in){
	float temp;
	uint32_t val;
	temp = in;
	temp *= 0.8057;
	val = temp;
	return val;
}

uint32_t GetCurr(uint32_t in){
	in -= 1800;
	float current;
	uint32_t val;
	current = in * 1000;
	current /= 80;
	val = current;
	return val;

}

