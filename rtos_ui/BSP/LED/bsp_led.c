#include "bsp_led.h"


void LED_Init(void)
{
	RCC->APB2ENR |= 1<<2;		// ʹ��GPIOA��ʱ��
	RCC->APB2ENR |= 1<<5;		// ʹ��GPIOD��ʱ��
	
		
 
	GPIOA->CRH &= 0xFFFFFFF0;		 
	GPIOA->CRH |= 0x00000003;		 
		
	GPIOA->BSRR &= ~(1<<8);	    // ����ö˿�
	
	GPIOD->CRL &= 0xFFFFF0FF;		 
	GPIOD->CRL |= 0x00000300;		 
	
	GPIOD->BSRR &= ~(1<<2);			// ����ö˿�
	


}
