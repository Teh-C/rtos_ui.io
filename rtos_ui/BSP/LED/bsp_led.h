#ifndef __BSP_LED_H
#define __BSP_LED_H


#include "stm32f10x.h"
#include "system_stm32f10x.h"

#define LED0	GPIOA_Out(8)
#define LED1	GPIOD_Out(2)
void LED_Init(void);
#endif

