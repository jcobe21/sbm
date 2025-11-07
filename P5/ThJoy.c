#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "ThJoy.h"
#include "stm32f4xx_hal.h"
#include <stdlib.h>
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThJoy;                        // thread id
 
void ThJoy (void *argument);                   // thread function

typedef struct {
    GPIO_InitTypeDef pin;
    GPIO_TypeDef *port;
    int delay;
    uint8_t counter;
} mygpio_pin;



int Init_Thread (void) {
 
  tid_ThJoy = osThreadNew(ThJoy, NULL, NULL);
  if (tid_ThJoy == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void ThJoy (void *argument) {
 
  while (1) {
    ; // Insert thread code here...
   // osThreadYield();                            // suspend thread
  }
}
