#include "cmsis_os2.h"                                                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include <stdlib.h>                                                             //

#include "ThJoy.h"

/*----------------------------------------------------------------------------
 * Timer Virtual <= se podría meter directamente en e ThJoy
 *
 * DEBERIAMOS DE TENER main
 *                     leds (NO DEBE ser hilo)
 *                     ThJoy (con el timer virtual y creada aqui la cola)
 *                     ThLCD
 *(pero no me dio tiempo a cambiarlo y probarlo antes de la entrega a las 8:30)
 *---------------------------------------------------------------------------*/

  int Init_Timer (void);
  osTimerId_t timer;
  osStatus_t statusTimer;


  void Timer_Callback(void *arg);

int Init_Timer(void){
  
  timer = osTimerNew(Timer_Callback, osTimerOnce, NULL, NULL);
  
  if (timer != NULL) {  // Si se ha creado el timer correctamente
    return 0;
  } else {
    return -1;
  }

  return NULL;

}


void Timer_Callback(void *arg){

  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 1){                               //RIGHT 00010
    osThreadFlagsSet(tid_ThJoy, 0x0010);
    
  } else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == 1){                        //UP 00001
    osThreadFlagsSet(tid_ThJoy, 0x0001);
    
  } else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_12) == 1){                        //DOWN 00100
    osThreadFlagsSet(tid_ThJoy, 0x0011);
    
  } else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_14) == 1){                        //LEFT 01000
    osThreadFlagsSet(tid_ThJoy, 0x0100);
    
  } else if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == 1){                        //CENTER 10000
    osThreadFlagsSet(tid_ThJoy, 0x0101);
    
  }

}

/* Y AQUÏ EN LA CALLBACK PODRÍAMOS TENER EL CAMBIO DE LOS LEDS, LA CUENTA DE GESTOS PULSADOS
 * EL CAMBIO DEL MENSAJE Y EL "osMessageQueuePut"?? */
