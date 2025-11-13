#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "ThJoy.h"
#include "stm32f4xx_hal.h"
#include <stdlib.h>
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThJoy;                        // thread id
 
void ThJoy (void *argument);                   // thread function
   GPIO_InitTypeDef joy_derecha = {
    .Pin = GPIO_PIN_11,
    .Mode = GPIO_MODE_IT_RISING,
    .Pull = GPIO_PULLDOWN,
    .Speed = GPIO_SPEED_FREQ_HIGH
  };
	
typedef struct {
    GPIO_InitTypeDef pin;
    GPIO_TypeDef *port;
    int delay;
    uint8_t counter;
} mygpio_pin;

mygpio_pin pinB11;

int Init_ThJoy (void) {
 
  tid_ThJoy = osThreadNew(ThJoy, NULL, NULL);
  if (tid_ThJoy == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void ThJoy (void *argument) {
	
	 __HAL_RCC_GPIOB_CLK_ENABLE();
	  pinB11.pin = joy_derecha;
    pinB11.port = GPIOB;
	  HAL_GPIO_Init(GPIOB,&joy_derecha);

	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	
  while (1) {
    ; // Insert thread code here...
		
		
		
   // osThreadYield();                            // suspend thread
  }
}








