/*
 * Control.h
 *
 * Created: 8/4/2017 3:05:33 PM
 *  Author: Angela Belnett
 */ 


#ifndef CONTROL_H_
#define CONTROL_H_

#include "Const.h"
#include "UART.h"

unsigned char Read_Brightness_Level(void);
unsigned char Read_TypeOfLamp_To_Control(void);
void Feedback_Control(unsigned char BrightnessLevel, unsigned char TypeOfLamp);

void SendMessage(unsigned char TypeOfMessage);
void ParseMessage(void);
void Ping(void);
void CheckOnlineDevice(void);
unsigned int CountOnlineDevice(void);
void ClearOnlineTable(void);

#endif /* CONTROL_H_ */