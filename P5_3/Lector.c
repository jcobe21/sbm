#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "ThJoy.h"
#include "lcd.h"
#include <stdio.h>
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Lector;                        // thread id
osStatus_t status_c;
int qsize = 0;

uint8_t val = 0;

char valor[32];
void Lector(void *argument);                   // thread function
 
int Init_Lector (void) {
 
  tid_Lector = osThreadNew(Lector, NULL, NULL);
  if (tid_Lector == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Lector (void *argument) {
	LCD_reset();
	LCD_init();
	LCD_limpiarBuffer();
  while (1) {
    ; // Insert thread code here...
		qsize=osMessageQueueGetCount (id_MsgQueue); 
		status_c = osMessageQueueGet(id_MsgQueue, &val,NULL,10U);
		sprintf(valor,"Posicion: %d",val);
		LCD_writeLine(2, valor);
		LCD_update();
    //osThreadYield();                            // suspend thread
  }
}
