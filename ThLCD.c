#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Driver_SPI.h"                                                         //
#include "Arial12x12.h"                                                         //este ha de estar aquí??
#include "string.h"

#include "ThLCD.h"
#include "ThJoy.h"                                                              //para pasarle la cola??????

/*----------------------------------------------------------------------------
 *        'Thread_LCD'
 *---------------------------------------------------------------------------*/

extern ARM_DRIVER_SPI Driver_SPI1;                                              //
ARM_DRIVER_SPI *SPIdrv = &Driver_SPI1;                                          //
ARM_SPI_STATUS stat;                                                            //

TIM_HandleTypeDef tim7;                                                         //
unsigned char buffer[512];                                                      //

uint16_t positionL1 = 0;                                                        //P4E2 lo TIENES que poner aquí pq
uint16_t positionL2 = 0;                                                        //P4E3 si no, da mensaje de error

void delay (uint32_t n_microsegundos);
void PINES_init(void);

void LCD_reset (void);
void LCD_wr_data (unsigned char data);
void LCD_wr_cmd (unsigned char cmd);
void LCD_init(void);
void LCD_update (void);
void LCD_clean (void);

//void pintar_cuadrado8x8(int pos);
//void pintar_borde_cuadrado(void);

//void LCD_symbolToLocalBuffer_L1(uint8_t symbol);
//void linea_L1(void);
//void LCD_symbolToLocalBuffer_L2(uint8_t symbol);
//void linea_L2(void);
void symbolToLocalBuffer(uint8_t line,uint8_t symbol);
//void lineas_L1L2(void);

//void LCD_L1(void);
//void LCD_L2(void);

void LCD_L1L2_JoyStick(void);
void LCD_L1L2_UP(void);
void LCD_L1L2_RIGHT(void);
void LCD_L1L2_DOWN(void);
void LCD_L1L2_LEFT(void);
void LCD_L1L2_CENTER(void);
void LCD_L1L2_JoyStick_error(void);

 // CUIDADO SI NO VES NADA EN EL LDC COMPRUEBA QUE TIENES EL UPDATE Y QUE LOS CABLES ESTÁN BIEN PUESTOS !!

osThreadId_t tid_ThLCD;                        // thread id
void ThLCD (void *argument);                   // thread function
 
