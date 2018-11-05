#ifndef _TIMMING_H_
#define _TIMMING_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "Const.h"
#include "MBSlave.h"

extern volatile unsigned char flag_1kHz, flag_100Hz, flag_10Hz, flag_1Hz;

void initTimer0(void);
void initTimer1(void);
void initTimer2(void);
void initInterrupt(void);
void StartStop_PWM(unsigned int DutyA, unsigned int DutyB);

#endif 
