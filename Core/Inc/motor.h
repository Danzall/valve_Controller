/*
 * motor.h
 *
 *  Created on: Apr 25, 2019
 *      Author: Jeefo
 */

#ifndef MOTOR_H_
#define MOTOR_H_

void Motor_Service();
void ValveOpen();
void ValveClose();
void open();
void close();
void stop();
void closeStep();
void openStep();
#include "RTC.h"
typedef enum{
	Open,
	Close,
	IdleValve
}valveState;
typedef struct{
	uint8_t calibrate;
	uint16_t idleCurrent;
	uint16_t openTimer;
	uint16_t closeTimer;
	uint32_t timeout;
	valveState state;
	uint8_t calibrationError;
	uint8_t stepSize;
	uint8_t position;
	uint16_t flowTimer;
	uint16_t stepTimer;
	uint16_t flowCount;
	uint8_t targetPosition;
	uint16_t runTimer;
}valveData;
typedef enum{
	error,
	calEndStop,
	calClose,
	calOpen,
	calCheck,
	flowMonitor,
	pressureRead,
	pressureLow,
	pressureHigh,
	pressureCalc,
	flowMonitorSave,
	sendingEmail,
	Idle
}valveService;
typedef struct{
	uint16_t initial;
	uint16_t high;
	uint16_t highPosition;
	uint16_t low;
	uint16_t lowPosition;
	uint16_t counter;
}pressure;
typedef struct{
	int eventNum;
	rtc timestamp;
	float gallons;
}event;
typedef struct{
	int counter;

}flow;
#endif /* MOTOR_H_ */
