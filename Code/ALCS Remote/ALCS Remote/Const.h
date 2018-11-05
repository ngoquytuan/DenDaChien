#ifndef _CONST_H_
#define _CONST_H_

#include <avr/io.h>
//Define for MODBUS
#define CLIENTADDRESS				0x01
#define MODBUSMODE					0
#define MULTIWIREMODE				1

//Define for button scan and button mapping
#define LEVEL_0						0
#define LEVEL_1						1
#define LEVEL_2						2
#define LEVEL_3						3

#define BRIGHTNESS_CONTROL			PINA&0xC0				//SA-SB
#define BRIGHTNESS_OFF				0xC0					//1100 0000
#define BRIGHTNESS_LEVEL_1			0x40					//0100 0000
#define BRIGHTNESS_LEVEL_2			0x80					//1000 0000
#define BRIGHTNESS_LEVEL_3			0x00					//0000 0000

#define TYPEOFLAMP_CONTROL			PINA&0x30				//SC-SRST
#define TYPEOFLAMP_RUNWAY			0x30					//0011 0000
#define TYPEOFLAMP_TAXIWAY			0x20					//0010 0000
#define TYPEOFLAMP_ENDTHRES			0x10					//0001 0000
#define TYPEOFLAMP_FLASHING			0x00					//0000 0000
//Feedback FB0-FB1: Brightness level
#define FB_LEVEL_0					PORTC=(PORTC&0xFC)|0x00
#define FB_LEVEL_1					PORTC=(PORTC&0xFC)|0x01
#define FB_LEVEL_2					PORTC=(PORTC&0xFC)|0x02
#define FB_LEVEL_3					PORTC=(PORTC&0xFC)|0x03
//Feedback FB2-FB3: Type of lamp
#define RUNWAY						0
// #define RUNWAY_WY				1
#define TAXIWAY						1
#define ENDTHRES					2
#define FLASHING					3
#define NUM_OF_ONLINE_LAMP			300
#define NUMOFLAMPTYPE				5

#define FB_LAMP_RUNWAY				PORTC=(PORTC&0xF3)|0x00
// #define FB_LAMP_RUNWAY_WY		PORTC=(PORTC&0xF3)|0x01
#define FB_LAMP_TAXIWAY				PORTC=(PORTC&0xF3)|0x01
#define FB_LAMP_ENDTHRES			PORTC=(PORTC&0xF3)|0x02
#define FB_LAMP_FLASHING			PORTC=(PORTC&0xF3)|0x03
//Mapping

//Define for data frame
//Define for data frame
#define CONTROL_MESSAGE					0
#define RESPONSE_MESSAGE				1
#define HEADER_LENGTH					2
#define	BRIGHTNESS_LEVEL_POS			4
#define TYPEOFLAMP_POS					2

//#define BRIGHTNESS_DATA_POS			20
//#define TYPEOFLAMP_DATA_POS			19
//Define for screen

#endif

