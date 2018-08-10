#include "bsp_timer3.h"
 
 
// ��ʱ��3��ʼ��
// psc: ��Ƶϵ��
// arr: ��װ����
void Timer3_Init(u16 psc, u16 arr)
{
	RCC->APB1ENR|=1<<1;		// ����ʱ�ӣ�APB1ʱ��Ϊ36MHz, ��ʱ��ʱ��Ϊ36*2

	TIM3->CR1 &= ~(1<<4);
	TIM3->CR1 |= 0<<4; 		// ���ϼ���

	TIM3->CR1 &= ~(3<<5);	
	TIM3->CR1 |= 0<<5;		// �������ģʽ
	TIM3->CR1 |= 1<<7;		// �Զ�ת��ֵ

	TIM3->CNT = 0;				// ��ռ�����
	
	TIM3->PSC =  psc;	    // 0~65535
	TIM3->ARR =  arr;

	TIM3->DIER |= 1<<0;		// ���������ж�
	TIM3->CR1 |= 1<<0; // 

	NVIC_Init(2,3,3, TIM3_IRQn);
}


 


