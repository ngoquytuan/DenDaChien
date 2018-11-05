/*
 * PL_Controller.c
 *
 * Created: 7/13/2017 9:06:04 AM
 * Author : Angela Belnett
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "Const.h"
#include "Control.h"
#include "Timming.h"
#include "UART.h"
//************************************************************************************************************
// #define DEBUG_MODE
#ifdef DEBUG_MODE
// 	#define DEBUG_LAMP_STATUS
// 	#define DEBUG_VOLTAGE
// 	#define DEBUG_DUTY_CURRENT
// 	#define DEBUG_LAMP_FAIL
#endif
//************************************************************************************************************
extern unsigned int DutyA, DutyB;
extern unsigned int DutyA_min[3], DutyA_max[3], DutyA_avg[3], DutyB_min[3], DutyB_max[3], DutyB_avg[3];
extern unsigned int LFA_new[3], LFA_old[3], LFB_new[3], LFB_old[3];
extern unsigned char brightness_level_changed, localremote_changed;
// extern volatile signed int e_A[PID_ERROR_NUMBER], e_B[PID_ERROR_NUMBER];
char str_buffer[20];
unsigned int Battery_voltage=0, Charger_voltage=0, Charger_current=0, Lamp_current1=0, Lamp_current2=0;
unsigned char lamp_error_counter=0;
unsigned char lamp_on=0, lamp_error_status=0;
//************************************************************************************************************
void initIO(void)
{
	//1111 1111 PB0: , PB1: PWM_LAMP1, PB2: PWM_LAMP2, PB3: MD0
	//PB4: MD1, PB5: , PB6: REMOTELED, PB7: FULLBATTERY_LED
	DDRB = 0xFF;
	//0000 0000  PC0: V_CHARGE_ADC, PC1: V_BATTERY_ADC, PC2: I_CHARGE_ADC, PC3: LOCAL_MODE,
	//PC4: REMOTE_MODE, PC5: HIGH_LEVEL, PC6: CURRENT_ADC1, PC7: CURRENT_ADC2
	DDRC = 0x00;
	//1111 0010 PD0: RXD, PD1: TXD, PD2: LOW_LEVEL, PD3: RFAUX_IN,
	//PD4: CHARGE_ENABLE, PD5: LOWBATTERY_LED, PD6: MIDBATTERY_LED, PD7: HIGHBATTERY_LED
	DDRD = 0xF2;
	
	PORTD &= 0x0F;//0000 1111
	PORTB &= 0x00;//0000 0000
}
//************************************************************************************************************
void init(void)
{
	cli();
	_delay_ms(100);
	initIO();
	initTimer0();
	initTimer1();
	init_ADC();
	initUART();
	sei();
	
	initLamp_Parameter();	
}
//************************************************************************************************************
int main(void)
{
	init();
    /* Replace with your application code */
    while (1) 
    {
//===================================================================================================================
		if (flag_1kHz)
		{
			flag_1kHz = 0;
			Read_ADC_Channel(&Battery_voltage, &Charger_voltage, &Charger_current, &Lamp_current1, &Lamp_current2);
		}//1kHz
//===================================================================================================================
		if (flag_100Hz)
		{
			flag_100Hz = 0;			
			ParseMessage();
			
			
			if (lamp_error_status==0) Lamp_Brightness_Control();
			else StartStop_PWM(0, 0);
		}//100Hz
//===================================================================================================================
		if (flag_10Hz)
		{
			flag_10Hz = 0;
			
			if (lamp_on>=10)
			{
				if (CheckErrorlamp()) lamp_error_counter++;
				if (lamp_error_counter>20) 
				{
					lamp_error_counter=20;
					LOCAL_REMOTE_LED_BLINK;
// 					lamp_error_status=1;
				}
			}
		}//10Hz
//===================================================================================================================
		if (flag_1Hz)
		{
			flag_1Hz=0;
			if ((brightness_level_changed)||(localremote_changed)) 
			{
				lamp_on++;
				if (lamp_on>10) 
				{
					lamp_on=10;
					brightness_level_changed=0;
					localremote_changed=0;
				}
			}	
// 			LOCAL_REMOTE_LED_BLINK;			
			Battery_Charger_Control();
			
			if (Charger_current) Battery_Voltage_Indicate(1);
			else Battery_Voltage_Indicate(0);
			
			ErrorLampIndicate();	
#ifdef DEBUG_MODE
			PORTB |= 0x18;//0000 000
#ifdef DEBUG_LAMP_STATUS
			writeUART('R');
			toDecStr4(str_buffer, PortableLamp.LocalRemote_Mode);
			writeMStringUART(str_buffer);
			writeUART('\r');writeUART('\n');
			writeUART('B');
			toDecStr4(str_buffer, PortableLamp.Brightness_level);
			writeMStringUART(str_buffer);
			writeUART('\r');writeUART('\n');
			writeUART('T');
			toDecStr4(str_buffer, PortableLamp.TypeOfLamp);
			writeMStringUART(str_buffer);
			writeUART('\r');writeUART('\n');
#endif			
#ifdef DEBUG_VOLTAGE
			writeUART('B');
			toDecStr4(str_buffer, Battery_voltage);
			writeMStringUART(str_buffer);
			writeUART('C');
			toDecStr4(str_buffer, Charger_voltage);
			writeMStringUART(str_buffer);
			writeUART('\r');writeUART('\n');
#endif			
#ifdef DEBUG_DUTY_CURRENT
			writeUART('A');	
			toDecStr4(str_buffer, DutyA);
			writeMStringUART(str_buffer);
			writeUART('-');
			toDecStr4(str_buffer, Lamp_current1);
			writeMStringUART(str_buffer);
			writeUART('\r');writeUART('\n');
			writeUART('B');
			toDecStr4(str_buffer, DutyB);
			writeMStringUART(str_buffer);
			writeUART('-');
			toDecStr4(str_buffer, Lamp_current2);
			writeMStringUART(str_buffer);
			writeUART('\r');writeUART('\n');
#endif
#ifdef DEBUG_LAMP_FAIL
			writeUART('>');
			if (DutyA_avg[2]) 
			{
				writeUART('(');
				LFA_new[2] = DutyA_avg[2]*10/(DutyA_max[2] - DutyA_min[2]);
				if (LFA_old[2]==0) LFA_old[2] = LFA_new[2];
				if ((LFA_new[2]<250)&&(LFA_old[2] - LFA_new[2])>=25) lamp_error_status=1;
				else LFA_old[2]=LFA_new[2];
				toDecStr4(str_buffer, LFA_new[2]);
				writeMStringUART(str_buffer);
				writeUART(')');
			}
			
			if (DutyB_avg[2]) 
			{
				writeUART('(');
				LFB_new[2] = DutyB_avg[2]*10/(DutyB_max[2] - DutyB_min[2]);
				if (LFB_old[2]==0) LFB_old[2] = LFB_new[2];
				if ((LFB_new[2]<250)&&(LFB_old[2] - LFB_new[2])>=25) lamp_error_status=1;
				else LFB_old[2]=LFB_new[2];
				toDecStr4(str_buffer, LFB_new[2]);
				writeMStringUART(str_buffer);
				writeUART(')');		
			}
			
			if (DutyA_avg[1]) 
			{
				writeUART('(');
				LFA_new[1] = DutyA_avg[1]*10/(DutyA_max[1] - DutyA_min[1]);
				if (LFA_old[1]==0) LFA_old[1] = LFA_new[1];
				if ((LFA_new[1]<250)&&(LFA_old[1] - LFA_new[1])>=25) lamp_error_status=1;
				else LFA_old[1]=LFA_new[1];
				toDecStr4(str_buffer, LFA_new[1]);
				writeMStringUART(str_buffer);
				writeUART(')');
			}
			
			if (DutyB_avg[1]) 
			{
				writeUART('(');
				LFB_new[1] = DutyB_avg[1]*10/(DutyB_max[1] - DutyB_min[1]);
				if (LFB_old[1]==0) LFB_old[1] = LFB_new[1];
				if ((LFB_new[1]<250)&&(LFB_old[1] - LFB_new[1])>=25) lamp_error_status=1;
				else LFB_old[1]=LFB_new[1];
				toDecStr4(str_buffer, LFB_new[1]);
				writeMStringUART(str_buffer);
				writeUART(')');
			}
			
			if (DutyA_avg[0]) 
			{
				writeUART('(');
				LFA_new[0] = DutyA_avg[0]*10/(DutyA_max[0] - DutyA_min[0]);
				if (LFA_old[0]==0) LFA_old[0] = LFA_new[0];
				if ((LFA_new[0]<250)&&(LFA_old[0] - LFA_new[0])>=25) lamp_error_status=1;
				else LFA_old[0]=LFA_new[0];
				toDecStr4(str_buffer, LFA_new[0]);
				writeMStringUART(str_buffer);
				writeUART(')');
			}
			
			if (DutyB_avg[0]) 
			{
				writeUART('(');
				LFB_new[0] = DutyB_avg[0]*10/(DutyB_max[0] - DutyB_min[0]);
				if (LFB_old[0]==0) LFB_old[0] = LFB_new[0];
				if ((LFB_new[0]<250)&&(LFB_old[0] - LFB_new[0])>=25) lamp_error_status=1;
				else LFB_old[0]=LFB_new[0];
				toDecStr4(str_buffer, LFB_new[0]);
				writeMStringUART(str_buffer);
				writeUART(')');
			}
			writeUART('\r');writeUART('\n');
#endif				
			PORTB &= 0xE7;//1110 0111
#endif	
		}//1Hz
    }
	return 0;
}
//************************************************************************************************************





