int Init_ThLCD (void) {
 
  tid_ThLCD = osThreadNew(ThLCD, NULL, NULL);
  if (tid_ThLCD == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void ThLCD (void *argument) {
  
  LCD_reset();                                                                 //
  LCD_init();                                                                  //
  
//  pintar_cuadrado8x8();                                                         //
//  pintar_borde_cuadrado();                                                      //
//  symbolToLocalBuffer_L1('H');                                                  //Para escribir un único símbolo
//  linea_L1();                                                                   //Para escribir en la L1
//  linea_L2();                                                                   //Para escribir en la L2
//  lineas_L1L2();                                                                //Para escribir en ambas líneas
  
//  LCD_L1();
//  LCD_L2();
  
  LCD_L1L2_JoyStick();
  
  LCD_update();                                                                //SI TE OLVIDAS DE ESTE, NUNCA SE REFLEJARÁ EN LA PANTALLA
  
  while (1) {
    
    //con este osDelay
    //osDelay(2000);  //comentar línea para ver funcionamiento real
    
    sizeQueue = osMessageQueueGetCount (id_MsgQueue);                           //nunca mete nada en la cola ??!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    statusQueue = osMessageQueueGet(id_MsgQueue, &cincoBits, NULL, 10U);
    
    if (statusQueue == osOK){
      LCD_clean();
      if (cincoBits == 1){
        LCD_L1L2_UP();
      }else if (cincoBits == 2){
        LCD_L1L2_RIGHT();
      }else if (cincoBits == 4){
        LCD_L1L2_DOWN();
      }else if (cincoBits == 8){
        LCD_L1L2_LEFT();
      }else if (cincoBits == 16){
        LCD_L1L2_CENTER();
      } else {LCD_L1L2_JoyStick_error();}
      LCD_update();
    } else {
      errors_or_timeouts++; }
    
  }
}

/******************************************************************************/

void delay (uint32_t n_microsegundos) {                                         //Función para el retraso del reset
  
  __HAL_RCC_TIM7_CLK_ENABLE();                                                  // Configurar y arrancar el timer
  
  tim7.Instance = TIM7;
  tim7.Init.Prescaler = 83; //84 Mhz / 84 = 1 MHz
  tim7.Init.Period = n_microsegundos - 1;
  
  HAL_TIM_Base_Init(&tim7);
  HAL_TIM_Base_Start(&tim7);
  
  while((TIM7->CNT) < (n_microsegundos-1)){
  } //Esperar a que se active el flag del registro de Match correspondiente
  
  __HAL_TIM_CLEAR_FLAG(&tim7,TIM_FLAG_UPDATE);                                  //Borrar el flag
  
  HAL_TIM_Base_Stop(&tim7);                                                     // Parar el Timer
  HAL_TIM_Base_DeInit(&tim7);                                                   // y ponerlo a 0 para la siguiente llamada a la función
  
}


void PINES_init(void) { //Inicializa pines de salida GPIO: RESET (PA6), A0 (PF13) y CS (PD14).
  
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  __HAL_RCC_GPIOA_CLK_ENABLE();                                                 //Inicialización PIN RESET: PA6
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
  
  __HAL_RCC_GPIOF_CLK_ENABLE();                                                 //Inicialización PIN A0: PF13
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);
  
  __HAL_RCC_GPIOD_CLK_ENABLE();                                                 //Inicialización PIN CS: PD14
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
  
}


void LCD_reset (void) {
  
  SPIdrv->Initialize(NULL); //Inicialización y configuración del driver SPI
  SPIdrv->PowerControl(ARM_POWER_FULL);
  
  SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_DATA_BITS(8), 20000000);
  //Master, CPOL1 y CPHA1, MBS a LSB, 8 bits datos, f-sclk 20MHZ
  //SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW | ARM_SPI_DATA_BITS(8), 20000000); //10000000 ????????????????????????????????????
  
  //SS (Slave Select) sirve para controlar la selección de esclavos
  //Si no se esta realizando ninguna transferencia coloca la linea en estado 
  //inactivo para evitar transferencias no deseadas	
  SPIdrv->Control (ARM_SPI_CONTROL_SS , ARM_SPI_SS_INACTIVE );
  
  
  PINES_init();
  
  delay(1);                                                                     //Generar la señal de reset
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); 
  delay(5);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
  delay(1001);
  
}


void LCD_wr_data (unsigned char data) { //Escribir un dato en el LCD
  
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);    //CS = 0
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);      //A0 = 1
  
  SPIdrv->Send(&data, sizeof(data)); //Escribir un dato
  
  do {
      stat = SPIdrv->GetStatus();
  } while(stat.busy); //Esperar a que se libere el bus SPI
  
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);      //CS = 1
}


void LCD_wr_cmd (unsigned char cmd) { //Recibir un comando en el LCD
  
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);    //CS = 0
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_RESET);    //A0 = 0
  
  SPIdrv->Send(&cmd, sizeof(cmd)); //Escribir un dato
  
  do{
      stat = SPIdrv->GetStatus();
  }while(stat.busy); //Esperar a que se libere el bus SPI
  
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);      //CS = 1
}


void LCD_init (void){ //Inicialización del LCD
  
  LCD_wr_cmd(0xAE); // Display off
  LCD_wr_cmd(0xA2); // Fija el valor de la relación de la tensión de polarización del LCD a 1/9
  LCD_wr_cmd(0xA0); // El direccionamiento de la RAM de datos del display es la normal
  LCD_wr_cmd(0xC8); // El scan en las salidas COM es el normal
  LCD_wr_cmd(0x22); // Fija la relación de resistencias interna a 2
  LCD_wr_cmd(0x2F); // Power on
  LCD_wr_cmd(0x40); // Display empieza en la línea 0
  LCD_wr_cmd(0xAF); // Display ON
  LCD_wr_cmd(0x81); // Contraste
  LCD_wr_cmd(0x17); // Valor Contraste                                          //Página 46 del Display Controller
  LCD_wr_cmd(0xA4); // Display all points normal
  LCD_wr_cmd(0xA6); // LCD Display normal
  
}

