/*
 * ADC.h
 *
 * Created: 7/17/2017 3:20:57 PM
 *  Author: Angela Belnett
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define NUM_OF_ADC_CHANNEL	5

extern unsigned int ADC_Data[NUM_OF_ADC_CHANNEL];

void init_ADC(void);

#endif /* ADC_H_ */