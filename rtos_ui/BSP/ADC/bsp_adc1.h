#ifndef __BSP_ADC1_H
#define __BSP_ADC1_H
#include "system_stm32f10x.h"
#include "stm32f10x.h"
void ADC1_Init_BSP(void);
unsigned short ADC1_StartConvert(void);
extern char vol_value[10];
#endif
