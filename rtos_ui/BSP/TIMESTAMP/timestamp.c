#include "timestamp.h"
#include "stm32f10x.h"

 

u32 TimeStampCount = 0;

void TimeStamp_Init(void)
{
	RCC->APB1ENR |= 1<<2;	 // 开启定时器4时钟															   
 
	TIM4->CR1 &= ~(1<<4);
	TIM4->CR1 |= 0<<4; 		 // 向上计数

	TIM4->CR1 &= ~(3<<5);	
	TIM4->CR1 |= 0<<5;		 // 中央对齐模式
	TIM4->CR1 |= 1<<7;		 // 自动转载值

	TIM4->CNT = 0;				 // 清空计数器
	
	TIM4->PSC =  71;	     // 1ms计数完
	TIM4->ARR =  999;

	TIM4->DIER |= 1<<0;		 // 开启更新中断
	TIM4->CR1  |= 1<<0;

	NVIC_Init(2,2,2, TIM4_IRQn);
}
	
 
// TimeStampCount每1ms自增
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001) 				// 判断是否更新中断
	{	
		TimeStampCount++;
		SCB->ICSR |= 1<<28;		// 挂起pendSV位
	}				
	TIM4->SR &= ~(1<<0);	    	// 清除中断标志位
	
}


u32 GetTimeStampCount(void)
{
	return TimeStampCount;
}
