/*
 * motor.c
 *
 *  Created on: Apr 25, 2019
 *      Author: Jeefo
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
//#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
//#include "GSM.h"
#include "motor.h"
#include "Relay.h"
#include "ADC.h"
#include "stdlib.h"
#include "string.h"
#include "ssd1306.h"
/* USER CODE END Includes */

//uint16_t  mode = 6;
uint32_t* adc_;
//uint32_t timeout;
//valveState state;
valveData valve;
valveService mode = calEndStop;
//valveService mode = Idle;
pressure waterPressure;
int diff;
extern float gallonsPm;
uint32_t tempVal;
uint8_t count;
event flowEvents[30];
flow flowData;
int eventCounter = 1;
float gpmAverage;
float gpmAverage1;

void Motor_Service(){

	switch(mode){
	case calEndStop:
		//if(valve.calibrate == 0){
		//if (valve.timeout == 0){
		if((TIM1->CR1 & TIM_CR1_CEN) == 0){
			//Debug_Send("Start timer 14\r\n");
			//TIM14->CR1 |= TIM_CR1_CEN;
			//}
			//open();
			mode = calClose;
			//mode = Idle;
			/*valve.timeout = 1000;
			valve.state = Open;

			valve.runTimer = 0;

			HAL_GPIO_WritePin(ValveDir_GPIO_Portt, ValveDir_Pin, GPIO_PIN_RESET);
			pulseTim(10000);*/
			Debug_Send("begin cal\r\n");
			valve.state = Open;
			HAL_GPIO_WritePin(ValveDir_GPIO_Port, ValveDir_Pin, GPIO_PIN_RESET);
			pulseTim(15000);
			valve.timeout = 3000;
			valve.runTimer = 0;
		}
		//}


		break;
	case calClose:	//brake
		//if (valve.timeout == 0){
		if((TIM1->CR1 & TIM_CR1_CEN) == 0){
			//close();
			mode = calOpen;
			//mode = calEndStop;
			//mode = Idle;
			valve.timeout = 4000;
			valve.state = Close;
			valve.closeTimer = 0;
			Debug_Send("close cal\r\n");
			HAL_GPIO_WritePin(ValveDir_GPIO_Port, ValveDir_Pin, GPIO_PIN_SET);
			pulseTim(15000);
			valve.timeout = 3000;
			valve.runTimer = 0;
		}
		break;
	case calOpen:	//forward
		//if (valve.timeout == 0){
		if((TIM1->CR1 & TIM_CR1_CEN) == 0){
			//open();
			//if (valve.closeTimer < 100) error
			mode = calCheck;
			valve.timeout = 4000;
			valve.state = Open;
			valve.openTimer = 0;
			Debug_Send("open cal\r\n");
			HAL_GPIO_WritePin(ValveDir_GPIO_Port, ValveDir_Pin, GPIO_PIN_RESET);
			pulseTim(15000);
			valve.timeout = 3000;
			valve.runTimer = 0;
		}
		break;
	case calCheck:	//brake
		//uint8_t diff;
		if (valve.timeout == 0){
			float tempS;
			Debug_Send("check cal\r\n");
			if (valve.openTimer > valve.closeTimer){
				Debug_Send("open >\r\n");
				diff = valve.openTimer - valve.closeTimer;
				if (diff < 100){
					valve.calibrate = 1;
					tempS = valve.closeTimer % 40;
					valve.closeTimer = valve.closeTimer - tempS;
					valve.stepSize = valve.closeTimer / 40;
				}
				mode = flowMonitor;

			}
			else if (valve.closeTimer > valve.openTimer){
				Debug_Send("close >\r\n");
				diff = valve.closeTimer - valve.openTimer;
				if (diff <100){
					valve.calibrate = 1;
					tempS = valve.openTimer % 40;
					valve.openTimer = valve.openTimer - tempS;
					valve.stepSize = valve.openTimer / 20;
					//valve.stepSize = tempS;
				}
				mode = flowMonitor;

			}
			else{
				Debug_Send("perfect match\r\n");
			}
			char temp[60];
			if ((diff > 150)||(valve.openTimer == 0)||(valve.closeTimer == 0)){
				Debug_Send("calibration failed\r\n");
				valve.calibrate = 0;
				mode = 7;
			}
			//valve.stepSize = 16;	//debug
			valve.position = 100;
			sprintf(temp, "Step:%i, open: %i, close: %i, position: %i\r\n",valve.stepSize, valve.openTimer, valve.closeTimer, valve.position);
			Debug_Send(temp);

		}
		break;
	case flowMonitor: //reverse
		//Debug_Send("Check flow task\r\n");
		valve.flowTimer++;
		if (valve.flowTimer == 10){
			Debug_Send("Check flow rate\r\n");
			valve.flowTimer = 0;
			char temp1[30];
			if (gallonsPm < 2){
				if(valve.position > 10){
					closeStep();
					//sprintf(temp, "Step count:%i,\r\n",count);
					Debug_Send("Step count:");
					myLongStr(count,temp1,10,10);
					Debug_Send(temp1);
					//Debug_Send(temp);
					Debug_Send("\r\n");
					count++;
					valve.timeout = 400;
				}
				else Debug_Send("Valve at its limit cannot close any more");
				valve.flowCount = 0;
			}
			else valve.flowCount++;
		}
		if (valve.flowCount >= 3){
			mode = pressureRead;
			waterPressure.initial = 0;
		}
		break;
	case pressureRead:
		//ValveOpen();
		ADC_Service();
		waterPressure.initial = GetADC1()[Pressure];
		/*ssd1306_WriteString("Open", Font_7x10, White);
		ssd1306_UpdateScreen();
		if (valve.timeout == 0){
			open();
			mode = 6;
			valve.timeout = 400;
			valve.state = Open;
		}*/
		waterPressure.low = 0;
		waterPressure.high = 0;
		waterPressure.counter = 0;
		mode = pressureLow;
		break;
	case pressureLow:
		valve.flowTimer++;
		if (valve.flowTimer == 10){
			valve.flowTimer = 0;
			Debug_Send("Check low pressure\r\n");
			openStep();
			valve.timeout = 400;
			HAL_Delay(100);
			ADC_Service();
			tempVal = GetADC1()[Pressure];
			if (tempVal < (waterPressure.initial - (waterPressure.initial / 10))){
				waterPressure.counter++;
			}
			else waterPressure.counter = 0;
			if (waterPressure.counter == 1){ 	//reading debounce
				waterPressure.low = tempVal;
				mode = pressureHigh;
				waterPressure.lowPosition = valve.position;
				valve.flowTimer = 0;
			}
		}
		break;
	case pressureHigh:
		valve.flowTimer++;
		if (valve.flowTimer == 10){
			valve.flowTimer = 0;
			Debug_Send("Check high pressure\r\n");
			closeStep();
			valve.timeout = 400;
			HAL_Delay(100);
			ADC_Service();
			tempVal = GetADC1()[Pressure];
			if (tempVal > (waterPressure.low + (waterPressure.low / 10))){
				waterPressure.counter++;
				Debug_Send("inc\r\n");
			}
			else waterPressure.counter = 0;
			if (waterPressure.counter == 1){
				Debug_Send("Completed high pressure\r\n");
				waterPressure.high = tempVal;
				mode = pressureCalc;
				waterPressure.highPosition = valve.position;
				Debug_Send("Display position differential\r\n");
			}
		}

		break;
	case pressureCalc:
		//sprintf(temp, "pos low:%i  pos high:%i\r\n",waterPressure.lowPosition ,waterPressure.highPosition );
		//Debug_Send(temp);

		diff = (waterPressure.lowPosition - waterPressure.highPosition) / 2;
		valve.targetPosition = diff + waterPressure.highPosition;
		//if((diff + waterPressure.highPosition)  > valve.position){
		if(valve.targetPosition  > valve.position){
			openStep();
			valve.timeout = 400;
		}
		else{
			Debug_Send("valve on optimal position\r\n");
			mode = flowMonitorSave;
		}
		Debug_Send("open step\r\n");
		break;
	case flowMonitorSave:
		flowData.counter++;
		rtc tempTime;
		char temp[20];
		char temp1[10];
		//gpmAverage = 0;
		ADC_Service();
		if(flowData.counter == 30){
			flowData.counter = 0;
			tempTime = getTime();

			flowEvents[eventCounter].timestamp = tempTime;
			flowEvents[eventCounter].eventNum = eventCounter;
			flowEvents[eventCounter].gallons = gallonsPm;
			gpmAverage += gallonsPm;
			gpmAverage1 = gpmAverage / eventCounter;


			strcpy(temp,"GPM ave:");

			gcvt(gpmAverage, 4, temp1);
			strcat(temp,temp1);
			strcat(temp,"-");
			myLongStr(eventCounter,temp1,10,10);
			strcat(temp,temp1);
			strcat(temp,"-");
			gcvt(gpmAverage1, 4, temp1);
			strcat(temp,temp1);
			strcat(temp,"\r\n");
			Debug_Send(temp);

			strcpy(temp,"Date1:");
			myLongStr(flowEvents[eventCounter].timestamp.months,temp1,10,10);
			strcat(temp,temp1);
			strcat(temp,"/");
			myLongStr(flowEvents[eventCounter].timestamp.days,temp1,10,10);
			strcat(temp,temp1);
			strcat(temp,"/");
			myLongStr(flowEvents[eventCounter].timestamp.years,temp1,10,10);
			strcat(temp,temp1);
			strcat(temp,"!!!!!!!\r\n");
			Debug_Send(temp);

			strcpy(temp,"time1:");
			myLongStr(flowEvents[eventCounter].timestamp.hours,temp1,10,10);
			strcat(temp,temp1);
			strcat(temp,":");
			myLongStr(flowEvents[eventCounter].timestamp.minutes,temp1,10,10);
			strcat(temp,temp1);
			strcat(temp,":");
			myLongStr(flowEvents[eventCounter].timestamp.seconds,temp1,10,10);
			strcat(temp,temp1);
			strcat(temp,"!!!!!!!!!!\r\n");
			Debug_Send(temp);
			if (eventCounter == 5){
				Debug_Send("Send Email");
				setEmail(&flowEvents, eventCounter, gpmAverage1);
				eventCounter = 1;
				gpmAverage = 0;
				mode = sendingEmail;

			}
			eventCounter++;
		}

		break;
	case sendingEmail:
		Debug_Send("Sending Email\r\n");
		RelayOff();
		break;
	case Idle:
		//Debug_Send("pulse now\r\n");
		//pulseTim(10);
		//valve.stepSize = 5;
		//openStep();
		//pulseTim(5000);
		//valve.timeout = 3000;
		break;
	}
}

