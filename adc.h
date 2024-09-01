/*
 * adc.h
 *
 *  Created on: Aug 21, 2024
 *      Author: youssef
 */

#ifndef ADC_H_
#define ADC_H_
#include "sensors.h"

//#define ADC_INTERRUPT_PRIORITY 4U

void ADCinit(void);


void ADCFinishedHandler(void);

void ADCTrigger(void);

#endif /* ADC_H_ */
