#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <avr/io.h>
#include "Const.h"
#include "LCD5110.h"
#include "UART.h"

void Brightness_screen(volatile unsigned char *key_press);
void Display_MainScreen(void);

void SendMessage(unsigned char TypeOfMessage);
void ParseMessage(void);
void Ping(void);
void CheckOnlineDevice(void);
unsigned int CountOnlineDevice(void);
void ClearOnlineTable(void);
#endif