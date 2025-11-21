#include "cmsis_os2.h"                                                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include <stdlib.h>

#include "ThLeds.h"                                                              //para inicializar el timer

/*----------------------------------------------------------------------------
* 'ThJoy': Thread que maneja las pulsaciones (5 GPIO)
 * CUIDADO Debería de inicializar el timer virtual aquí !!
 *---------------------------------------------------------------------------*/

osThreadId_t tid_ThJoy;                        // thread id
void ThJoy (void *argument);                   // thread function
uint32_t statusJoy = 0x0000;

int cnt_right = 0;
int cnt_left = 0;
int cnt_up = 0;
int cnt_down = 0;
int cnt_center = 0;

osMessageQueueId_t id_MsgQueue;                                               //identificador COLA
int sizeQueue = 0;
uint8_t cincoBits;                                                            //mensaje que se mete en la cola
osStatus_t statusQueue;
int errors_or_timeouts = 0;

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
  
  id_MsgQueue = osMessageQueueNew(16, sizeof(uint8_t), NULL);
  
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
 
 pinB11.pin = joy_right;
 pinB11.port = GPIOB;
 
 pinB10.pin = joy_up;
 pinB10.port = GPIOB;
 
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
    
    statusJoy = osThreadFlagsWait(0x0111, osFlagsWaitAny, osWaitForever);      //en el momento en el que me llegue el flag, salta de linea
    
    switch (statusJoy){
    
      case 0x0001:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);                   //LD1 Verde 2^0
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);                 //LD2 Azul  2^1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                //LD3 Rojo  2^2
        cnt_up++;
        cincoBits = 1;
        osMessageQueuePut(id_MsgQueue, &cincoBits, 0U, 0U);
        break;
      
      case 0x0010:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);                 //LD1 Verde 2^0
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);                   //LD2 Azul  2^1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                //LD3 Rojo  2^2
        cnt_right++;
        cincoBits = 2;
        osMessageQueuePut(id_MsgQueue, &cincoBits, 0U, 0U);
        break;
      
      case 0x0011:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);                   //LD1 Verde 2^0
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);                   //LD2 Azul  2^1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                //LD3 Rojo  2^2
        cnt_down++;
        cincoBits = 4;
        osMessageQueuePut(id_MsgQueue, &cincoBits, 0U, 0U);
        break;
      
      case 0x0100:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);                 //LD1 Verde 2^0
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);                 //LD2 Azul  2^1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                  //LD3 Rojo  2^2
        cnt_left++;
        cincoBits = 8;
        osMessageQueuePut(id_MsgQueue, &cincoBits, 0U, 0U);
        break;
      
      case 0x0101:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);                   //LD1 Verde 2^0
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);                 //LD2 Azul  2^1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                  //LD3 Rojo  2^2
        cnt_center++;
        cincoBits = 16;
        osMessageQueuePut(id_MsgQueue, &cincoBits, 0U, 0U);
        break;
      
      default:                                                                  //los dejamos apagados
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);                 //LD1 Verde 2^0
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);                 //LD2 Azul  2^1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                //LD3 Rojo  2^2
        break;
    }
   
    // osThreadYield();                                                          // suspend thread
    //Sirve para ceder la CPU al siguiente hilo preparado con la misma prioridad
    
  }
}
