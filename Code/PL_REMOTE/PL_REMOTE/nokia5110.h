#ifndef __NOKIA_5110_H__
#define __NOKIA_5110_H__

//#include <avr/pgmspace.h>
//#include <stdint.h>

#define PORT_LCD PORTB
#define DDR_LCD DDRB

#define LCD_SCE PB1
#define LCD_RST PB2
#define LCD_DC PB3
#define LCD_DIN PB4
#define LCD_CLK PB5

#define LCD_CONTRAST 0x40


void nokia_lcd_init(void);

void nokia_lcd_clear(void);


void nokia_lcd_power(unsigned char on);


void nokia_lcd_set_pixel(unsigned char x, unsigned char y, unsigned char value);


void nokia_lcd_write_char(char code, unsigned char scale);


void nokia_lcd_write_string(const char *str, unsigned char scale);


void nokia_lcd_set_cursor(unsigned char x, unsigned char y);

void nokia_lcd_render(void);


#endif
