#include "cmsis_os2.h"                                                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
//#include <stdlib.h>

#include "Timer.h"                                                              //

/*----------------------------------------------------------------------------
 *      'ThLEDs': este hilo configura los LEDs
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThLeds;                                                        // thread id
 
void ThLeds (void *argument);                                                   // thread function
int Init_ThLeds (void);
uint32_t statusLeds = 0x0000;


typedef struct {
    GPIO_InitTypeDef pin;
    GPIO_TypeDef *port;
    int delay;
    uint8_t counter;
} mygpio_pin;

mygpio_pin pinB0;
mygpio_pin pinB7;
mygpio_pin pinB14;


int Init_ThLeds (void) {                                                        //Función de inicialización del hilo 
  tid_ThLeds = osThreadNew(ThLeds, NULL, NULL);
  if (tid_ThLeds == NULL) {
    return(-1);
  }
  return(0);
}

//Como queremos que un hilo gestione 3 leds, definimos...
//...en ThLeds() la configuracion de los pines de los leds:
 
void ThLeds (void *argument) {
  
  GPIO_InitTypeDef led1 = {
    .Pin = GPIO_PIN_0,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW
  };

GPIO_InitTypeDef led2 = {
    .Pin = GPIO_PIN_7,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW
};

  GPIO_InitTypeDef led3 = {
    .Pin = GPIO_PIN_14,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Speed = GPIO_SPEED_FREQ_LOW
  };

  //Habilitamos reloj del puerto B
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
    pinB0.pin = led1;
    pinB0.port = GPIOB;
//    pinB0.delay = 0;
//    pinB0.counter = 0;
  
    pinB7.pin = led2;
    pinB7.port = GPIOB;
//    pinB7.delay = 0;
//    pinB7.counter = 0;
  
    pinB14.pin = led3;
    pinB14.port = GPIOB;
//    pinB14.delay = 0;
//    pinB14.counter = 0;
  
  //Inicializamos pines con la configuración descrita (arriba)
  HAL_GPIO_Init(GPIOB,&led1);
  HAL_GPIO_Init(GPIOB,&led2);
  HAL_GPIO_Init(GPIOB,&led3);
  
  //Les ponemos un valor inicial
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);                           //LD1 Verde
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);                         //LD2 Azul
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                        //LD3 Rojo
  
  //contamos 3 segundos
  osTimerStart(timer, 3000U);
  
  while (1) {
    
    //WaitAny = "Rango"     WaitALL = justo los mismo bits ?
    statusLeds = osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);      //en el momento en el que me llegue el flag, salta de linea...
   
    if(statusLeds == 0x0001){ //...por ello este if sobra en este caso, pero es mejor ponerlo !!
      osTimerStart(timer, 3000);
      statusLeds = 0x0000; //este en este caso, sobraría
    }
    
  }
  
}
