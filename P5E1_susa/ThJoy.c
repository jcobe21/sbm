#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"                                                    //para el GPIO_InitTypeDef y GPIO_TypeDef
//#include <stdlib.h>

/*----------------------------------------------------------------------------
 *          'ThJoy': Este hilo configura TODAS las pulsaciones
 *---------------------------------------------------------------------------*/

osThreadId_t tid_ThJoy;                        // thread id
void ThJoy (void *argument);                   // thread function

typedef struct {
    GPIO_InitTypeDef pin;
    GPIO_TypeDef *port;
    int delay;
    uint8_t counter;
} mygpio_pin;

mygpio_pin pinB11;
mygpio_pin pinB10;
mygpio_pin pinE12;
mygpio_pin pinE14;
mygpio_pin pinE15;


int Init_ThJoy (void) {
  tid_ThJoy = osThreadNew(ThJoy, NULL, NULL);
  if (tid_ThJoy == NULL) {
    return(-1);
  }
  return(0);
}


void ThJoy (void *argument) {
  
  // PUERTO B
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
 GPIO_InitTypeDef joy_right = {
   .Pin =   GPIO_PIN_11,
   .Mode =  GPIO_MODE_IT_RISING,
   .Pull =  GPIO_PULLDOWN,
   .Speed = GPIO_SPEED_FREQ_HIGH
 };  
 
 GPIO_InitTypeDef joy_up = {
   .Pin = GPIO_PIN_10,
   .Mode = GPIO_MODE_IT_RISING,
   .Pull = GPIO_PULLDOWN,
   .Speed = GPIO_SPEED_FREQ_HIGH
 }; 
 
 // PUERTO E
  __HAL_RCC_GPIOE_CLK_ENABLE();
 
 GPIO_InitTypeDef joy_down = {
   .Pin = GPIO_PIN_12,
   .Mode = GPIO_MODE_IT_RISING,
   .Pull = GPIO_PULLDOWN,
   .Speed = GPIO_SPEED_FREQ_HIGH
 };
 
 GPIO_InitTypeDef joy_left = {
   .Pin = GPIO_PIN_14,                                                          //recuerda, este es puerto E
   .Mode = GPIO_MODE_IT_RISING,                                                 //si fuese del puerto B coincidiria con un led
   .Pull = GPIO_PULLDOWN,
   .Speed = GPIO_SPEED_FREQ_HIGH
 }; 
 
 GPIO_InitTypeDef joy_center = {
   .Pin = GPIO_PIN_15,
   .Mode = GPIO_MODE_IT_RISING,
   .Pull = GPIO_PULLDOWN,
   .Speed = GPIO_SPEED_FREQ_HIGH
 };
 
 pinB11.pin = joy_right;
 pinB11.port = GPIOB;
 
 pinB10.pin = joy_up;
 pinB10.port = GPIOB;
 
 pinE12.pin = joy_down;
 pinE12.port = GPIOE;
 
 pinE14.pin = joy_left;
 pinE14.port = GPIOE;
 
 pinE15.pin = joy_center;
 pinE15.port = GPIOE;
 
 
  HAL_GPIO_Init(GPIOB, &joy_right);
  HAL_GPIO_Init(GPIOB, &joy_up);
  HAL_GPIO_Init(GPIOE, &joy_down);
  HAL_GPIO_Init(GPIOE, &joy_left);
  HAL_GPIO_Init(GPIOE, &joy_center);

  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);                                           //habilito interrupciones
 
  while (1) {
   // osThreadYield();                                                          // suspend thread <= CREO QUE AQUÍ SÍ PODEMOS DES-COMENTAR ESTA LÍNEA
  }
}
