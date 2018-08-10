#ifndef __BSP_TIME_H
#define __BSP_TIME_H
#include "system_stm32f10x.h"



typedef void (Func)(u8, u8);




void Timer3_Init(u16 psc, u16 arr);
void Timer4_Init(u16 psc, u16 arr);
void Timer3_ARQ_Config(Func *f, u8 arg1,  u8 arg2);
void Timer3_ARQ_Run(void);
u8 TIM3_IS_ARQ(void);
u8 TIM3_IS_ARQ2(void);
void TIM3_ARQ_On(void);
void TIM3_ARQ_Off(void);
#endif











