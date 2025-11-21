#ifndef __THLEDS_H
#define __THLEDS_H

#include "cmsis_os2.h"                          // CMSIS RTOS header file

  extern osThreadId_t tid_ThLeds;                        // thread id
  int Init_ThLeds (void);

#endif