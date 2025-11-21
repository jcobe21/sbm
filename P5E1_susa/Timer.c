#include "cmsis_os2.h"                                                          // CMSIS RTOS header file

#include "stm32f4xx_hal.h"                                                      // Hay que
#include <string.h>                                                             // ponerlos??

#include "ThLeds.h"                                                             //EL TIMER APAGA LOS LEDS CUANDO SE ACABE LA CUENTA
#include "ThJoy.h"                                                              //Y DESHABILITA LAS INTERRUPCIONES EXTERNAS

/*----------------------------------------------------------------------------
 *      Timer Virtual
 *---------------------------------------------------------------------------*/

  osTimerId_t timer;
  void Init_Timer (void);                                                       //creo que se puede borrar
  void Timer_Callback(void *arg);
  
void Init_Timer(void){
  
  //Creamos timer virtual
  timer = osTimerNew(Timer_Callback, osTimerOnce, NULL, NULL);
  //(función que se ejecutará cuando el timer expire, tipo de timer, . . . )
  
  //eso de inicializar y empezarlo por tiempo de ejecución, no va
  //lo inicializamos en otro lado
  
}


void Timer_Callback(void *arg){
  
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);                         //LD1 Verde
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                          //LD3 Rojo
  
  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);                                          //Deshabilitamos interrupciones
}