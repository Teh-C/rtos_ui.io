/**
  ******************************************************************************
  * @file    usart.h
  * @author  
  * @version 
  * @date    21-March-2017
  * @brief   
  *
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#ifndef __BSP_USART1_H
#define __BSP_USART1_H
#include "system_stm32f10x.h"
	

void uart1_init(u32 pclk2,u32 bound);
void debug(char * s);

#endif	   
















