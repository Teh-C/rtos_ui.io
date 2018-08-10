#include "bsp_beep.h"
#include "bsp_systick.h"

// 蜂鸣器初始化
void Beep_Init(void)
{
	RCC->APB2ENR |= 1<<2;		// 使能GPIOA口时钟
 	    
	GPIOA->CRH &= 0xFFF0FFFF;	 
	GPIOA->CRH |= 0x00030000;
	GPIOA->ODR &= ~(1<<12);	// 默认输出低电平，即蜂鸣器默认不响 
}

void Beep_Set(u8 state)
{
	if(state)

		GPIOE->CRL |= 0x00030000;
	else
		GPIOE->CRL &= 0xFFF0FFFF;
}

// 蜂鸣器响应
// c: 响应次数
// t: 响应频率
void Beep_Run(u8 c, u16 t)
{
	while(c)
	{
		BEEP = 1;
		delay_ms(t);
		BEEP = 0;
		delay_ms(t);
		c--;
	}
}

