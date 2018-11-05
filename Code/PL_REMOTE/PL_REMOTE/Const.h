#ifndef _CONST_H_
#define _CONST_H_

#include <avr/io.h>

// #define sbi(var, mask)		((var) |= _BV(mask))
// #define cbi(var, mask)		((var) &= ~(_BV(mask)))
// #define DDR(x)				(*(&x - 1))
// #define PIN(x)				(*(&x - 2))
// #define CONTROLLER_SS_PORT   PORTD
// #define CONTROLLER_SS_PIN    4
// #define GLCD_SELECT()		cbi(CONTROLLER_SS_PORT,CONTROLLER_SS_PIN)//;PORTB|=(1<<1)
// #define GLCD_DESELECT()		sbi(CONTROLLER_SS_PORT,CONTROLLER_SS_PIN)//;PORTB&=~(1<<1)

//Define for button scan and button mapping
#define NUM_OF_BUTTON					16
#define BUTTON_PRESS_THRESHOLD			100
//Scan port and pin
#define BUTTON_S11						(PIND&(1<<2))
#define BUTTON_COL(x)					PORTD=((PORTD&0x8F)|(1<<(x)))//1000 1111
#define BUTTON_ROW						(PINC&0x1F)//0001 1111
//Mapping
#define BUTTON_DOWN						16//S11
#define BUTTON_UP						6//S3
#define BUTTON_LEFT						1//S2
#define BUTTON_RIGHT					11//S4
#define BUTTON_NUM1						2//S5
#define BUTTON_NUM2						7//S6
#define BUTTON_NUM3						12//S7
#define BUTTON_NUM4						3//S8
#define BUTTON_NUM5						8//S9
#define BUTTON_NUM6						13//S10
#define BUTTON_NUM7						4//S12
#define BUTTON_NUM8						9//S13
#define BUTTON_NUM9						14//S14
#define BUTTON_NUM0						10//S16
#define BUTTON_STAR						5//S15
#define BUTTON_HASH						15//S17
//Define for data frame
#define CONTROL_MESSAGE					0
#define RESPONSE_MESSAGE				1
#define HEADER_LENGTH					2
#define	BRIGHTNESS_LEVEL_POS			4
#define TYPEOFLAMP_POS					2

// #define TYPEOFLAMP						19
// #define FEEDBACK_POS					15
//Type of lamp and brightness level
#define LEVEL_0							0
#define LEVEL_1							1
#define LEVEL_2							2
#define LEVEL_3							3

#define NUMOFLAMPTYPE					5
#define RUNWAY							0
#define TAXIWAY							1
#define ENDTHRES						2
#define FLASHING						3
#define ALLLAMP							4
#define NUM_OF_ONLINE_LAMP				300
//Define for screen
#define MAIN_SCREEN						0
#define MONITOR_SCREEN					1

//Define for LCD Display
#define LCD_BACKLIGHT_OFF				PORTD|=(1<<7)
#define LCD_BACKLIGHT_ON				PORTD&=~(1<<7)
#define LCD_BACKLIGHT_BLINK				PORTD^=(1<<7)
#define LCD_BACKLIGHT_TIMEOUT			10
#define LCD_CONTRAST					72

// #define ARROW_UP						126
// #define ARROW_DOWN						127

#endif