void closeStep(){	//10 = 1ms
	//HAL_GPIO_WritePin(ValveEN_GPIO_Port, ValveEN_Pin, GPIO_PIN_SET);
	/*TIM1->CCR1 = 6500 - (valve.stepSize * 100);
	TIM1-> CNT = TIM1->CCR1 - 30;
	if((TIM1->CR1 & TIM_CR1_CEN) == 0){
	  Debug_Send("Start timer 14\r\n");
	  TIM1->CR1 |= TIM_CR1_CEN;
	}*/

	HAL_GPIO_WritePin(ValveDir_GPIO_Port, ValveDir_Pin, GPIO_PIN_SET);
	//pulseTim(valve.stepSize * 10);
	pulseTim(valve.stepSize);
	valve.state = Close;
	valve.position -= 5;
}

void openStep(){
	//HAL_GPIO_WritePin(ValveEN_GPIO_Port, ValveEN_Pin, GPIO_PIN_SET);
	/*TIM14->CCR1 = 6500 - (valve.stepSize * 100);
	if((TIM14->CR1 & TIM_CR1_CEN) == 0){
	  Debug_Send("Start timer 14\r\n");
	  TIM14->CR1 |= TIM_CR1_CEN;
	}*/
	HAL_GPIO_WritePin(ValveDir_GPIO_Port, ValveDir_Pin, GPIO_PIN_RESET);
	pulseTim(valve.stepSize);
	valve.state = Open;
	valve.position += 5;
}

