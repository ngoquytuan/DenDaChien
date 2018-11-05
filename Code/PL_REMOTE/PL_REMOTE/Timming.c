#include "Timming.h"

// extern volatile unsigned char gb_SysTimer;
volatile unsigned char flag_1kHz=0, flag_100Hz=0, flag_10Hz=0, flag_1Hz=0;
volatile unsigned int tick=0;
volatile unsigned char screen_timeout=0;
volatile unsigned char Button[NUM_OF_BUTTON]={0};
volatile unsigned char button_pressed=0;
// volatile unsigned int ADC_Data[3], tp=0;

// void initInterrupt(void)
// {
// 	GICR|=0x40;
// 	MCUCR=0x02;
// 	GIFR=0x40;
// }

void initTimer0(void)//1KHz
{		
	TCCR0 	= 	0x03;//Internal clock CLK/64
	TCNT0 	= 	130;
	TIMSK 	|= 	0x01;
}
// void initTimer1(void)//Fast PWM OC1A and OC1B clk_pwm = CLK/(8*(1+ICR1)) = 1000Hz TOP = ICR1
// {
// 	TCCR1A	|=	(1<<WGM11)|(0<<WGM10);
// 	TCCR1B	|=	(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);//CLK/8
// 	TCNT1	=	0;
// 	ICR1H	=	0x03;//999
// 	ICR1L	=	0xE7;
// 	OCR1A	=	0;
// 	OCR1B	=	0;
// }

// void StartStop_PWM(unsigned int DutyA, unsigned int DutyB)
// {
// 	if (DutyA>0)
// 	{
// 		OCR1A	=	DutyA;
// 		TCCR1A	|=	(1<<COM1A1);	
// 	}
// 	else TCCR1A	&=	~(1<<COM1A1);
// 	if (DutyB>0)
// 	{
// 		OCR1B	=	DutyB;
// 		TCCR1A	|=	(1<<COM1B1);
// 	}
// 	else TCCR1A	&=	~(1<<COM1B1);
// }

// void initTimer2(void)
// {
// 	TCCR2	|=	(1<<WGM21)|(1<<COM21)|(1<<CS22)|(1<<CS21)|(1<<CS20);//CTC Non-Invert CLK/1024
// 	TCNT2	=	0x00;
// 	OCR2	=	127;//50%
// 	TIMSK 	|=	0x80; 
// }

ISR(TIMER0_OVF_vect)//1KHz
{
	unsigned char i=0, j=0;
	TCNT0= 130;
	tick++;
	flag_1kHz = 1;
	
	if (tick%10==0){flag_100Hz = 1;}
	if (tick%100==0){flag_10Hz = 1;}
	if (tick%1000==0) 
	{
		flag_1Hz = 1;
		if (screen_timeout) {screen_timeout--; LCD_BACKLIGHT_ON;}
		else LCD_BACKLIGHT_OFF;
	}
	
	if (BUTTON_S11==0){if (Button[15]<255) Button[15]++;}else {if (Button[15]>=BUTTON_PRESS_THRESHOLD) button_pressed=16;Button[15]=0;}
	for (j=0; j<3; j++)
	{
		BUTTON_COL(j+4);
		for (i=0;i<5;i++)
			if (BUTTON_ROW==(1<<i))
			{
				Button[j*5+i]++;if (Button[j*5+i]>=BUTTON_PRESS_THRESHOLD) Button[j*5+i]=BUTTON_PRESS_THRESHOLD;
			}
			else
			{
				if (Button[j*5+i]>=BUTTON_PRESS_THRESHOLD)
				{
					button_pressed=j*5+i+1;
					Button[j*5+i]=0;
				}
			}
	}
	wdt_reset();
}

// ISR(TIMER1_COMPA_vect)
// {
// 	
// 	wdt_reset();
// }
// 
// ISR(TIMER2_COMP_vect)
// {
// 	adctick++;//<255
// 	if (adctick>=100)//100ms
// 	{
// 		adctick=0;
// 		ADC_Channel++;
// 		if (ADC_Channel>2) ADC_Channel=0;
// 		ADMUX	= 0xC0 | ADC_Channel;
// 		ADCSRA 	|= 0x40;
// 	}
// 
// 	tick++;//<255
// 	if (tick>=100)//100ms
// 	{
// 		tick=0;
// 	
// 		tp=ADC_Data[2]*61/250-3;
// 		// 		if (Tempold==0) {Temperature=tp;}
// 		// 		else {Temperature=(tp+Tempold)/2;}
// 		// 		Tempold=Temperature;
// 
// 		tp=ADC_Data[0];
// 		// 		if (CBold==0) {CB_Current=tp;}
// 		// 		else {CB_Current=(tp+CBold)/2;}
// 		// 		CBold=CB_Current;
// 
// 		tp=ADC_Data[1];
// 		// 		if (HBold==0) {HB_Current=tp;}
// 		// 		else {HB_Current=(tp+HBold)/2;}
// 		// 		HBold=HB_Current;
// 	}
// 	wdt_reset();
// } 
/*
ISR(INT0_vect)
{ 

}
*/
/*
ISR(INT1_vect)
{ 

} 
*/

