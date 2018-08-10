#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H 			   
#include "system_stm32f10x.h"  
#include "includes.h"

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif












