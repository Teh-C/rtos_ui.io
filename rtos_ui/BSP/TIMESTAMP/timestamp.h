#ifndef __TIMESTATMP_H
#define __TIMESTATMP_H
#include "system_stm32f10x.h"



typedef struct {
	volatile u32 TimeStampCount;
}_TimeStampState;
 


void TimeStamp_Init(void);
u32 GetTimeStampCount(void);
#endif











