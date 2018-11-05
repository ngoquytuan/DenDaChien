#ifndef _CONST_H_
#define _CONST_H_

#include <avr/io.h>
//-----------------------------------------------------------------------------------------------------------
#define TESTLAMP
// #define RUNWAYLAMP1
// #define RUNWAYLAMP2
// #define TAXIWAYLAMP
// #define ENDTHRESLAMP
// #define FLASHINGLAMP
//-----------------------------------------------------------------------------------------------------------
#ifdef TESTLAMP
	#define DEFAULTTYPEOFLAMP				RUNWAY
	#define DEVICE_ID						1
	#define HIGH_LEVEL_BRIGHTNESSA			450					//WHITE	100%
	#define MED_LEVEL_BRIGHTNESSA			400					//75%
	#define LOW_LEVEL_BRIGHTNESSA			300					//50%
	#define HIGH_LEVEL_BRIGHTNESSB			450					//WHITE	100%
	#define MED_LEVEL_BRIGHTNESSB			400					//75%
	#define LOW_LEVEL_BRIGHTNESSB			300					//50% (Min 200)
	#define BATTERY_OFFSET_VOLTAGE			45
	#define CHARGER_OFFSET_VOLTAGE			35
#endif
//-----------------------------------------------------------------------------------------------------------
#ifdef RUNWAYLAMP1
	#define DEFAULTTYPEOFLAMP				RUNWAY
	#define DEVICE_ID						1
	#define HIGH_LEVEL_BRIGHTNESSA			415//450			//WHITE	100%	
	#define MED_LEVEL_BRIGHTNESSA			400//400			//75%
	#define LOW_LEVEL_BRIGHTNESSA			385//300			//50%
	#define HIGH_LEVEL_BRIGHTNESSB			415					//WHITE	100%
	#define MED_LEVEL_BRIGHTNESSB			400					//75%
	#define LOW_LEVEL_BRIGHTNESSB			385					//50% (Min 200)
	#define BATTERY_OFFSET_VOLTAGE			85
	#define CHARGER_OFFSET_VOLTAGE			70
#endif
//-----------------------------------------------------------------------------------------------------------
#ifdef RUNWAYLAMP2
	#define DEFAULTTYPEOFLAMP				RUNWAY
	#define DEVICE_ID						2
	#define HIGH_LEVEL_BRIGHTNESSA			450//450			//WHITE
	#define MED_LEVEL_BRIGHTNESSA			435//400
	#define LOW_LEVEL_BRIGHTNESSA			410//300
	#define HIGH_LEVEL_BRIGHTNESSB			370//450			//YELLOW
	#define MED_LEVEL_BRIGHTNESSB			340//400
	#define LOW_LEVEL_BRIGHTNESSB			320//300
	#define BATTERY_OFFSET_VOLTAGE			65
	#define CHARGER_OFFSET_VOLTAGE			47
#endif
//-----------------------------------------------------------------------------------------------------------
#ifdef TAXIWAYLAMP
	#define DEFAULTTYPEOFLAMP				TAXIWAY
	#define DEVICE_ID						1
	#define HIGH_LEVEL_BRIGHTNESSA			400//400			//BLUE
	#define MED_LEVEL_BRIGHTNESSA			350//300
	#define LOW_LEVEL_BRIGHTNESSA			300//200
	#define HIGH_LEVEL_BRIGHTNESSB			400//400			//BLUE
	#define MED_LEVEL_BRIGHTNESSB			350//300
	#define LOW_LEVEL_BRIGHTNESSB			300//200
	#define BATTERY_OFFSET_VOLTAGE			41
	#define CHARGER_OFFSET_VOLTAGE			28
#endif
//-----------------------------------------------------------------------------------------------------------
#ifdef ENDTHRESLAMP
	#define DEFAULTTYPEOFLAMP				ENDTHRES
	#define DEVICE_ID						1
	#define HIGH_LEVEL_BRIGHTNESSA			375					//GREEN
	#define MED_LEVEL_BRIGHTNESSA			300			
	#define LOW_LEVEL_BRIGHTNESSA			200			
	#define HIGH_LEVEL_BRIGHTNESSB			275					//RED
	#define MED_LEVEL_BRIGHTNESSB			225
	#define LOW_LEVEL_BRIGHTNESSB			200
	#define BATTERY_OFFSET_VOLTAGE			70
	#define CHARGER_OFFSET_VOLTAGE			61
#endif
//-----------------------------------------------------------------------------------------------------------
#ifdef FLASHINGLAMP
	#define DEFAULTTYPEOFLAMP				FLASHING
	#define DEVICE_ID						1
	#define FLASHING_PERIOD					120					//Hz
	#define HIGH_LEVEL_BRIGHTNESSA			400					//WHITE
	#define MED_LEVEL_BRIGHTNESSA			300
	#define LOW_LEVEL_BRIGHTNESSA			200
	#define HIGH_LEVEL_BRIGHTNESSB			400					//WHITE
	#define MED_LEVEL_BRIGHTNESSB			300
	#define LOW_LEVEL_BRIGHTNESSB			200
	#define BATTERY_OFFSET_VOLTAGE			91
	#define CHARGER_OFFSET_VOLTAGE			82
