/*
 * Control.c
 *
 * Created: 8/4/2017 3:05:49 PM
 *  Author: Angela Belnett
 */ 
#include "Control.h"
extern unsigned char TypeOfLamp, BrightnessLevel;
extern char str_buffer[20];

const char HEADER[HEADER_LENGTH] = "AT";
volatile unsigned char pos=0, datapos=0;
volatile unsigned char header_ok=0;
volatile unsigned char NewMessage_Flag=0;
char buffer[20];
unsigned int Online_Device_Table[NUM_OF_ONLINE_LAMP];
unsigned int numofonlinedevice[NUMOFLAMPTYPE];

unsigned char Read_Brightness_Level(void)
{
	switch (BRIGHTNESS_CONTROL)
	{
		case BRIGHTNESS_OFF:		return LEVEL_0; break;
		case BRIGHTNESS_LEVEL_1:	return LEVEL_1; break;
		case BRIGHTNESS_LEVEL_2:	return LEVEL_2; break;
		case BRIGHTNESS_LEVEL_3:	return LEVEL_3; break;
	}
	return LEVEL_0;
}
unsigned char Read_TypeOfLamp_To_Control(void)
{
	switch (TYPEOFLAMP_CONTROL)
	{
		case TYPEOFLAMP_RUNWAY: return RUNWAY; break;
		case TYPEOFLAMP_TAXIWAY: return TAXIWAY; break;
		case TYPEOFLAMP_ENDTHRES: return ENDTHRES; break;
		case TYPEOFLAMP_FLASHING: return FLASHING; break;
	}
	return RUNWAY;
}
void Feedback_Control(unsigned char BrightnessLevel, unsigned char TypeOfLamp)
{
	switch (BrightnessLevel)
	{
		case LEVEL_0: FB_LEVEL_0; break;
		case LEVEL_1: FB_LEVEL_1; break;
		case LEVEL_2: FB_LEVEL_2; break;
		case LEVEL_3: FB_LEVEL_3; break; 
	}
	switch (TypeOfLamp)
	{
		case RUNWAY: FB_LAMP_RUNWAY; break;
		case TAXIWAY: FB_LAMP_TAXIWAY; break;
		case ENDTHRES: FB_LAMP_ENDTHRES; break;
		case FLASHING: FB_LAMP_FLASHING; break;
	}
}

//==============================================================================================================================
void CheckOnlineDevice(void)
{
	unsigned int i=0;
	unsigned long Temp_ID=0;
	unsigned char Device_Listed=0, Message_Type=0;
	
	Message_Type = (buffer[0]-'0')*10 + (buffer[1]-'0');
	if (Message_Type==RESPONSE_MESSAGE)
	{
		Temp_ID += (buffer[2]-'0')*10000 + (buffer[3]-'0')*1000 + (buffer[4]-'0')*100 + (buffer[5]-'0')*10 + (buffer[6]-'0');
		while (Online_Device_Table[i])
		{
			if (Temp_ID==Online_Device_Table[i])
			{
				Device_Listed=1;
				break;
			}
			i++;
		}
		if (Device_Listed==0) Online_Device_Table[i] = Temp_ID;
	}
}
//==============================================================================================================================
unsigned int CountOnlineDevice(void)
{
	unsigned int i=0;
	
	for (i=0; i<NUMOFLAMPTYPE; i++) numofonlinedevice[i]=0;
	for (i=0; i<NUM_OF_ONLINE_LAMP; i++)
	{
		if (Online_Device_Table[i])
		{
			numofonlinedevice[4]++;
			if (Online_Device_Table[i]<1000)
			numofonlinedevice[0]++;
			else
			if (Online_Device_Table[i]<2000)
			numofonlinedevice[1]++;
			else
			if (Online_Device_Table[i]<3000)
			numofonlinedevice[2]++;
			else
			if (Online_Device_Table[i]<4000)
			numofonlinedevice[3]++;
		}
	}
	return numofonlinedevice[4];
}
//==============================================================================================================================
void ClearOnlineTable(void)
{
	unsigned int i=0;
	for (i=0; i<NUM_OF_ONLINE_LAMP; i++)
	{
		Online_Device_Table[i]=0;
	}
	for (i=0; i<NUMOFLAMPTYPE; i++)
	{
		numofonlinedevice[i]=0;
	}
	
}
//==============================================================================================================================
void Ping(void)
{
	ClearOnlineTable();
	writeStringUART1("AT02\n");
}
//==============================================================================================================================
void SendMessage(unsigned char TypeOfMessage)
{
	ClearOnlineTable();
	if (TypeOfMessage == CONTROL_MESSAGE)
	{
		// 		LCD_write_string(0,4, "Sending...");
		writeStringUART1("AT");
		toDecStr2(str_buffer, TypeOfMessage);					writeMStringUART1(str_buffer);
		toDecStr2(str_buffer, TypeOfLamp);						writeMStringUART1(str_buffer);
		toDecStr2(str_buffer, BrightnessLevel);	writeMStringUART1(str_buffer);
		writeStringUART1("\n");
	}
}
//==============================================================================================================================
void ParseMessage(void)
{
	char Temp=0;
	if (queueLengthUART1())
	{
		Temp = readUART1();
		// 		writeUART(Temp);
		if (Temp=='\n')
		{
			NewMessage_Flag=1;
			// 			CheckOnlineDevice();
			header_ok=0;
			datapos=0;
			pos=0;
		}
		else
		{
			if (header_ok==0)
			{
				if (Temp==HEADER[pos])
				{
					pos++;
					if (pos>=HEADER_LENGTH) {header_ok=1;datapos=0;}
				}
				else
				{
					pos=0;
					header_ok=0;
					datapos=0;
				}
			}
			else
			{
				pos++;
				buffer[datapos] = Temp;
				datapos++;
				buffer[datapos]='\0';
			}
		}
	}
}
//==============================================================================================================================


