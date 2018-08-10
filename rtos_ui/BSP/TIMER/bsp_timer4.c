#include "bsp_timer4.h"
 
 
// ��ʱ��3��ʼ��
// psc: ��Ƶϵ��
// arr: ��װ����
void Timer4_Init(u16 psc, u16 arr)
{
	RCC->APB1ENR|=1<<2;		// ����ʱ�ӣ�APB1ʱ��Ϊ36MHz, ��ʱ��ʱ��Ϊ36*2

	TIM4->CR1 &= ~(1<<4);
	TIM4->CR1 |= 0<<4; 		// ���ϼ���

	TIM4->CR1 &= ~(3<<5);	
	TIM4->CR1 |= 0<<5;		// �������ģʽ
	TIM4->CR1 |= 1<<7;		// �Զ�ת��ֵ

	TIM4->CNT = 0;				// ��ռ�����
	
	TIM4->PSC =  psc;	    // 0~65535
	TIM4->ARR =  arr;

	TIM4->DIER |= 1<<0;		// ���������ж�
	TIM4->CR1 |= 1<<0; // 

	NVIC_Init(2,3,3, TIM4_IRQn);
}


void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001) 				// �ж��Ƿ�����ж�
	{	
		printf("test");
		TIM4->SR &= ~(1<<0);	    	// ����жϱ�־λ
	}

	
}

 