void LCD_update (void) { //nuncaa te olvides de este porque si no, nunca se verá reflejado en el LCD
  
  int i; 
  LCD_wr_cmd(0x00);      // 4 bits de la parte baja de la dirección a 0 
  LCD_wr_cmd(0x10);      // 4 bits de la parte alta de la dirección a 0 
  LCD_wr_cmd(0xB0);      // Página 0 
  
  for(i=0;i<128;i++){ 
    LCD_wr_data(buffer[i]); 
  }

   
  LCD_wr_cmd(0x00);      // 4 bits de la parte baja de la dirección a 0 
  LCD_wr_cmd(0x10);      // 4 bits de la parte alta de la dirección a 0 
  LCD_wr_cmd(0xB1);      // Página 1 
   
  for(i=128;i<256;i++){ 
    LCD_wr_data(buffer[i]); 
  }
  
  LCD_wr_cmd(0x00);
  LCD_wr_cmd(0x10);
  LCD_wr_cmd(0xB2);      //Página 2 


  for(i=256;i<384;i++){ 
    LCD_wr_data(buffer[i]); 
  }
  
  LCD_wr_cmd(0x00);
  LCD_wr_cmd(0x10);
  LCD_wr_cmd(0xB3);      // Pagina 3 
   
   
  for(i=384;i<512;i++){ 
    LCD_wr_data(buffer[i]); 
  }
  
}

void LCD_clean (void){
  
  positionL1 = 0;
  positionL2 = 0;
  
  for(int i = 0; i < 512; i++) {
    buffer[i] = 0x00;
  }
  
  /* OTRA OPCION */
//  memset(buffer, 0 , 512u); //implica añadir la libreria: #include "string.h"
//  LCD_update();
  
}


///******************************************************************************/

//void pintar_cuadrado8x8(int pos){
//  
//  int i;
//  for(i = 0; i<pos; i++){
//  buffer[i]=0xFF;
//  }
//  
//}

///******************************************************************************/

//void pintar_borde_cuadrado(void){
//  
//  buffer[0]=0xFF;
//  buffer[1]=0x81;
//  buffer[2]=0x81;
//  buffer[3]=0x81;
//  buffer[4]=0x81;
//  buffer[5]=0x81;
//  buffer[6]=0x81;
//  buffer[7]=0xFF;
//  
//}

///** Para escribir un único símbolo ********************************************/

//void symbolToLocalBuffer_L1(uint8_t symbol){
//  
//  uint8_t i, value1, value2;
//  uint16_t offset=0;
//  offset=25*(symbol - ' ');
//  
//  for(i=0; i<12; i++){
//    value1=Arial12x12[offset+i*2+1];
//    value2=Arial12x12[offset+i*2+2];
//    buffer [i/*+positionL1*/]=value1;
//    buffer [i+128/*+positionL1*/]=value2;
//  }
//  
//}

///** Para escribir en la L1 ****************************************************/

//void LCD_symbolToLocalBuffer_L1(uint8_t symbol){
//  
//  uint8_t i, value1, value2;
//  uint16_t offset=0;
//  offset=25*(symbol - ' ');
//  
//  for(i=0; i<12; i++){
//    value1=Arial12x12[offset+i*2+1];
//    value2=Arial12x12[offset+i*2+2];
//    buffer [i+positionL1]=value1;                                               //quitamos lo que
//    buffer [i+128+positionL1]=value2;                                           //estaba comentado
//  }
//  
//  positionL1=positionL1+Arial12x12[offset];                                     //una vez pintado, vamos a la siguiente posición
//  
//}

//void linea_L1(void){
//  LCD_symbolToLocalBuffer_L1('P');
//  LCD_symbolToLocalBuffer_L1('r');
//  LCD_symbolToLocalBuffer_L1('u');
//  LCD_symbolToLocalBuffer_L1('e');
//  LCD_symbolToLocalBuffer_L1('b');
//  LCD_symbolToLocalBuffer_L1('a');
//  LCD_symbolToLocalBuffer_L1(' ');
//  LCD_symbolToLocalBuffer_L1('d');
//  LCD_symbolToLocalBuffer_L1('e');
//  LCD_symbolToLocalBuffer_L1(' ');
//  LCD_symbolToLocalBuffer_L1('t');
//  LCD_symbolToLocalBuffer_L1('e');
//  LCD_symbolToLocalBuffer_L1('x');
//  LCD_symbolToLocalBuffer_L1('t');
//  LCD_symbolToLocalBuffer_L1('o');
//  LCD_symbolToLocalBuffer_L1(' ');
//  LCD_symbolToLocalBuffer_L1('L');
//  LCD_symbolToLocalBuffer_L1('1');
//}

///** Para escribir en la L2 ****************************************************/

