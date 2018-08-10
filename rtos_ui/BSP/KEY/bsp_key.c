#include "bsp_key.h"
#include "bsp_systick.h" 
 

 
 
 
volatile u32 timestampcount = 0;
// 按键初始化
void Key_Init(void)
{
	RCC->APB2ENR |= 1<<2;		// 使能GPIOA口时钟
	RCC->APB2ENR |= 1<<4;		// 使能GPIOC口时钟
	
 
//	GPIOA->CRH &= 0xFFFFFFFF;	// 清除GPIOA_4、5、6相应的控制位
//	GPIOA->CRH |= 0x80000000;	// 配置GPIOA_4、5、6相应的控制位
//	GPIOA->BSRR |= 1<<15;
	
 
	GPIOC->CRL &= 0xFF0FFFFF;
	GPIOC->CRL |= 0x00800000;
  GPIOC->BSRR |= 1<<5;
}


// 按键扫描
// 包含了电源键
char Key_Scan(char mode)
{
	static u8 key_up = 1; 	// 标记按键不能连按
	unsigned char key_value = 0;
	if(key_up && (KEY0 == 0))
	{
//		delay_ms(10);
// 	vTaskDelay(20);
		key_up = 0;
 
		if(KEY0 == 0)		// 按键1
		{
			key_value = 1;
		}
//		else 	if(KEY1 == 0)	// 按键2
//		{
//			key_value = 2;
//		}
	}
	// 按键处于释放状态，则清除key_up标志
	else if(KEY0 == 1)
	{
			key_up = 1;
	}
	return key_value;
}
 

 

 
