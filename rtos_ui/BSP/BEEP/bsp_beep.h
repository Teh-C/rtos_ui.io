#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H
#include "system_stm32f10x.h"

#define BEEP GPIOA_Out(12)

void Beep_Init(void);
void Beep_Set(u8 state);	 
void Beep_Run(u8 c, u16 t);
#endif