//void LCD_symbolToLocalBuffer_L2(uint8_t symbol){
//  
//  uint8_t i, value1, value2;
//  uint16_t offset=0;
//  offset=25*(symbol - ' ');
//  
//  for(i=0; i<12; i++){
//    value1=Arial12x12[offset+i*2+1];
//    value2=Arial12x12[offset+i*2+2];
//    buffer [i+128+128+positionL2]=value1;
//    buffer [i+256+128+positionL2]=value2;
//  }
//  
//  positionL2=positionL2+Arial12x12[offset];
//  
//}

//void linea_L2(void){
//  LCD_symbolToLocalBuffer_L2('P');
//  LCD_symbolToLocalBuffer_L2('r');
//  LCD_symbolToLocalBuffer_L2('u');
//  LCD_symbolToLocalBuffer_L2('e');
//  LCD_symbolToLocalBuffer_L2('b');
//  LCD_symbolToLocalBuffer_L2('a');
//  LCD_symbolToLocalBuffer_L2(' ');
//  LCD_symbolToLocalBuffer_L2('d');
//  LCD_symbolToLocalBuffer_L2('e');
//  LCD_symbolToLocalBuffer_L2(' ');
//  LCD_symbolToLocalBuffer_L2('t');
//  LCD_symbolToLocalBuffer_L2('e');
//  LCD_symbolToLocalBuffer_L2('x');
//  LCD_symbolToLocalBuffer_L2('t');
//  LCD_symbolToLocalBuffer_L2('o');
//  LCD_symbolToLocalBuffer_L2(' ');
//  LCD_symbolToLocalBuffer_L2('L');
//  LCD_symbolToLocalBuffer_L2('2');
//}

/** Para escribir en ambas líneas *********************************************/

void symbolToLocalBuffer(uint8_t line,uint8_t symbol){
  
  if(line==1){
    uint8_t i, value1, value2;
    uint16_t offset=0;
    offset=25*(symbol - ' ');
    for(i=0; i<12; i++){
      value1=Arial12x12[offset+i*2+1];
      value2=Arial12x12[offset+i*2+2];
      buffer [i+positionL1]=value1;
      buffer [i+128+positionL1]=value2;
    }
    positionL1=positionL1+Arial12x12[offset];
  }
  
  if(line==2){
    uint8_t i, value1, value2;
    uint16_t offset=0;
    offset=25*(symbol - ' ');
    for(i=0; i<12; i++){
      value1=Arial12x12[offset+i*2+1];
      value2=Arial12x12[offset+i*2+2];
      buffer [i+128+128+positionL2]=value1;
      buffer [i+256+128+positionL2]=value2;
    }
    positionL2=positionL2+Arial12x12[offset];
  }
  
/* OTRA OPCIÓN
  if (line==1){
    symbolToLocalBuffer_L1(symbol);
  }
  if(line==2){
    symbolToLocalBuffer_L2(symbol);
  }
*/
  
}

//void lineas_L1L2(void){
//  symbolToLocalBuffer(1,'P');
//  symbolToLocalBuffer(1,'r');
//  symbolToLocalBuffer(1,'u');
//  symbolToLocalBuffer(1,'e');
//  symbolToLocalBuffer(1,'b');
//  symbolToLocalBuffer(1,'a');
//  symbolToLocalBuffer(1,' ');
//  symbolToLocalBuffer(1,'p');
//  symbolToLocalBuffer(1,'a');
//  symbolToLocalBuffer(1,'r');
//  symbolToLocalBuffer(1,'a');
//  symbolToLocalBuffer(1,' ');
//  symbolToLocalBuffer(1,'v');
//  symbolToLocalBuffer(1,'e');
//  symbolToLocalBuffer(1,'r');
//  symbolToLocalBuffer(1,' ');
//  symbolToLocalBuffer(1,'t');
//  symbolToLocalBuffer(1,'e');
//  symbolToLocalBuffer(1,'x');
//  symbolToLocalBuffer(1,'t');
//  symbolToLocalBuffer(1,'o');
//  
//  symbolToLocalBuffer(2,'e');
//  symbolToLocalBuffer(2,'n');
//  symbolToLocalBuffer(2,' ');
//  symbolToLocalBuffer(2,'a');
//  symbolToLocalBuffer(2,'m');
//  symbolToLocalBuffer(2,'b');
//  symbolToLocalBuffer(2,'a');
//  symbolToLocalBuffer(2,'s');
//  symbolToLocalBuffer(2,' ');
//  symbolToLocalBuffer(2,'l');
//  symbolToLocalBuffer(2,'i');
//  symbolToLocalBuffer(2,'n');
//  symbolToLocalBuffer(2,'e');
//  symbolToLocalBuffer(2,'a');
//  symbolToLocalBuffer(2,'s');
//  symbolToLocalBuffer(2,' ');
//  symbolToLocalBuffer(2,'=');
//  symbolToLocalBuffer(2,')');
//  
//}

