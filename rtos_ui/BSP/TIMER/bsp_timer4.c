#include "bsp_timer4.h"
 
 
// 定时器3初始化
// psc: 分频系数
// arr: 与装载数
void Timer4_Init(u16 psc, u16 arr)
{
	RCC->APB1ENR|=1<<2;		// 开启时钟，APB1时钟为36MHz, 定时器时钟为36*2

	TIM4->CR1 &= ~(1<<4);
	TIM4->CR1 |= 0<<4; 		// 向上计数

	TIM4->CR1 &= ~(3<<5);	
	TIM4->CR1 |= 0<<5;		// 中央对齐模式
	TIM4->CR1 |= 1<<7;		// 自动转载值

	TIM4->CNT = 0;				// 清空计数器
	
	TIM4->PSC =  psc;	    // 0~65535
	TIM4->ARR =  arr;

	TIM4->DIER |= 1<<0;		// 开启更新中断
	TIM4->CR1 |= 1<<0; // 

	NVIC_Init(2,3,3, TIM4_IRQn);
}


void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001) 				// 判断是否更新中断
	{	
		printf("test");
		TIM4->SR &= ~(1<<0);	    	// 清除中断标志位
	}

	
}

 


