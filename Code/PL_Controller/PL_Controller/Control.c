/*
 * Control.c
 *
 * Created: 7/14/2017 8:57:40 AM
 *  Author: Angela Belnett
 */ 
#include "Control.h"
extern char str_buffer[20];
extern unsigned int DutyA, DutyB;
unsigned int DutyA_min[3]={0,0,0}, DutyA_max[3]={0,0,0}, DutyB_min[3]={0,0,0}, DutyB_max[3]={0,0,0};
unsigned int DutyA_avg[3]={0,0,0}, DutyB_avg[3]={0,0,0};
unsigned int LFA_new[3]={0,0,0}, LFA_old[3]={0,0,0}, LFB_new[3]={0,0,0}, LFB_old[3]={0,0,0};
extern unsigned char lamp_on, lamp_error_counter, lamp_error_status;
extern volatile unsigned int time_to_reset_data;

PortableLampStruct PortableLamp; 
unsigned char TypeOfLamp_tmp=0, Brightness_tmp=0;
unsigned char count_adc=0;
unsigned int Temp_ADC[NUM_OF_ADC_CHANNEL][AVERAGE_NUM];
unsigned char Charging_status=0, Charging_old_status=0;
unsigned char flag_ok=0;

const char HEADER[HEADER_LENGTH] = "AT";
volatile unsigned char pos=0, datapos=0;
volatile unsigned char header_ok=0;
volatile unsigned char NewMessage_Flag=0;
volatile unsigned int time_to_response=0;
char buffer[20];
unsigned char old_brightness_level=0, brightness_level_changed=0;
unsigned char old_localremote_mode=0, localremote_changed=0;
//-----------------------------------------------------------------------------------
void initLamp_Parameter(void)
{
	PortableLamp.TypeOfLamp = DEFAULTTYPEOFLAMP;
	PortableLamp.Brightness_level = MED_LEVEL_SELECTED;
	PortableLamp.LocalRemote_Mode = LOCAL_MODE_SELECTED;
}
//-----------------------------------------------------------------------------------
unsigned char Read_Brighness_Level(void)
{
	if (!HIGH_LEVEL) return HIGH_LEVEL_SELECTED;
	else if (!LOW_LEVEL) return LOW_LEVEL_SELECTED;
	else return MED_LEVEL_SELECTED;
}
//-----------------------------------------------------------------------------------
unsigned char Read_LocalRemote_Mode(void)
{
	if (!REMOTE_MODE) return REMOTE_MODE_SELECTED;
	else if (!LOCAL_MODE) return LOCAL_MODE_SELECTED;
	else return TEST_MODE_SELECTED;
}
//-----------------------------------------------------------------------------------
void Read_ADC_Channel(unsigned int *V_BATTERY, unsigned int *V_CHARGER, unsigned int *I_CHARGER,
						unsigned int *LAMP_CURRENT1, unsigned int *LAMP_CURRENT2)
{
	unsigned char i=0, j=0;
	unsigned int sum[NUM_OF_ADC_CHANNEL]={0,0,0,0,0};
	unsigned int U_Ch=0, U_Ba=0, I_Ch=0, L_Cr1=0, L_Cr2=0;
	
	for (j=0; j<NUM_OF_ADC_CHANNEL; j++)
	{	
		for (i=AVERAGE_NUM-1; i>0; i--)
		{
			Temp_ADC[j][i] = Temp_ADC[j][i-1];
			sum[j]+=Temp_ADC[j][i];
		}
		Temp_ADC[j][0] = ADC_Data[j];
		sum[j]+=Temp_ADC[j][0];	
	}
	U_Ch = sum[0]/AVERAGE_NUM;
	U_Ch = (float) U_Ch*1.6084 + 1.4641 + CHARGER_OFFSET_VOLTAGE;
	
	U_Ba = sum[1]/AVERAGE_NUM;
	if (U_Ba>0) U_Ba = (float) U_Ba*1.6125 - 8.6517 + BATTERY_OFFSET_VOLTAGE;
	
	I_Ch = sum[2]/AVERAGE_NUM;
	if (I_Ch>=BATTERY_CURRENT_MIN) I_Ch = (float)I_Ch*5.2307 + 62.331;
	
	L_Cr1 = sum[3]/AVERAGE_NUM;
	if (L_Cr1>=LAMP_CURRENT_MIN) L_Cr1 = (float) L_Cr1*1.1179 + 74.218;
//	else L_Cr1=0;
	L_Cr2 = sum[4]/AVERAGE_NUM;
	if (L_Cr2>=LAMP_CURRENT_MIN) L_Cr2 = (float) L_Cr2*1.1179 + 74.218;
//	else L_Cr2=0;
// 	L_Tp = sum[4]/AVERAGE_NUM;
	U_Ba = (float)U_Ba - (float)I_Ch*0.047;
	
	*V_CHARGER = U_Ch;
	*V_BATTERY = U_Ba;
	*I_CHARGER = I_Ch;
	*LAMP_CURRENT1 = L_Cr1;
	*LAMP_CURRENT2 = L_Cr2;
}
//-----------------------------------------------------------------------------------
void Battery_Voltage_Indicate(unsigned char Display_Enable)
{
	if (Display_Enable)
	{
		if (Battery_voltage>=BATTERY_VOLTAGE_LOW)
		{
			LOWBATTERY_LED_ON;
			if (Battery_voltage>=BATTERY_VOLTAGE_MID)
			{
				MIDBATTERY_LED_ON;
				if (Battery_voltage>=BATTERY_VOLTAGE_HIGH)
				{
					HIGHBATTERY_LED_ON;
					if (Battery_voltage>=BATTERY_VOLTAGE_FULL) {FULLBATTERY_LED_ON;}
					else {FULLBATTERY_LED_BLINK;}
				}
				else {HIGHBATTERY_LED_BLINK;FULLBATTERY_LED_OFF;}
			}
			else {MIDBATTERY_LED_BLINK;HIGHBATTERY_LED_OFF;FULLBATTERY_LED_OFF;}
		}
		else {LOWBATTERY_LED_BLINK;MIDBATTERY_LED_OFF;HIGHBATTERY_LED_OFF;FULLBATTERY_LED_OFF;}
	}
	else
	{
		LOWBATTERY_LED_OFF;
		MIDBATTERY_LED_OFF;
		HIGHBATTERY_LED_OFF;
		FULLBATTERY_LED_OFF;
	}
}
//-----------------------------------------------------------------------------------
void Battery_Charger_Control(void)
{
	if (Charger_current==0)
	{
		if ((Charger_voltage>=CHARGER_VOLTAGE_MIN)&&(Charger_voltage<=CHARGER_VOLTAGE_MAX))
		{
			if (Battery_voltage<=BATTERY_VOLTAGE_FULL-BATTERY_VOLTAGE_CHARGE_ON) CHARGER_ENABLE;
		}
		else
		{
			CHARGER_DISABLE;
		}
	}
	else
	{
		if (Battery_voltage>=BATTERY_VOLTAGE_FULL+BATTERY_VOLTAGE_CHARGE_OFF) CHARGER_DISABLE;
	}
}
//-----------------------------------------------------------------------------------
void Lamp_Brightness_Control(void)
{
	unsigned char Message_Type=0;

	PortableLamp.LocalRemote_Mode = Read_LocalRemote_Mode();
	if (PortableLamp.LocalRemote_Mode != old_localremote_mode)
	{
		old_localremote_mode = PortableLamp.LocalRemote_Mode;
		PortableLamp.Brightness_level=0;
		localremote_changed=1;
		lamp_on=0;
		lamp_error_counter=0;
		lamp_error_status=0;
		Reset_Duty_Min_Max();
	}
	switch (PortableLamp.LocalRemote_Mode)
	{
//=======================================================================================================================		
		case LOCAL_MODE_SELECTED:
			LOCAL_REMOTE_LED_OFF;
			PortableLamp.Brightness_level = Read_Brighness_Level();
			if (PortableLamp.Brightness_level != old_brightness_level) 
			{
				old_brightness_level = PortableLamp.Brightness_level;
				brightness_level_changed=1;
				lamp_on=0;
				lamp_error_counter=0;
				lamp_error_status=0;
				Reset_Duty_Min_Max();
			}
			if (Battery_voltage<=BATTERY_VOLTAGE_LOW-BATTERY_VOLTAGE_DELTA_OFF)
			{
				Battery_Voltage_Indicate(1);
				StartStop_PWM(0,0);
				PortableLamp.Brightness_level=0;
			}
			else if (Battery_voltage>=BATTERY_VOLTAGE_LOW+BATTERY_VOLTAGE_DELTA_ON)
			{
				switch (PortableLamp.Brightness_level)
				{
					case HIGH_LEVEL_SELECTED:
						if (PortableLamp.TypeOfLamp!=FLASHING) StartStop_PWM(HIGH_LEVEL_BRIGHTNESSA,HIGH_LEVEL_BRIGHTNESSB);
						else
							if (flag_flashing) {flag_flashing=0;StartStop_PWM(HIGH_LEVEL_BRIGHTNESSA,HIGH_LEVEL_BRIGHTNESSB);}
							else StartStop_PWM(0,0);
					break;
					case MED_LEVEL_SELECTED:
						if (PortableLamp.TypeOfLamp!=FLASHING) StartStop_PWM(MED_LEVEL_BRIGHTNESSA,MED_LEVEL_BRIGHTNESSB);
						else
							if (flag_flashing) {flag_flashing=0;StartStop_PWM(MED_LEVEL_BRIGHTNESSA,MED_LEVEL_BRIGHTNESSB);}
							else StartStop_PWM(0,0);
					break;
					case LOW_LEVEL_SELECTED:
						if (PortableLamp.TypeOfLamp!=FLASHING) StartStop_PWM(LOW_LEVEL_BRIGHTNESSA,LOW_LEVEL_BRIGHTNESSB);
						else
							if (flag_flashing) {flag_flashing=0;StartStop_PWM(LOW_LEVEL_BRIGHTNESSA,LOW_LEVEL_BRIGHTNESSB);}
							else StartStop_PWM(0,0);
					break;
				}
			}
			break;
//=======================================================================================================================		
		case REMOTE_MODE_SELECTED:
			LOCAL_REMOTE_LED_ON;
//==============================================================================================================
			if (NewMessage_Flag)
			{
				NewMessage_Flag=0;
				LOCAL_REMOTE_LED_BLINK;
				Message_Type = (buffer[0]-'0')*10 + (buffer[1]-'0');
				if (Message_Type==CONTROL_MESSAGE)
				{
					TypeOfLamp_tmp = (buffer[TYPEOFLAMP_POS]-'0')*10 + (buffer[TYPEOFLAMP_POS+1]-'0');
					if ((TypeOfLamp_tmp == PortableLamp.TypeOfLamp)||(TypeOfLamp_tmp == ALLLAMP))
					{
						PortableLamp.Brightness_level = (buffer[BRIGHTNESS_LEVEL_POS]-'0')*10 + (buffer[BRIGHTNESS_LEVEL_POS+1]-'0');
						if (PortableLamp.Brightness_level != old_brightness_level)
						{
							old_brightness_level = PortableLamp.Brightness_level;
							brightness_level_changed=1;
							lamp_on=0;
							lamp_error_counter=0;
							lamp_error_status=0;
							Reset_Duty_Min_Max();
						}
						time_to_response=PortableLamp.TypeOfLamp*1000 + DEVICE_ID*500;
// 						Send_Response_Message();
					}
				}	
			}
//==============================================================================================================
			if (Battery_voltage<=BATTERY_VOLTAGE_LOW-BATTERY_VOLTAGE_DELTA_OFF)
			{
				Battery_Voltage_Indicate(1);
				StartStop_PWM(0,0);
				PortableLamp.Brightness_level=0;
			}
			else if (Battery_voltage>=BATTERY_VOLTAGE_LOW+BATTERY_VOLTAGE_DELTA_ON)
			{
				if ((TypeOfLamp_tmp == PortableLamp.TypeOfLamp)||(TypeOfLamp_tmp == ALLLAMP))
 				{
					switch(PortableLamp.Brightness_level)
					{
						case OFF_LEVEL_SELECTED:
							StartStop_PWM(0,0);
							break;
						case HIGH_LEVEL_SELECTED:
							if (PortableLamp.TypeOfLamp==FLASHING)
							{
								if (flag_flashing) {flag_flashing=0;StartStop_PWM(HIGH_LEVEL_BRIGHTNESSA, HIGH_LEVEL_BRIGHTNESSB);}
								else StartStop_PWM(0,0);
								flag_ok=1;
							}
							else
							{
								StartStop_PWM(HIGH_LEVEL_BRIGHTNESSA, HIGH_LEVEL_BRIGHTNESSB);
								flag_ok=1;
							}
							break;
						case MED_LEVEL_SELECTED:
							if (PortableLamp.TypeOfLamp==FLASHING)
							{
								if (flag_flashing) {flag_flashing=0;StartStop_PWM(MED_LEVEL_BRIGHTNESSA, MED_LEVEL_BRIGHTNESSB);}
								else StartStop_PWM(0,0);
								flag_ok=1;
							}
							else
							{
								StartStop_PWM(MED_LEVEL_BRIGHTNESSA, MED_LEVEL_BRIGHTNESSB);
								flag_ok=1;
							}
							break;
						case LOW_LEVEL_SELECTED:
							if (PortableLamp.TypeOfLamp==FLASHING)
							{
								if (flag_flashing) {flag_flashing=0;StartStop_PWM(LOW_LEVEL_BRIGHTNESSA, LOW_LEVEL_BRIGHTNESSB);}
								else StartStop_PWM(0,0);
								flag_ok=1;
							}
							else
							{
								StartStop_PWM(LOW_LEVEL_BRIGHTNESSA, LOW_LEVEL_BRIGHTNESSB);
								flag_ok=1;
							}
							break;
						}
					}
				}
				break;
//=======================================================================================================================	
		case TEST_MODE_SELECTED:
// 			LOCAL_REMOTE_LED_BLINK;
			StartStop_PWM(0,0);
			break;
	}
}
//=======================================================================================================================
void Reset_Duty_Min_Max(void)
{
	unsigned char i=0;
	
	for (i=0; i<3; i++)
	{
		DutyA_max[i]=0;
		DutyA_min[i]=0;
		DutyA_avg[i]=0;
		DutyB_max[i]=0;
		DutyB_min[i]=0;
		DutyB_avg[i]=0;
	}
}
//=======================================================================================================================
unsigned char CheckErrorlamp(void)
{
	unsigned char lamp_error=0;
	if (PortableLamp.Brightness_level==HIGH_LEVEL_SELECTED)
	{
		if (DutyA>DutyA_max[2]) DutyA_max[2]=DutyA;
		if ((DutyA_min[2]==0)||(DutyA<DutyA_min[2])) DutyA_min[2]=DutyA;
		if (DutyB>DutyB_max[2]) DutyB_max[2]=DutyB;
		if ((DutyB_min[2]==0)||(DutyB<DutyB_min[2])) DutyB_min[2]=DutyB;
		DutyA_avg[2] = (DutyA_max[2] + DutyA_min[2])/2;
		DutyB_avg[2] = (DutyB_max[2] + DutyB_min[2])/2;
		if ((DutyA_max[2]-DutyA_min[2])>=(DutyA_avg[2]*10/130)) lamp_error=1;//10
		if ((DutyB_max[2]-DutyB_min[2])>=(DutyB_avg[2]*10/130)) lamp_error=1;//10
	}
	if (PortableLamp.Brightness_level==MED_LEVEL_SELECTED)
	{
		if (DutyA>DutyA_max[1]) DutyA_max[1]=DutyA;
		if ((DutyA_min[1]==0)||(DutyA<DutyA_min[1])) DutyA_min[1]=DutyA;
		if (DutyB>DutyB_max[1]) DutyB_max[1]=DutyB;
		if ((DutyB_min[1]==0)||(DutyB<DutyB_min[1])) DutyB_min[1]=DutyB;
		DutyA_avg[1] = (DutyA_max[1] + DutyA_min[1])/2;
		DutyB_avg[1] = (DutyB_max[1] + DutyB_min[1])/2;
		if ((DutyA_max[1]-DutyA_min[1])>=(DutyA_avg[1]*10/100)) lamp_error=1;//10
		if ((DutyB_max[1]-DutyB_min[1])>=(DutyB_avg[1]*10/100)) lamp_error=1;//10
	}
	if (PortableLamp.Brightness_level==LOW_LEVEL_SELECTED)
	{
		if (DutyA>DutyA_max[0]) DutyA_max[0]=DutyA;
		if ((DutyA_min[0]==0)||(DutyA<DutyA_min[0])) DutyA_min[0]=DutyA;
		if (DutyB>DutyB_max[0]) DutyB_max[0]=DutyB;
		if ((DutyB_min[0]==0)||(DutyB<DutyB_min[0])) DutyB_min[0]=DutyB;
		DutyA_avg[0] = (DutyA_max[0] + DutyA_min[0])/2;
		DutyB_avg[0] = (DutyB_max[0] + DutyB_min[0])/2;
		if ((DutyA_max[0]-DutyA_min[0])>=(DutyA_avg[0]*10/90)) lamp_error=1;//10
		if ((DutyB_max[0]-DutyB_min[0])>=(DutyB_avg[0]*10/90)) lamp_error=1;//10
	}
	return lamp_error;
}
void ErrorLampIndicate(void)
{
	if (time_to_reset_data>1)
	{
// 	writeUART('>');
	if (DutyA_avg[2])
	{
// 		writeUART('(');
		LFA_new[2] = DutyA_avg[2]*10/(DutyA_max[2] - DutyA_min[2]);
		if (LFA_old[2]==0) LFA_old[2] = LFA_new[2];
		if ((LFA_new[2]<250)&&(LFA_old[2] - LFA_new[2])>=80) lamp_error_status=1;
		else LFA_old[2]=LFA_new[2];
// 		toDecStr4(str_buffer, LFA_new[2]);
// 		writeMStringUART(str_buffer);
// 		writeUART(')');
	}
	
	if (DutyB_avg[2])
	{
// 		writeUART('(');
		LFB_new[2] = DutyB_avg[2]*10/(DutyB_max[2] - DutyB_min[2]);
		if (LFB_old[2]==0) LFB_old[2] = LFB_new[2];
		if ((LFB_new[2]<250)&&(LFB_old[2] - LFB_new[2])>=80) lamp_error_status=1;
		else LFB_old[2]=LFB_new[2];
// 		toDecStr4(str_buffer, LFB_new[2]);
// 		writeMStringUART(str_buffer);
// 		writeUART(')');
	}
	
	if (DutyA_avg[1])
	{
// 		writeUART('(');
		LFA_new[1] = DutyA_avg[1]*10/(DutyA_max[1] - DutyA_min[1]);
		if (LFA_old[1]==0) LFA_old[1] = LFA_new[1];
		if ((LFA_new[1]<250)&&(LFA_old[1] - LFA_new[1])>=80) lamp_error_status=1;
		else LFA_old[1]=LFA_new[1];
// 		toDecStr4(str_buffer, LFA_new[1]);
// 		writeMStringUART(str_buffer);
// 		writeUART(')');
	}
	
	if (DutyB_avg[1])
	{
// 		writeUART('(');
		LFB_new[1] = DutyB_avg[1]*10/(DutyB_max[1] - DutyB_min[1]);
		if (LFB_old[1]==0) LFB_old[1] = LFB_new[1];
		if ((LFB_new[1]<250)&&(LFB_old[1] - LFB_new[1])>=80) lamp_error_status=1;
		else LFB_old[1]=LFB_new[1];
// 		toDecStr4(str_buffer, LFB_new[1]);
// 		writeMStringUART(str_buffer);
// 		writeUART(')');
	}
	
	if (DutyA_avg[0])
	{
// 		writeUART('(');
		LFA_new[0] = DutyA_avg[0]*10/(DutyA_max[0] - DutyA_min[0]);
		if (LFA_old[0]==0) LFA_old[0] = LFA_new[0];
		if ((LFA_new[0]<250)&&(LFA_old[0] - LFA_new[0])>=80) lamp_error_status=1;
		else LFA_old[0]=LFA_new[0];
// 		toDecStr4(str_buffer, LFA_new[0]);
// 		writeMStringUART(str_buffer);
// 		writeUART(')');
	}
	
	if (DutyB_avg[0])
	{
// 		writeUART('(');
		LFB_new[0] = DutyB_avg[0]*10/(DutyB_max[0] - DutyB_min[0]);
		if (LFB_old[0]==0) LFB_old[0] = LFB_new[0];
		if ((LFB_new[0]<250)&&(LFB_old[0] - LFB_new[0])>=80) lamp_error_status=1;
		else LFB_old[0]=LFB_new[0];
// 		toDecStr4(str_buffer, LFB_new[0]);
// 		writeMStringUART(str_buffer);
// 		writeUART(')');
	}
// 	writeUART('\r');writeUART('\n');
	}
}
//=======================================================================================================================
void ParseMessage(void)
{
	char Temp=0;
	if (queueLengthUART())
	{
		Temp = readUART();
		if (Temp=='\n')
		{
			if (NewMessage_Flag==0) NewMessage_Flag=1;
			header_ok=0;
			datapos=0;
			pos=0;
			flushUARTRX();
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
			}
		}
	}	
}
//=======================================================================================================================
void Send_Response_Message(void)
{
	writeStringUART("AT01");
	toDecStr2(str_buffer, PortableLamp.TypeOfLamp);
	writeMStringUART(str_buffer);
	toDecStr3(str_buffer, DEVICE_ID);
	writeMStringUART(str_buffer);
	writeUART('\n');
}
//=======================================================================================================================




