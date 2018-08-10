#include "timestamp.h"
#include "stm32f10x.h"

 

u32 TimeStampCount = 0;

void TimeStamp_Init(void)
{
	RCC->APB1ENR |= 1<<2;	 // ������ʱ��4ʱ��															   
 
	TIM4->CR1 &= ~(1<<4);
	TIM4->CR1 |= 0<<4; 		 // ���ϼ���

	TIM4->CR1 &= ~(3<<5);	
	TIM4->CR1 |= 0<<5;		 // �������ģʽ
	TIM4->CR1 |= 1<<7;		 // �Զ�ת��ֵ

	TIM4->CNT = 0;				 // ��ռ�����
	
	TIM4->PSC =  71;	     // 1ms������
	TIM4->ARR =  999;

	TIM4->DIER |= 1<<0;		 // ���������ж�
	TIM4->CR1  |= 1<<0;

	NVIC_Init(2,2,2, TIM4_IRQn);
}
	
 
// TimeStampCountÿ1ms����
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001) 				// �ж��Ƿ�����ж�
	{	
		TimeStampCount++;
		SCB->ICSR |= 1<<28;		// ����pendSVλ
	}				
	TIM4->SR &= ~(1<<0);	    	// ����жϱ�־λ
	
}


u32 GetTimeStampCount(void)
{
	return TimeStampCount;
}
