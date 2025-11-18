#ifndef __lcd_h_
#define __lcd_h_

#include "main.h"

void LCD_reset(void);
void LCD_wr_data(unsigned char data);
void LCD_wr_cmd(unsigned char cmd) ;

void LCD_init (void);

void LCD_update (void);

void symbolToLocalBuffer(uint8_t line,uint8_t symbol);

void LCD_writeLine(uint8_t line, const char* texto);

void LCD_llenarBuffer(void) ;
void LCD_limpiarBuffer(void) ;

#endif