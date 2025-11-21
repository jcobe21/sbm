#ifndef __THJOY_H
#define __THJOY_H

#include "cmsis_os2.h"                                                          // ::CMSIS:RTOS2

  extern osThreadId_t tid_ThJoy;
  int Init_ThJoy (void);

  extern osMessageQueueId_t id_MsgQueue;                                        //COLA
  extern int sizeQueue;                                                            //tamaño
  extern uint8_t cincoBits;                                                            //información a enviar
  extern osStatus_t statusQueue;
  extern int errors_or_timeouts;

#endif