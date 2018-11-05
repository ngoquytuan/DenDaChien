#ifndef _TIMMING_H_
#define _TIMMING_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "Control.h"
#include "UART.h"

extern volatile unsigned char flag_1Hz, flag_1kHz, flag_10Hz, flag_100Hz, flag_flashing;

void initTimer0(void);
void initTimer1(void);
void initTimer2(void);
void initInterrupt(void);

void StartStop_PWM(unsigned int CurrentA, unsigned int CurrentB);

#endif 