// 			if(gb_ModuleWorkEnableFlag)
// 			{
// 				if (Tx_request)
// 				{
// 					if (gb_SystemMode==C_SysMode_LoRaRx)
// 					{
// 						if (gb_ParameterChangeFlag==0)
// 						{
// 							gb_SystemMode=C_SysMode_LoRaTx;
// 							gb_ParameterChangeFlag=1;
// 						}
// 					}
// 				}
// 				if (gb_SysTimer_1S==0)
// 				{
// 					if (gb_SystemMode==C_SysMode_LoRaTx)
// 					{
// 						if (gb_ParameterChangeFlag==0)
// 						{
// 							gb_SystemMode=C_SysMode_LoRaRx;
// 							gb_ParameterChangeFlag=1;
// 						}
// 					}
// 				}
// 				if(TxFlag==1)                                        //Sent successfully
// 				{
// 					TxFlag=0;
// 					Tx_request=0;
// 					gb_SysTimer_1S=3;
// 					gw_SendDataCount++;
// 					if(gw_SendDataCount>9999){gw_SendDataCount=0;}
// 				}
//
// 				if(RxFlag==1)                                        //Successfully received
// 				{
// 					RxFlag=0;
// 					Tx_request=1;
//  					RFM98Data[BRIGHTNESS_LEVEL] = Brightness_level+'0';
// 					gw_ReceiveDataCount++;
// 					if(gw_ReceiveDataCount>9999){gw_ReceiveDataCount=0;}
// 				}
// 				RFM98_Running(gb_SystemMode,gb_ModuleWorkMode,gb_ParameterChangeFlag,&TxFlag,&RxFlag,&gb_RF_RSSI);
//  				if(gb_ParameterChangeFlag==1){gb_ParameterChangeFlag=0;}    //clear parameter flag
// 				if(gb_ModuleWorkMode!=C_ModuleWorkMode_FSK && gb_ModuleWorkMode!=C_ModuleWorkMode_OOK && gb_ModuleWorkMode!=C_ModuleWorkMode_LoRa)
// 				{
// 					gb_StatusTx=0;
// 					gb_StatusRx=0;
// 				}
// 			}
// 			else
// 			{
// 				gb_StatusTx=0;
// 				gb_StatusRx=0;
// 			}




