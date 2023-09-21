/*
 * ADC.h
 *
 *  Created on: Jun 24, 2019
 *      Author: Jeefo
 */

#ifndef ADC_H_
#define ADC_H_

void ADC_Service();
uint32_t GetADC();
uint32_t GetVolt(uint32_t in);
uint32_t GetCurr(uint32_t in);
uint32_t* GetADC1();

typedef enum{
	Pressure = 0,
	Vsupply,
	ValveLimit
}ADC_H;
uint32_t getADC(ADC_H val);
#endif /* ADC_H_ */
