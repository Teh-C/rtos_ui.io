#include "bsp_timer3.h"
 
 
// 定时器3初始化
// psc: 分频系数
// arr: 与装载数
void Timer3_Init(u16 psc, u16 arr)
{
	RCC->APB1ENR|=1<<1;		// 开启时钟，APB1时钟为36MHz, 定时器时钟为36*2

	TIM3->CR1 &= ~(1<<4);
	TIM3->CR1 |= 0<<4; 		// 向上计数

	TIM3->CR1 &= ~(3<<5);	
	TIM3->CR1 |= 0<<5;		// 中央对齐模式
	TIM3->CR1 |= 1<<7;		// 自动转载值

	TIM3->CNT = 0;				// 清空计数器
	
	TIM3->PSC =  psc;	    // 0~65535
	TIM3->ARR =  arr;

	TIM3->DIER |= 1<<0;		// 开启更新中断
	TIM3->CR1 |= 1<<0; // 

	NVIC_Init(2,3,3, TIM3_IRQn);
}


 


