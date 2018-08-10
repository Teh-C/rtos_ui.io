#include "bsp_timer.h"
 

typedef struct 
{
	Func *f;
	u8 arg1;
	u8 arg2;
	u16 psc;
	u16 arr;
	u8 ARQ;
	u8 ARQ2;
}_TIMER_STATE;


_TIMER_STATE TIMER_STATE;
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

	TIM3->CNT = 0;			// 清空计数器
	
	
	TIMER_STATE.psc = psc;
	TIMER_STATE.arr = arr;
	TIM3->PSC =  psc;	    // 0~65535
	TIM3->ARR =  arr;

	TIM3->DIER |= 1<<0;		// 开启更新中断
	TIM3->CR1 &= ~(1<<0);

	NVIC_Init(2,3,3, TIM3_IRQn);
}



// 应答配置，配置
void Timer3_ARQ_Config(Func *f, u8 arg1,  u8 arg2)
{
	TIMER_STATE.f = f;
	TIM3->CNT = 0;
	TIMER_STATE.arg1 = arg1;
	TIMER_STATE.arg2 = arg2;
	TIMER_STATE.ARQ = 0;
	TIMER_STATE.ARQ2 = 0;
	TIM3->PSC = TIMER_STATE.psc;
	TIM3->ARR = TIMER_STATE.arr;	
}

void TIM3_ARQ_On()
{
	TIM3->CR1 |= 1<<0;
}
	
void TIM3_ARQ_Off()
{
	TIM3->CR1 &= ~(1<<0);
	TIM3->CNT = 0;
	TIMER_STATE.ARQ  = 0;
	TIMER_STATE.ARQ2 = 0;
	
}

void Timer3_ARQ_Run(void)
{
	TIMER_STATE.f(TIMER_STATE.arg1, TIMER_STATE.arg2);
}

void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001) 				// 判断是否更新中断
	{	
			if(TIMER_STATE.ARQ)
				TIMER_STATE.ARQ2 = 1;
			else
				TIMER_STATE.ARQ = 1;
	}
	TIM3->CR1 &= ~(1<<0);				
	TIM3->SR &= ~(1<<0);	    	// 清除中断标志位
	
}

u8 TIM3_IS_ARQ(void)
{
	return TIMER_STATE.ARQ;
}

u8 TIM3_IS_ARQ2(void)
{
	return TIMER_STATE.ARQ2;
}



