#include "Timming.h"

volatile unsigned char flag_1kHz=0, flag_100Hz=0, flag_10Hz=0, flag_1Hz=0;
volatile unsigned int tick=0;

void initInterrupt(void)
{
/*ATMEGA8
	GICR|=0x40;
	MCUCR=0x02;
	GIFR=0x40;
*/
//ATMEGA128
	// INT0: On Mode: Falling Edge
	// INT1: Off
	// INT2: off
	// INT3: Off
	// INT4: off
	// INT5: off
	// INT6: Off
	// INT7: Off
	EICRA	=	0x02;//00 00 00 10			INT0:3
	EICRB	=	0x00;//00 00 00 00			INT4:7
	EIMSK	=	0x01;//0000 0001			INT0:7
	EIFR	=	0x01;//0000 0001			INT0:7
	// Analog Comparator initialization
	// Analog Comparator: Off
}

void initTimer0(void)//1KHz
{		
	TCCR0 	= 	0x04;//Internal clock CLK/64
	TCNT0 	= 	25;
	TIMSK 	|= 	0x01;
}

void initTimer1(void)//10kHz
{
//Fast PWM OC1A and OC1B clk_pwm = CLK/(8*(1+ICR1)) = 1000Hz TOP = ICR1	
// 	TCCR1A	|=	(1<<WGM11)|(0<<WGM10);
// 	TCCR1B	|=	(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);//CLK/8
// 	TCNT1	=	0;
// 	ICR1H	=	0x03;//999
// 	ICR1L	=	0xE7;
// 	OCR1A	=	0;
// 	OCR1B	=	0;
	TCCR1B	=	0x01;//CLK/1 CTC Top Mode
	OCR1AH	=	0x05;
	OCR1AL	=	0xC2;
	TIMSK 	|=	0x10;
}

void StartStop_PWM(unsigned int DutyA, unsigned int DutyB)
{
	if (DutyA>0)
	{
		OCR1A	=	DutyA;
		TCCR1A	|=	(1<<COM1A1);	
	}
	else TCCR1A	&=	~(1<<COM1A1);
	if (DutyB>0)
	{
		OCR1B	=	DutyB;
		TCCR1A	|=	(1<<COM1B1);
	}
	else TCCR1A	&=	~(1<<COM1B1);
}

void initTimer2(void)
{
	TCCR2	|=	(1<<WGM21)|(1<<COM21)|(1<<CS22)|(1<<CS21)|(1<<CS20);//CTC Non-Invert CLK/1024
	TCNT2	=	0x00;
	OCR2	=	127;//50%
	TIMSK 	|=	0x80; 
}

ISR(TIMER0_OVF_vect)//1KHz
{
	TCNT0= 25;
	tick++;
	flag_1kHz = 1;
	
	if (tick%10==0){flag_100Hz = 1;}
	if (tick%100==0) {flag_10Hz = 1;}
// 	if (tick%150==0){if(gb_SysTimer){gb_SysTimer--;}}
	if (tick%1000==0) {flag_1Hz = 1;}

	wdt_reset();
}

ISR(TIMER1_COMPA_vect)
{
	modbusTickTimer();
	wdt_reset();
}

ISR(TIMER2_COMP_vect)
{
	wdt_reset();
} 

ISR(INT0_vect)
{ 

}

ISR(INT1_vect)
{ 

} 


