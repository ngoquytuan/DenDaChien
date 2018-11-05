/*
 * ALCS Remote.c
 *
 * Created: 8/3/2017 10:46:48 AM
 * Author : Angela Belnett
 */ 
//************************************************************************************************************
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "Const.h"
#include "Timming.h"
#include "UART.h"
#include "Control.h"
#include "MBSlave.h"
//************************************************************************************************************
char str_buffer[20];

unsigned char TypeOfLamp=RUNWAY, BrightnessLevel=LEVEL_0;
unsigned char TypeOfLampOld=0xFF, BrightnessLevelOld=0xFF;
unsigned char Multiwire_MODBUS_Mode = MODBUSMODE;
unsigned char Rx_BrightnessLevel_Old=0xFF, Rx_TypeOfLamp_Old=0xFF;
//************************************************************************************************************
void InitIO(void)
{
	//0000 1111 PA0: PA1: PA2: PA3:
	//			PA4: SRST_in PA5: SC_in PA6: SB_in PA7: SA_in
	DDRA = 0x0F;
	//1111 1111 PB0: PB1: PB2: PB3:
	//			PB4: PB5: PB6: PB7: 
	DDRB = 0xFF;
	//1111 1111	PC0: FB0 PC1: FB1 PC2: FB2 PC3: FB3
	//			PC4: FB4 PC5: PC6: PC7:
	DDRC = 0xFF;
	//1111 1010 PD0: RFAUX PD1: DE/RE PD2: RXD1 PD3: TXD1
	//			PD4: MD1 PD5: MD0 PD6: PD7:
	DDRD = 0xFA;
	//1111 1110 PE0: RXD0 PE1: TXD0 PE2: PE3:
	//			PE4: PE5: PE6: PE7: 
	DDRE = 0xFE;
	//1111 1111 PF0: PF1: PF2: PF3: 
	//			PF4: PF5: PF6: PF7:
	DDRF = 0xFF;
	//1111 1111 PG0: PG1: PG2: PG3: 
	//			PG4: PG5: PG6: PG7:
	DDRG = 0xFF;
	
	PORTC = 0x00;
	PORTD &= 0xCF;//RF Transceiver mode
}
//************************************************************************************************************
void init(void)
{
	cli();
	_delay_ms(100);
	InitIO();
	initTimer0();
	initTimer1();
	
	initUART();
	modbusSetAddress(CLIENTADDRESS);
	modbusInit();
		
	wdt_enable(WDTO_2S);
	sei();
}
//************************************************************************************************************
int main(void)
{
    init();
	/* Replace with your application code */
// 	writeStringUART1("Hanhyeu");
    while (1) 
    {
//============================================================================================================
		if (flag_1kHz)
		{
			flag_1kHz=0;
			
		}
//============================================================================================================
		if (flag_100Hz)
		{
			flag_100Hz=0;
			
		}
//============================================================================================================
		if (flag_10Hz)
		{
			flag_10Hz=0;
//------------------------------------------------------------------------------------------------------------
//Read multiwire signal in Multiwire mode			
			if (Multiwire_MODBUS_Mode==MULTIWIREMODE)
			{
				BrightnessLevel = Read_Brightness_Level();
				TypeOfLamp = Read_TypeOfLamp_To_Control();
				if ((BrightnessLevel!=BrightnessLevelOld)||(TypeOfLamp!=TypeOfLampOld))
				{
					BrightnessLevelOld=BrightnessLevel;
					TypeOfLampOld=TypeOfLamp;
					SendMessage(CONTROL_MESSAGE);
				}
			}
			else
			{
				if ((BrightnessLevel!=BrightnessLevelOld)||(TypeOfLamp!=TypeOfLampOld))
				{
					BrightnessLevelOld=BrightnessLevel;
					TypeOfLampOld=TypeOfLamp;
					SendMessage(CONTROL_MESSAGE);
				}
			}
//------------------------------------------------------------------------------------------------------------
//Send feedback control to multiwire relay
			Feedback_Control(BrightnessLevel, TypeOfLamp);
//------------------------------------------------------------------------------------------------------------
//MODBUS routine: Update data to MODBUS registers
			holdingRegisters[0] = BrightnessLevel;						//1 Brightness Level	- 2 Reserved
			holdingRegisters[1] = TypeOfLamp;							//3 Type Of Lamp		- 4 Reserved
			holdingRegisters[2] = Multiwire_MODBUS_Mode;				//5 Reserved			- 6 Reserved
			holdingRegisters[3] = 69;									//7 Reserved			- 8 Reserved
			holdingRegisters[4] = 96;									//9 Reserved			- 10 Reserved
			holdingRegisters[5] = 69;									//11 Reserved			- 12 Reserved
			holdingRegisters[6] = 96;									//13 Reserved			- 14 Reserved
			holdingRegisters[7] = 69;									//15 Reserved			- 16 Reserved
			holdingRegisters[8] = 96;									//17 Reserved			- 18 Reserved
			holdingRegisters[9] = 69;									//19 Reserved			- 20 Reserved
			modbusGet();
		}
//============================================================================================================
		if (flag_1Hz)
		{
			flag_1Hz=0;
			
		}
//============================================================================================================
    }
	return 0;
}
//************************************************************************************************************

