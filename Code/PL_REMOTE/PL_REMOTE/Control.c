/*
 * Control.c
 *
 * Created: 7/14/2017 8:57:40 AM
 *  Author: Angela Belnett
 */ 
#include "Control.h"

extern unsigned char TypeOfLamp, Brightness_Level[NUMOFLAMPTYPE];
extern char str_buffer[20];

const char HEADER[HEADER_LENGTH] = "AT";
volatile unsigned char pos=0, datapos=0;
volatile unsigned char header_ok=0;
volatile unsigned char NewMessage_Flag=0;
char buffer[20];
unsigned int Online_Device_Table[NUM_OF_ONLINE_LAMP];
unsigned int numofonlinedevice[NUMOFLAMPTYPE];
//==============================================================================================================================
void Brightness_screen(volatile unsigned char *key_press)
{
	switch (*key_press)
	{
		case BUTTON_NUM0:
			Brightness_Level[TypeOfLamp]=LEVEL_0;
				SendMessage(CONTROL_MESSAGE);
			break;
		case BUTTON_NUM1:
			Brightness_Level[TypeOfLamp]=LEVEL_1;
				SendMessage(CONTROL_MESSAGE);
			break;
		case BUTTON_NUM2:
			Brightness_Level[TypeOfLamp]=LEVEL_2;
				SendMessage(CONTROL_MESSAGE);
			break;
		case BUTTON_NUM3:
			Brightness_Level[TypeOfLamp]=LEVEL_3;
				SendMessage(CONTROL_MESSAGE);
			break;
		case BUTTON_LEFT:
			if (TypeOfLamp==0) TypeOfLamp = NUMOFLAMPTYPE-1;
			else TypeOfLamp--;
			break;
		case BUTTON_RIGHT:
			if (TypeOfLamp==(NUMOFLAMPTYPE-1)) TypeOfLamp=0;
			else TypeOfLamp++;
			break;
		case BUTTON_UP:
			if (Brightness_Level[TypeOfLamp]<3) 
			{
				Brightness_Level[TypeOfLamp]++;
				SendMessage(CONTROL_MESSAGE);
			}
			break;
		case BUTTON_DOWN:
			if (Brightness_Level[TypeOfLamp]>0) 
			{
				Brightness_Level[TypeOfLamp]--;
				SendMessage(CONTROL_MESSAGE);
			}
			break;
	}
	//Send message to RF module
	*key_press=0;
}
//==============================================================================================================================
void Display_MainScreen(void)
{
	if (NewMessage_Flag)
	{
		NewMessage_Flag=0;
		CheckOnlineDevice();
		CountOnlineDevice();
	}
	LCD_write_charxy(0, 0, '<');
	LCD_write_charxy(78, 0, '>');
	LCD_write_string(0,1, "Brightness:");
	LCD_write_charxy(78, 1, Brightness_Level[TypeOfLamp]+'0');
	LCD_write_string(0,5, "Online: ");
	switch(TypeOfLamp)
	{
		case RUNWAY:
			LCD_write_string(20, 0, "Runway  ");
			toDecStr(str_buffer, numofonlinedevice[0]);
			LCD_write_string(65,5, str_buffer);
			break;
		case TAXIWAY:
			LCD_write_string(20, 0, "Taxiway ");
			toDecStr(str_buffer, numofonlinedevice[1]);
			LCD_write_string(65,5, str_buffer);
			break;
		case ENDTHRES:
			LCD_write_string(20, 0, "EndThres");
			toDecStr(str_buffer, numofonlinedevice[2]);
			LCD_write_string(65,5, str_buffer);
			break;
		case FLASHING:
			LCD_write_string(20, 0, "Flashing");
			toDecStr(str_buffer, numofonlinedevice[3]);
			LCD_write_string(65,5, str_buffer);
			break;
		case ALLLAMP:
			LCD_write_string(20, 0, "All Lamp");
			toDecStr(str_buffer, numofonlinedevice[4]);
			LCD_write_string(65,5, str_buffer);
			break;
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
	writeStringUART("AT02\n");
}
//==============================================================================================================================
void SendMessage(unsigned char TypeOfMessage)
{
	ClearOnlineTable();
	if (TypeOfMessage == CONTROL_MESSAGE)
	{
// 		LCD_write_string(0,4, "Sending...");
		writeStringUART("AT");
		toDecStr2(str_buffer, TypeOfMessage);					writeMStringUART(str_buffer);
		toDecStr2(str_buffer, TypeOfLamp);						writeMStringUART(str_buffer);
		toDecStr2(str_buffer, Brightness_Level[TypeOfLamp]);	writeMStringUART(str_buffer);
		writeStringUART("\n");
	}
}
//==============================================================================================================================
void ParseMessage(void)
{
	char Temp=0;
	if (queueLengthUART())
	{
		Temp = readUART();
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
