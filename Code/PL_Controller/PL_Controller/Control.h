#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <avr/io.h>
#include "Const.h"
#include "ADC.h"
#include "Timming.h"
#include "UART.h"

typedef struct
{
	unsigned char LampID;
	unsigned char TypeOfLamp;
	unsigned char Brightness_level;
	unsigned char LocalRemote_Mode;
} PortableLampStruct;

extern unsigned int Battery_voltage, Charger_voltage, Charger_current, Lamp_current1, Lamp_current2;
//extern unsigned char Brightness_level, LocalRemote_Mode, TypeOfLamp;
extern PortableLampStruct PortableLamp;
extern unsigned char flag_ok;

void initLamp_Parameter(void);
unsigned char Read_Brighness_Level(void);
unsigned char Read_LocalRemote_Mode(void);
void Read_ADC_Channel(unsigned int *V_BATTERY, unsigned int *V_CHARGER, unsigned int *I_CHARGER, 
						unsigned int *LAMP_CURRENT1, unsigned int *LAMP_CURRENT2);

// signed char Read_RF_Temperature_Sensor(void);
void Lamp_Brightness_Control(void);
void Battery_Charger_Control(void);
void Battery_Voltage_Indicate(unsigned char Display_Enable);
void ParseMessage(void);
void Send_Response_Message(void);
unsigned char CheckErrorlamp(void);
void ErrorLampIndicate(void);
void Reset_Duty_Min_Max(void);
#endif