#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "ThJoy.h"
#include "ThLeds.h"
#include "stm32f4xx_hal.h"
#include <stdlib.h>
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThJoy;                        // thread id
 
uint8_t cont_derecha = 0;
uint8_t cont_izquierda = 0;
uint8_t cont_arriba = 0;
uint8_t cont_abajo = 0;
uint8_t cont_centro = 0;

void Timer1_Callback(void *argument);


void ThJoy (void *argument);                   // thread function
   GPIO_InitTypeDef joy_derecha = {
    .Pin = GPIO_PIN_11, //PB 11
    .Mode = GPIO_MODE_IT_RISING,
    .Pull = GPIO_PULLDOWN,
    .Speed = GPIO_SPEED_FREQ_HIGH
  };
	    GPIO_InitTypeDef joy_izquierda = {
    .Pin = GPIO_PIN_14, //PE 14
    .Mode = GPIO_MODE_IT_RISING,
    .Pull = GPIO_PULLDOWN,
    .Speed = GPIO_SPEED_FREQ_HIGH
  };
			   GPIO_InitTypeDef joy_arriba = {
    .Pin = GPIO_PIN_10, //PB 10
    .Mode = GPIO_MODE_IT_RISING,
    .Pull = GPIO_PULLDOWN,
    .Speed = GPIO_SPEED_FREQ_HIGH
  };
	   GPIO_InitTypeDef joy_abajo = {
    .Pin = GPIO_PIN_12, //PE 12
    .Mode = GPIO_MODE_IT_RISING,
    .Pull = GPIO_PULLDOWN,
    .Speed = GPIO_SPEED_FREQ_HIGH
  };
		 
	   GPIO_InitTypeDef joy_centro = {
    .Pin = GPIO_PIN_15,  //PE15
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
mygpio_pin pinE14;
mygpio_pin pinB10;
mygpio_pin pinE12;
mygpio_pin pinE15;
int Init_ThJoy (void) {
 
  tid_ThJoy = osThreadNew(ThJoy, NULL, NULL);
  if (tid_ThJoy == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void ThJoy (void *argument) {
	
	  __HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOE_CLK_ENABLE();
	
	  pinB11.pin = joy_derecha;
    pinB11.port = GPIOB;
	  HAL_GPIO_Init(GPIOB,&joy_derecha);

		pinB10.pin = joy_arriba;
    pinB10.port = GPIOB;
	  HAL_GPIO_Init(GPIOB,&joy_arriba);
	
		pinE14.pin = joy_izquierda;
    pinE14.port = GPIOE;
	  HAL_GPIO_Init(GPIOE,&joy_izquierda);
	
		pinE12.pin = joy_abajo;
    pinE12.port = GPIOE;
	  HAL_GPIO_Init(GPIOE,&joy_abajo);
	
		pinE15.pin = joy_centro;
    pinE15.port = GPIOE;
	  HAL_GPIO_Init(GPIOE,&joy_centro);
	
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		
		osTimerId_t tim1 = osTimerNew(Timer1_Callback, osTimerOnce, NULL,NULL);
	
  while (1) {
    ; // Insert thread code here...
		osThreadFlagsWait(0x0001,osFlagsWaitAny,osWaitForever);
		osTimerStart(tim1,50);
		
		
   // osThreadYield();                            // suspend thread
  }

	
}

	void Timer1_Callback(void *argument){
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11)) {
			
			cont_derecha++;
			osThreadFlagsSet(tid_Thleds,0x0010);
			
		} else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)) {
			
			cont_arriba++;
  	osThreadFlagsSet(tid_Thleds,0x0001);
			
		} else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12)) {
			
			cont_abajo++;
			osThreadFlagsSet(tid_Thleds,0x0011);
			
		} else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14)) {
			
			cont_izquierda++;
			osThreadFlagsSet(tid_Thleds,0x0100);
			
		} else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15)) {
			
			cont_centro++;
			osThreadFlagsSet(tid_Thleds,0x0101);
		}
	}