void open(){
	//HAL_GPIO_WritePin(ValveEN_GPIO_Port, ValveEN_Pin, GPIO_PIN_SET);
	TIM1-> CNT = 0;
	TIM1->CCR1 = 5;
	if((TIM1->CR1 & TIM_CR1_CEN) == 0){
	  Debug_Send("Start timer 14\r\n");
	  TIM1->CR1 |= TIM_CR1_CEN;
	}
	HAL_GPIO_WritePin(ValveDir_GPIO_Port, ValveDir_Pin, GPIO_PIN_RESET);
	valve.state = Open;
	valve.runTimer = 0;
}

void close(){
	//HAL_GPIO_WritePin(ValveEN_GPIO_Port, ValveEN_Pin, GPIO_PIN_SET);
	TIM1-> CNT = 0;
	TIM1->CCR1 = 2000;
	if((TIM1->CR1 & TIM_CR1_CEN) == 0){
	  Debug_Send("Start timer 14\r\n");
	  TIM1->CR1 |= TIM_CR1_CEN;
	}
	HAL_GPIO_WritePin(ValveDir_GPIO_Port, ValveDir_Pin, GPIO_PIN_SET);
	valve.state = Close;
	valve.runTimer = 0;
}

void stop(){
	//HAL_GPIO_WritePin(ValveEN_GPIO_Port, ValveEN_Pin, GPIO_PIN_RESET);
	TIM14->CR1 &= ~(TIM_CR1_CEN);
	//HAL_GPIO_WritePin(ValvePH_GPIO_Port, ValvePH_Pin, GPIO_PIN_RESET);
	valve.state = IdleValve;
	valve.timeout = 0;
}