// 		LOCAL_REMOTE_LED_ON;
// 		FULLBATTERY_LED_ON;
// 		HIGHBATTERY_LED_ON;
// 		MIDBATTERY_LED_ON;
// 		LOWBATTERY_LED_ON;
// 		UPPER_LAMP_ON;
//		_delay_ms(1);
// 		LOCAL_REMOTE_LED_OFF;
// 		FULLBATTERY_LED_OFF;
// 		HIGHBATTERY_LED_OFF;
// 		MIDBATTERY_LED_OFF;
// 		LOWBATTERY_LED_OFF;
// 		UPPER_LAMP_OFF;
// 		_delay_ms(10);
// 		FULLBATTERY_LED_BLINK;

// 			toStr(str_buffer, Battery_voltage);writeStringUART("\r\n");writeMStringUART(str_buffer);
// 			toStr(str_buffer, Charger_voltage);writeStringUART(" ");writeMStringUART(str_buffer);
// 			toStr(str_buffer, Charger_current);writeStringUART(" ");writeMStringUART(str_buffer);
// 			toStr(str_buffer, Lamp_current);writeStringUART(" ");writeMStringUART(str_buffer);
// 			toStr(str_buffer, Lamp_temperature);writeStringUART(" ");writeMStringUART(str_buffer);
// 			toStr(str_buffer, temp_rf);writeStringUART(" ");writeMStringUART(str_buffer);







// 				toStr(str_buffer, gb_SystemMode);writeStringUART(" ");writeMStringUART(str_buffer);
// 				toStr(str_buffer, gb_ModuleWorkMode);writeStringUART(" ");writeMStringUART(str_buffer);
// 				toStr(str_buffer, gb_ParameterChangeFlag);writeStringUART(" ");writeMStringUART(str_buffer);
//			toStr(str_buffer, TxFlag);writeStringUART(" ");writeMStringUART(str_buffer);
//			toStr(str_buffer, RxFlag);writeStringUART(" ");writeMStringUART(str_buffer);
//			toStr(str_buffer, Tx_request);writeStringUART(" ");writeMStringUART(str_buffer);
//			toStr(str_buffer, gb_RF_RSSI);writeStringUART(" ");writeMStringUART(str_buffer);
// 				toStr(str_buffer, gb_SysTimer_1S);writeStringUART(" ");writeMStringUART(str_buffer);
// 				writeStringUART("S ");
//
//			toStr(str_buffer, gb_SystemMode);writeStringUART(" ");writeMStringUART(str_buffer);writeStringUART(" ");




