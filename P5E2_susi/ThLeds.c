#include "cmsis_os2.h"                                                          // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include <stdlib.h>

/*----------------------------------------------------------------------------
 *      'ThLEDs': un hilo 
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThLeds;                                                        // thread id
 
void ThLeds (void *argument);                                                   // thread function
int Init_ThLeds (void);
uint32_t statusLeds = 0x0000;

  int cnt_right = 0;
  int cnt_left = 0;
  int cnt_up = 0;
  int cnt_down = 0;
  int cnt_center = 0;


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

  tid_ThLeds = osThreadNew(ThLeds, NULL, NULL);                                 //Como queremos que un hilo gestione 3 leds...

  if (tid_ThLeds == NULL) {
    return(-1);
  }

  return(0);
}


void ThLeds (void *argument) {

  //...definimos aqui la configuracion de los pines de los leds:
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

  //Habilitamos reloj y asociamos pin con cada "configuración" debida
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
  
  /*
  Aqui no haria falta usar   mygpio_pin *gpio = (mygpio_pin *)argument; 
  porque argument es NULL en tid_Thleds = osThreadNew(Thleds, NULL, NULL)

  HAL_GPIO_Init(gpio->port, &(gpio->pin)); No podemos escribirlo asi
  porque no hemos utilizado   mygpio_pin *gpio = (mygpio_pin *)argument;
  */
  
  //Inicializamos cada led y les ponemos un valor inicial
  HAL_GPIO_Init(GPIOB,&led1);
  HAL_GPIO_Init(GPIOB,&led2);
  HAL_GPIO_Init(GPIOB,&led3);
  
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);                         //LD1 Verde 2^0
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);                         //LD2 Azul  2^1
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                        //LD3 Rojo  2^2
  
  while (1) {
    
    statusLeds = osThreadFlagsWait(0x0111, osFlagsWaitAny, osWaitForever);      //en el momento en el que me llegue el flag, salta de linea
    
    switch (statusLeds){
    
      case 0x0001:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);                   //LD1 Verde 2^0
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);                 //LD2 Azul  2^1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                //LD3 Rojo  2^2
        cnt_up++;
        break;
      
      case 0x0010:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);                 //LD1 Verde 2^0
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);                   //LD2 Azul  2^1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                //LD3 Rojo  2^2
        cnt_right++;
        break;
      
      case 0x0011:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);                   //LD1 Verde 2^0
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);                   //LD2 Azul  2^1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);                //LD3 Rojo  2^2
        cnt_down++;
        break;
      
      case 0x0100:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);                 //LD1 Verde 2^0
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);                 //LD2 Azul  2^1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                  //LD3 Rojo  2^2
        cnt_left++;
        break;
      
      case 0x0101:
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);                   //LD1 Verde 2^0
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);                 //LD2 Azul  2^1
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);                  //LD3 Rojo  2^2
        cnt_center++;
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
