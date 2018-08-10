#include "bsp_led.h"


void LED_Init(void)
{
	RCC->APB2ENR |= 1<<2;		// 使能GPIOA口时钟
	RCC->APB2ENR |= 1<<5;		// 使能GPIOD口时钟
	
		
 
	GPIOA->CRH &= 0xFFFFFFF0;		 
	GPIOA->CRH |= 0x00000003;		 
		
	GPIOA->BSRR &= ~(1<<8);	    // 清除该端口
	
	GPIOD->CRL &= 0xFFFFF0FF;		 
	GPIOD->CRL |= 0x00000300;		 
	
	GPIOD->BSRR &= ~(1<<2);			// 清除该端口
	


}