#endif
//-----------------------------------------------------------------------------------------------------------
//Define for Type Of Lamp
#define NUMOFLAMPTYPE					5
#define RUNWAY							0
#define TAXIWAY							1
#define ENDTHRES						2
#define FLASHING						3
#define ALLLAMP							4
//-----------------------------------------------------------------------------------------------------------
//Define for ADC and PID
#define AVERAGE_NUM						8
#define PID_ERROR_NUMBER				5
//-----------------------------------------------------------------------------------------------------------
//Define for Operation MODE
#define REMOTE_MODE						(PINC&(1<<5))				//(PINC&(1<<4))
#define LOCAL_MODE						(PIND&(1<<2))				//(PINC&(1<<3))
#define REMOTE_MODE_SELECTED			2
#define LOCAL_MODE_SELECTED				1
#define TEST_MODE_SELECTED				0
//-----------------------------------------------------------------------------------------------------------
//Define for Brightness Level
#define HIGH_LEVEL						(PINC&(1<<4))				//(PINC&(1<<5))
#define LOW_LEVEL						(PINC&(1<<3))				//(PIND&(1<<2))
#define HIGH_LEVEL_SELECTED				3
#define MED_LEVEL_SELECTED				2
#define LOW_LEVEL_SELECTED				1
#define OFF_LEVEL_SELECTED				0
#define OFF								0
//-----------------------------------------------------------------------------------------------------------
//Define for Control and Driver
#define CHARGER_ENABLE					PORTD|=(1<<4)
#define CHARGER_DISABLE					PORTD&=~(1<<4)
#define CHARGER_VOLTAGE_MIN				1420						//14.20 VDC
#define CHARGER_VOLTAGE_MAX				1500						//15.00 VDC
#define BATTERY_VOLTAGE_FULL			1420						//14.20 VDC
#define BATTERY_VOLTAGE_HIGH			1360						//13.60 VDC
#define BATTERY_VOLTAGE_MID				1260						//12.60 VDC
#define BATTERY_VOLTAGE_LOW				1060						//10.60 VDC
#define BATTERY_VOLTAGE_CHARGE_OFF		5							//>14.25V
#define BATTERY_VOLTAGE_CHARGE_ON		60							//<13.60V
#define BATTERY_VOLTAGE_DELTA_OFF		15							//0.15VDC	Old: 15 ~ 0.15VDC
#define BATTERY_VOLTAGE_DELTA_ON		15							//0.5VDC	Old: 15 ~ 0.15VDC
#define BATTERY_CURRENT_MIN				9							//Minimum ADC value of battery charge current 
#define LAMP_CURRENT_MIN				60
//-----------------------------------------------------------------------------------------------------------
#define UPPER_LAMP_ON					PORTB|=(1<<1)
#define UPPER_LAMP_OFF					PORTB&=~(1<<1)
#define LOWER_LAMP_ON					PORTB|=(1<<2)
#define LOWER_LAMP_OFF					PORTB&=~(1<<2)
//-----------------------------------------------------------------------------------------------------------
//Define for Status Indication
#define LOWBATTERY_LED_ON				PORTB|=(1<<7)
#define LOWBATTERY_LED_OFF				PORTB&=~(1<<7)
#define MIDBATTERY_LED_ON				PORTD|=(1<<5)
#define MIDBATTERY_LED_OFF				PORTD&=~(1<<5)
#define HIGHBATTERY_LED_ON				PORTD|=(1<<6)
#define HIGHBATTERY_LED_OFF				PORTD&=~(1<<6)
#define FULLBATTERY_LED_ON				PORTD|=(1<<7)
#define FULLBATTERY_LED_OFF				PORTD&=~(1<<7)

#define LOWBATTERY_LED_BLINK			PORTB^=(1<<7)
#define MIDBATTERY_LED_BLINK			PORTD^=(1<<5)
#define HIGHBATTERY_LED_BLINK			PORTD^=(1<<6)
#define FULLBATTERY_LED_BLINK			PORTD^=(1<<7)

#define LOCAL_REMOTE_LED_OFF			PORTB&=~(1<<6)
#define LOCAL_REMOTE_LED_ON				PORTB|=(1<<6)
#define LOCAL_REMOTE_LED_BLINK			PORTB^=(1<<6)
//-----------------------------------------------------------------------------------------------------------
//Define for RF Communication
// #define RF_SELECT						PORTB&=~(1<<0)
// #define RF_DESELECT						PORTB|=(1<<0)
//-----------------------------------------------------------------------------------------------------------
//Define for data frame
#define CONTROL_MESSAGE					0
#define HEADER_LENGTH					2
#define	BRIGHTNESS_LEVEL_POS			4
#define TYPEOFLAMP_POS					2
// #define FEEDBACK_POS					15
#endif
