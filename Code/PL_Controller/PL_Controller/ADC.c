/*
 * ADC.c
 *
 * Created: 7/17/2017 3:21:38 PM
 *  Author: Angela Belnett
 */ 
#include "ADC.h"

volatile unsigned char adc_channel[NUM_OF_ADC_CHANNEL] ={0,1,2,6,7}, adc_index=0;
unsigned int ADC_Data[NUM_OF_ADC_CHANNEL];	

void init_ADC(void)
{
	ADCSRA 	= 0x8E;//1000 1110 Interrupt Enable, CLK/64
	ADMUX 	= 0xC0|adc_channel[adc_index];//1100 0000 AREF
	ADCSRA 	|= 0x40;
}

ISR(ADC_vect)
{
	ADC_Data[adc_index]	= ADCW;
	
	adc_index++;
	if (adc_index>=NUM_OF_ADC_CHANNEL) adc_index=0;
	ADMUX	= 0xC0 | adc_channel[adc_index];
	ADCSRA 	|= 0x40;
}