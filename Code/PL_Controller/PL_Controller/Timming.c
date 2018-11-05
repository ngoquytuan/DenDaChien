#include "Timming.h"

extern volatile unsigned int time_to_response;
volatile unsigned char flag_1kHz=0, flag_100Hz=0, flag_10Hz=0, flag_1Hz=0, flag_flashing=0;
volatile unsigned int tick=0;
signed int PID_Param[3] = {10, 1, 2};//Good 5 1 2
signed int pid_sub_param = 100;
volatile signed int e_A[PID_ERROR_NUMBER], e_B[PID_ERROR_NUMBER];
unsigned int DutyA=0, DutyB=0;
// volatile unsigned char ADC_Channel=0, adctick=0;
// volatile unsigned int ADC_Data[3], tp=0;
volatile unsigned int time_to_reset_data=0;
char buffer[20];

void initInterrupt(void)
{
	GICR|=0x40;
	MCUCR=0x02;
	GIFR=0x40;
}
void initTimer0(void)//1KHz
{
	TCCR0 	= 	0x03;//Internal clock CLK/64
	TCNT0 	= 	130;
	TIMSK 	|= 	0x01;
}
void initTimer1(void)//Fast PWM OC1A and OC1B clk_pwm = CLK/(1*(1+ICR1)) = 8kHz TOP = ICR1
{
	TCCR1A	|=	(1<<WGM11)|(0<<WGM10);
	TCCR1B	|=	(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);//CLK/1
	TCNT1	=	0;
	ICR1H	=	0x27;//0x03;//999
	ICR1L	=	0x0F;//0xE7;
	OCR1A	=	0;
	OCR1B	=	0;
}
/*
void initTimer2(void)
{
	TCCR2	|=	(1<<WGM21)|(1<<COM21)|(1<<CS22)|(1<<CS21)|(1<<CS20);//CTC Non-Invert CLK/1024
	TCNT2	=	0x00;
	OCR2	=	127;//50%
	TIMSK 	|=	0x80; 
}
*/
void StartStop_PWM(unsigned int CurrentA, unsigned int CurrentB)
{
signed int sumA=0, sumB=0;
unsigned char i=0;
 	
	if (CurrentA>0)
	{		
		for (i=PID_ERROR_NUMBER-1; i>0; i--)
		{
			e_A[i] = e_A[i-1];
			sumA += e_A[i];
		}
		e_A[0] = (signed int)CurrentA - (signed int)Lamp_current1;
		sumA += e_A[0];
		DutyA += (signed int)PID_Param[0]*(signed int)e_A[0]/pid_sub_param;
		DutyA += (signed int)PID_Param[1]*(signed int)sumA/pid_sub_param;
		DutyA += (signed int)PID_Param[2]*(signed int)(e_A[0]-e_A[1])/pid_sub_param;
	}
	else DutyA = 0;
	
	if (CurrentB>0)
	{
		for (i=PID_ERROR_NUMBER-1; i>0; i--)
		{
			e_B[i] = e_B[i-1];
			sumB += e_B[i];
		}
		e_B[0] = (signed int)CurrentB - (signed int)Lamp_current2;
		sumB += e_B[0];
		DutyB += (signed int)PID_Param[0]*(signed int)e_B[0]/pid_sub_param;
		DutyB += (signed int)PID_Param[1]*(signed int)sumB/pid_sub_param;
		DutyB += (signed int)PID_Param[2]*(signed int)(e_B[0]-e_B[1])/pid_sub_param;
	}
	else DutyB = 0;
	
// 	DutyA = CurrentA;
// 	DutyB = CurrentB;
	if (DutyA>9999) DutyA=9999;
	if (DutyB>9999) DutyB=9999;
	
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
//=============================================================================================================
ISR(TIMER0_OVF_vect)
{
	TCNT0= 130;
	tick++;
	flag_1kHz = 1;
	
	if (tick%10==0) {flag_100Hz = 1;}
	if (PortableLamp.TypeOfLamp!=FLASHING) 
	{
		if (tick%100==0){flag_10Hz = 1;}
	}
	else 
	{
		if (tick%50==0){flag_10Hz = 1;}
		if (tick%500==0) {if (PortableLamp.Brightness_level) flag_flashing=1;}
	}
// 	if (tick%150==0){if(gb_SysTimer){gb_SysTimer--;}}
	if (tick%1000==0) 
	{
		flag_1Hz = 1;
		time_to_reset_data++;
		if (time_to_reset_data>=120)
		{
			time_to_reset_data=0;
			Reset_Duty_Min_Max();
		}
	}	
// 	if(gw_TxTimer){gw_TxTimer--;}
	if (time_to_response) 
	{
		time_to_response--;
		if (time_to_response==0) Send_Response_Message(); 
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

