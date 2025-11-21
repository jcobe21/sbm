#ifndef __THJOY_H
#define __THJOY_H

#include "cmsis_os2.h"                                                          // ::CMSIS:RTOS2

  extern osThreadId_t tid_ThJoy;                                                //en este caso no hace falta pq
  int Init_ThJoy (void);                                                        //no le enviamos ningula flag a este hilo

#endif