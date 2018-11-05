/*
 * PL_REMOTE.c
 *
 * Created: 3/28/2017 2:31:14 PM
 *  Author: Angela Belnett
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>

#include "Const.h"
#include "Control.h"
#include "Timming.h"
#include "UART.h"
#include "LCD5110.h"
//************************************************************************************************************
char str_buffer[20];
unsigned char screen=MAIN_SCREEN;
unsigned char TypeOfLamp=ALLLAMP, Brightness_Level[NUMOFLAMPTYPE]={LEVEL_0};
//************************************************************************************************************
void InitIO(void)
{
	//1110 1111 PB0: LCD_RESET, PB1: LCD_SCE, PB2: LCD_DC, PB3: MOSI
	//PB4: MISO, PB5: SCK, PB6: MD0, PB7: MD1
	DDRB = 0xEF;
	//0010 0000  PC0: ROW1, PC1: ROW2, PC2: ROW3, PC3: ROW4,
	//PC4: ROW5, PC5: , PC6: , PC7:
	//PORTC &= 0xE0; 
	DDRC = 0x20;
	//1111 0010 PD0: RXD, PD1: TXD, PD2: BUTTON11, PD3: RFAUX,
	//PD4: COL1, PD5: COL2, PD6: COL3, PD7: LCD_LED
	DDRD = 0xF2;
	
// 	PORTD|=(1<<4);
 	PORTB&=0x3F;
}
//************************************************************************************************************
void init(void)
{
	cli();
	_delay_ms(100);
	InitIO();
	initTimer0();
	
	LCD_init();
	LCD_clear();
	LCD_set_contrast(LCD_CONTRAST);
		
	initUART();
	wdt_enable(WDTO_2S);
	sei();
}
//************************************************************************************************************
int main(void)
{
	init();

    while(1)
    {
//TODO:: Please write your application code 
//===================================================================================================================
		if (flag_1kHz)
		{
			flag_1kHz = 0;
			
		}//1kHz
//===================================================================================================================
		if (flag_100Hz)
		{
			flag_100Hz = 0;
			ParseMessage();
			if (button_pressed) screen_timeout = LCD_BACKLIGHT_TIMEOUT;
			switch (screen)
			{
				case MAIN_SCREEN:
					Brightness_screen(&button_pressed);
					break;
			}
		}//100Hz
//===================================================================================================================
		if (flag_10Hz)
		{
			flag_10Hz = 0;
			Display_MainScreen();
		}//10Hz		
//===================================================================================================================
		if (flag_1Hz)
		{
			flag_1Hz = 0;
// 			writeStringUART("Hello! Hanhyeu");
// 			LCD_BACKLIGHT_BLINK;
		}//1Hz
    }//while
//===================================================================================================================	
	return 1;
}
//************************************************************************************************************




























// 				toStr(str_buffer, gb_SystemMode);glcd_display_string_small(0, 0, str_buffer);
// 				toStr(str_buffer, gb_ModuleWorkMode);glcd_display_string_small(0, 1, str_buffer);
// 				toStr(str_buffer, gb_ParameterChangeFlag);glcd_display_string_small(0, 2, str_buffer);
//				toStr(str_buffer, TxFlag);glcd_display_string_small(0, 3, str_buffer);
// 				toStr(str_buffer, RxFlag);glcd_display_string_small(0, 4, str_buffer);
// 				toStr(str_buffer, gb_SysTimer_1S);glcd_display_string_small(0, 5, str_buffer);
// 				toStr(str_buffer, gw_SendDataCount);glcd_display_string_small(60, 5, str_buffer);
// 				writeStringUART("\r\nSendDataCount: ");
// 				toStr(str_buffer, gb_SystemMode);writeStringUART(" ");writeMStringUART(str_buffer);
// 				toStr(str_buffer, gb_ModuleWorkMode);writeStringUART(" ");writeMStringUART(str_buffer);
// 				toStr(str_buffer, gb_ParameterChangeFlag);writeStringUART(" ");writeMStringUART(str_buffer);
// 				toStr(str_buffer, TxFlag);writeStringUART(" ");writeMStringUART(str_buffer);
// 				toStr(str_buffer, RxFlag);writeStringUART(" ");writeMStringUART(str_buffer);
// 				toStr(str_buffer, gb_RF_RSSI);writeStringUART(" ");writeMStringUART(str_buffer);
// 				toStr(str_buffer, gb_SysTimer_1S);writeStringUART(" ");writeMStringUART(str_buffer);
// 				toStr(str_buffer, gw_SendDataCount);writeStringUART(" ");writeMStringUART(str_buffer);
// 			GLCD_DESELECT();
//PORTB|=(1<<1);
//-----------------------------------------------------------------------------------
//_delay_ms(500);
//glcd_test_circles();
//glcd_test_counter_and_graph();
//glcd_test_text_up_down();
//glcd_test_tiny_text();
//glcd_test_hello_world();
//glcd_test_rectangles();
//glcd_test_scrolling_graph();

//glcd_test_bitmap_128x64();
// 			PORTD|=(1<<3);
// 		_delay_ms(500);
//glcd_draw_rect(42,24,20,20,BLACK);
//glcd_draw_rect_thick(1,1,20,20,2,2,BLACK);
// 			glcd_write();
//_delay_ms(500);
//glcd_draw_rect(42,24,20,20,WHITE);
//glcd_write();
//_delay_ms(500);
//glcd_draw_rect_thick(1,1,20,20,2,2,WHITE);
//glcd_write();
//_delay_ms(500);

// 		glcd_draw_bitmap(bmp_oslogo);
// 		_delay_ms(500);

// 			glcd_draw_bitmap(clock_bmp);
// 			_delay_ms(500);
// 			glcd_clear();

// 		glcd_display_string_small(0, 0, str);
// 		_delay_ms(500);
//glcd_display_number(0, 0, 5, adc);//Line max = 5
//_delay_ms(500);
//glcd_display_number(1, 0, 15, adc2);//Line max = 24 12345
//_delay_ms(500);
// 			glcd_display_char(40, 0, 126);
// 		_delay_ms(500);
// 			glcd_display_char(40, 44, 127);
// 		_delay_ms(500);
//glcd_draw_rect(0,0,84,48,BLACK);
//glcd_write();
//_delay_ms(500);
//glcd_test_glcdutils();
// 		RFM98_Running(gb_SystemMode,gb_ModuleWorkMode,gb_ParameterChangeFlag,&TxFlag,&RxFlag,&gb_RF_RSSI);

// 			toStr(str_buffer, Battery_voltage);writeStringUART("\r\n");writeMStringUART(str_buffer);
// 			toStr(str_buffer, Charger_voltage);writeStringUART(" ");writeMStringUART(str_buffer);
// 			toStr(str_buffer, Charger_current);writeStringUART(" ");writeMStringUART(str_buffer);
// 			toStr(str_buffer, Lamp_current);writeStringUART(" ");writeMStringUART(str_buffer);
// 			toStr(str_buffer, Lamp_temperature);writeStringUART(" ");writeMStringUART(str_buffer);
// 			toStr(str_buffer, temp_rf);writeStringUART(" ");writeMStringUART(str_buffer);

// 			if (button_pressed)
// 			{
// 				toStr(str_buffer, button_pressed);glcd_display_string_small(30, 5, str_buffer);
// 				button_pressed=0;
// 				RFM98Data[0]='0';RFM98Data[1]='0';RFM98Data[2]='0';RFM98Data[3]='0';
// 				RFM98Data[4]='0';RFM98Data[5]='0';RFM98Data[6]='0';RFM98Data[7]='0';
// 				RFM98Data[8]='0';RFM98Data[9]='0';RFM98Data[10]='0';RFM98Data[11]='0';
// 				RFM98Data[12]='0';RFM98Data[13]='0';RFM98Data[14]='0';RFM98Data[15]='0';
// 				RFM98Data[16]='0';RFM98Data[17]='0';RFM98Data[18]='0';RFM98Data[19]='0';
// 				RFM98Data[20]='1';
// 				Tx_request=1;
// 			}
