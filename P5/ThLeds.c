#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include <stdlib.h>
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thleds;                        // thread id
 
void Thleds (void *argument);                   // thread function
int Init_Thleds (void);


typedef struct {
    GPIO_InitTypeDef pin;
    GPIO_TypeDef *port;
    int delay;
    uint8_t counter;
} mygpio_pin;

mygpio_pin pinB0;
//mygpio_pin pinB7;
mygpio_pin pinB14;
 
int Init_Thleds (void) {       //Función de inicialización del hilo 
 	
  tid_Thleds = osThreadNew(Thleds, NULL, NULL); //Como queremos que un hilo gestione 3 leds...
	
  if (tid_Thleds == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thleds (void *argument) { 
	//...definimos aqui la configuracion de los pines de los leds:
	GPIO_InitTypeDef led1 = {
    .Pin = GPIO_PIN_0,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW
  };
 
//GPIO_InitTypeDef led2 = {
//    .Pin = GPIO_PIN_7,
//    .Mode = GPIO_MODE_OUTPUT_PP,
//    .Pull = GPIO_NOPULL,
//    .Speed = GPIO_SPEED_FREQ_LOW
//};

  GPIO_InitTypeDef led3 = {
    .Pin = GPIO_PIN_14,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW
  };
 		__HAL_RCC_GPIOB_CLK_ENABLE();
	  pinB0.pin = led1;
    pinB0.port = GPIOB;
//    pinB0.delay = 0;
//    pinB0.counter = 0;
	
	
//		pinB7.pin = led2;
//    pinB7.port = GPIOB;
//    pinB7.delay = 0;
//    pinB7.counter = 0;	
	
	  pinB14.pin = led3;
    pinB14.port = GPIOB;
//    pinB14.delay = 0;
//    pinB14.counter = 0;	
	
	/*
	Aqui no haria falta usar   mygpio_pin *gpio = (mygpio_pin *)argument; 
	porque argument es NULL en tid_Thleds = osThreadNew(Thleds, NULL, NULL)
	
	HAL_GPIO_Init(gpio->port, &(gpio->pin)); No podemos escribirlo asi
	porque no hemos utilizado   mygpio_pin *gpio = (mygpio_pin *)argument;
	*/
	
	HAL_GPIO_Init(GPIOB,&led1);
	//HAL_GPIO_Init(GPIOB,&led2);
	HAL_GPIO_Init(GPIOB,&led3);
	
  while (1) {
    ; // Insert thread code here...
    //osThreadYield();                            // suspend thread
  }
}