///** Para escribir en ambas líneas *********************************************/

//void LCD_L1(void){
//  uint32_t i, valor1=1234;
//  char bufferl1[50];
//  sprintf(bufferl1,"Prueba valor1: %d", valor1);
//  for(i=0; i<19; i++)
//    symbolToLocalBuffer(1, bufferl1[i]);
//}

//void LCD_L2(void){
//  uint8_t i;
//  float valor2=3.14159;
//  char bufferl2[50];
//  sprintf(bufferl2,"Prueba valor2: %.5f", valor2);
//  for(i=0; i<22; i++)
//    symbolToLocalBuffer(2, bufferl2[i]);
//}

/** Para escribir en ambas líneas los gestos del JoyStick *********************/
void LCD_L1L2_JoyStick(void){
  uint32_t i;
  char bufferl1[50];
  sprintf(bufferl1,"Hola =)");
  for(i=0; i<7; i++)
    symbolToLocalBuffer(1, bufferl1[i]);
  uint8_t j;
  char bufferl2[50];
  sprintf(bufferl2,"Pulsa el Joystick");
  for(j=0; j<17; j++)
    symbolToLocalBuffer(2, bufferl2[j]);
}


void LCD_L1L2_UP(void){
  uint32_t i;
  char bufferl1[50];
  sprintf(bufferl1,"Gesto pulsado: UP");
  for(i=0; i<17; i++)
    symbolToLocalBuffer(1, bufferl1[i]);
  uint8_t j;
  char bufferl2[50];
  sprintf(bufferl2,"es decir, arriba");
  for(j=0; j<16; j++)
    symbolToLocalBuffer(2, bufferl2[j]);
}

void LCD_L1L2_RIGHT(void){
  uint32_t i;
  char bufferl1[50];
  sprintf(bufferl1,"Gesto pulsado: RIGHT");
  for(i=0; i<20; i++)
    symbolToLocalBuffer(1, bufferl1[i]);
    uint8_t j;
  char bufferl2[50];
  sprintf(bufferl2,"es decir, derecha");
  for(j=0; j<17; j++)
    symbolToLocalBuffer(2, bufferl2[j]);
}
void LCD_L1L2_DOWN(void){
  uint32_t i;
  char bufferl1[50];
  sprintf(bufferl1,"Gesto pulsado: DOWN");
  for(i=0; i<19; i++)
    symbolToLocalBuffer(1, bufferl1[i]);
    uint8_t j;
  char bufferl2[50];
  sprintf(bufferl2,"es decir, abajo");
  for(j=0; j<15; j++)
    symbolToLocalBuffer(2, bufferl2[j]);
}
void LCD_L1L2_LEFT(void){
  uint32_t i;
  char bufferl1[50];
  sprintf(bufferl1,"Gesto pulsado: LEFT");
  for(i=0; i<19; i++)
    symbolToLocalBuffer(1, bufferl1[i]);
    uint8_t j;
  char bufferl2[50];
  sprintf(bufferl2,"es decir, izquierda");
  for(j=0; j<19; j++)
    symbolToLocalBuffer(2, bufferl2[j]);
}
void LCD_L1L2_CENTER(void){
  uint32_t i;
  char bufferl1[50];
  sprintf(bufferl1,"Gesto pulsado: CENTER");
  for(i=0; i<21; i++)
    symbolToLocalBuffer(1, bufferl1[i]);
    uint8_t j;
  char bufferl2[50];
  sprintf(bufferl2,"es decir, centro");
  for(j=0; j<16; j++)
    symbolToLocalBuffer(2, bufferl2[j]);
}

void LCD_L1L2_JoyStick_error(void){
  uint32_t i;
  char bufferl1[50];
  sprintf(bufferl1,"EROR 404");
  for(i=0; i<8; i++)
    symbolToLocalBuffer(1, bufferl1[i]);
  uint8_t j;
  char bufferl2[50];
  sprintf(bufferl2,"  ERROR 404");
  for(j=0; j<10; j++)
    symbolToLocalBuffer(2, bufferl2[j]);
}