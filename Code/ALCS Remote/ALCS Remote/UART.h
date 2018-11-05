/*
uart.h
Implement uart communication
Date created: 24/07/2008
*/

#ifndef _UART_INCLUDED_
#define _UART_INCLUDED_     

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


// #define USE_1UART_ATMEGA
//#define USE_RS_485
// #define USE_MODBUS

#define USE_2UART_ATMEGA
#define USE_UART1
//#define USE_RS_485_1
//#define USE_MODBUS

#define USE_UART2
//#define USE_RS_485_2
#define USE_MODBUS

#ifdef USE_1UART_ATMEGA
	#define BAUND_RATE						192      //x100
	#define XTAL_FREQ						80000    //x100 Hz
	#define MAX_QUEUE						1//00
	#ifdef USE_RS_485
		#define RS_485_DIRECTION_PORT		PORTD
		#define RS_485_DIRECTION			7
	#endif
	
	void initUART(void);
	//UART
	unsigned char readUART();				//wait until data available
	int readWordUART();
	void writeUART(unsigned char data);			//write UART data and wait until transmit complete
	void writeUARTRaw(unsigned char data);			//write UART data and return immediately
	void writeWordUART(int data);
	void writeWordUARTRaw(int data);
	void writeStringUART(const char * str);			//write ROM string
	void writeMStringUART(char * str);			//write RAM string
	unsigned int queueLengthUART();			//check buffer size
	void flushUARTRX(void);				//empty RX buffer 
#endif
#ifdef USE_2UART_ATMEGA
	#define XTAL_FREQ  							147456   //*100 Hz
	void initUART(void);
	
	#ifdef USE_UART1
		#define BAUND_RATE1 					96      //*100
		#define MAX_QUEUE1						100//500//1024	
		#ifdef USE_RS_485_1
			#define RS_485_DIRECTION_PORT1		PORTD
			#define RS_485_DIRECTION1			7
		#endif
		//UART1
		unsigned char readUART1(void);				//wait until data available
		int readWordUART1(void);
		void writeUART1(unsigned char data);			//write UART data and wait until transmit complete
		void writeUART1Raw(unsigned char data);			//write UART data and return immediately
		void writeWordUART1(int data);
		void writeWordUART1Raw(int data);
		void writeStringUART1(const char * str);			//write ROM string
		void writeMStringUART1(char * str);			//write RAM string
		unsigned int queueLengthUART1(void);			//check buffer size
		void flushUART1RX(void);				//empty RX buffer
	#endif//USE_UART1
	#ifdef USE_UART2
		#define BAUND_RATE2 					384      //*100
		#define MAX_QUEUE2						100
		#ifdef USE_RS_485_2
			#define RS_485_DIRECTION_PORT2		PORTD
			#define RS_485_DIRECTION2			1
		#endif
		//UART2
		unsigned char readUART2(void);				//wait until data available
		int readWordUART2(void);
		void writeUART2(unsigned char data);			//write UART data and wait until transmit complete
		void writeUART2Raw(unsigned char data);			//write UART data and return immediately
		void writeWordUART2(int data);
		void writeWordUART2Raw(int data);
		void writeStringUART2(const char * str);			//write ROM string
		void writeMStringUART2(char * str);			//write RAM string
		unsigned int queueLengthUART2(void);			//check buffer size
		void flushUART2RX(void);				//empty RX buffer
	#endif//USE_UART2
#endif
unsigned char toHex(char ch);
void toStr(char *str, unsigned int num);
void toHexStr2(char *str, unsigned char num);
void toHexStr4(char *str, unsigned int num);
void toDecStr(char *str, unsigned int num);
void toDecStr2(char *str, unsigned int num);
void toDecStr5(char *str, unsigned int num);

#endif 
