#include "cmsis_os2.h"                                                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include <stdlib.h>                                                             //

#include "ThLeds.h"                                                             //

/*----------------------------------------------------------------------------
 *      Timer Virtual
 *---------------------------------------------------------------------------*/

  int Init_Timer (void);
  osTimerId_t timer;
  osStatus_t statusTimer;

  void Timer_Callback(void *arg);

int Init_Timer(void){
  timer = osTimerNew(Timer_Callback, osTimerOnce, NULL, NULL);
  
//  if (timer != NULL) {  // Si se ha creado el timer correctamente
//    statusTimer = osTimerStart(timer, 50U); // arranca el temporizador con 100 ms de retardo
//    // si se ha iniciado el temporizador correctamente devuelve osOK, aquí status<=osOK
//    if (statusTimer != osOK) { //si falla poner en marcha el temporizador
//      return -1;  //para indicar error
//    }
//  } /*TENGO QUE MATAR AL TIMER VIRTUAL O SE MATA EL SOLO??*/
  
  return NULL;
  
}


void Timer_Callback(void *arg){
  
  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 1){
    osThreadFlagsSet(tid_ThLeds, 0x0010);
    
  } else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 1){
    osThreadFlagsSet(tid_ThLeds, 0x0001);
    
  } else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12) == 1){
    osThreadFlagsSet(tid_ThLeds, 0x0011);
    
  } else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14) == 1){
    osThreadFlagsSet(tid_ThLeds, 0x0100);
    
  } else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == 1){
    osThreadFlagsSet(tid_ThLeds, 0x0101);
    
  }
  
}

