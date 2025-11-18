#ifndef __THJOY_H
#define __THJOY_H
#include "cmsis_os2.h"  
extern osThreadId_t tid_ThJoy;  
extern osMessageQueueId_t id_MsgQueue; 
	int Init_ThJoy (void);  
#endif