/*
 * LCD5110.h
 *
 * Created: 10/6/2017 9:43:47 AM
 *  Author: Angela Belnett
 */ 


#ifndef LCD5110_H_
#define LCD5110_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "DK_SPI.h"

#define LCD_SS_set	 PORTB &=~(1<<1)
#define LCD_SS_clr	 PORTB |= (1<<1)
#define LCD_RST_set  PORTB |= (1<<0)    //external reset input
#define LCD_RST_clr  PORTB &=~(1<<0)
#define LCD_DC_set   PORTB |= (1<<2)    //data/commande
#define LCD_DC_clr   PORTB &=~(1<<2)
#define SDIN_set     PORTB |= (1<<3)    //serial data input
#define SDIN_clr     PORTB &=~(1<<3)
#define SCLK_set     PORTB |= (1<<5)    //serial clock input
#define SCLK_clr     PORTB &=~(1<<5)

void LCD_select(void);
void LCD_deselect(void);
void LCD_set_contrast(unsigned char constrast_value);
void LCD_write_byte(unsigned char dat, unsigned char command);
void LCD_init(void);
void LCD_clear(void);
void LCD_set_XY(unsigned char X, unsigned char Y);
void LCD_write_charxy(unsigned char X,unsigned char Y, unsigned char c);
void LCD_write_char(unsigned char c);
void LCD_write_string(unsigned char X,unsigned char Y,char *s);

#endif /* LCD5110_H_ */