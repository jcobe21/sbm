#ifndef __LCD_H
#define __LCD_H

#include "main.h"
#include "stdio.h"                                                              //para que funcione el sprintf //COBE NO LO TIENE

#include "cmsis_os2.h"                                      // ::CMSIS:RTOS2

extern osThreadId_t tid_ThLCD;                        // thread id
int Init_ThLCD();

void delay (uint32_t n_microsegundos);
void PINES_init(void);

void LCD_reset (void);
void LCD_wr_data (unsigned char data);
void LCD_wr_cmd (unsigned char cmd);
void LCD_init(void);
void LCD_update (void);
void LCD_clean (void);

//void pintar_cuadrado8x8(int pos);
//void pintar_borde_cuadrado(void);

//void LCD_symbolToLocalBuffer_L1(uint8_t symbol);
//void linea_L1(void);
//void LCD_symbolToLocalBuffer_L2(uint8_t symbol);
//void linea_L2(void);
void symbolToLocalBuffer(uint8_t line,uint8_t symbol);
//void lineas_L1L2(void);

//void LCD_L1(void);
//void LCD_L2(void);

void LCD_L1L2_JoyStick(void);
void LCD_L1L2_UP(void);
void LCD_L1L2_RIGHT(void);
void LCD_L1L2_DOWN(void);
void LCD_L1L2_LEFT(void);
void LCD_L1L2_CENTER(void);
void LCD_L1L2_JoyStick_error(void);

#endif
