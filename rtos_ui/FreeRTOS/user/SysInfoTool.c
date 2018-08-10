#include "sysinfotool.h"
#include "bsp_timer3.h"
#include "bsp_usart1.h"



volatile uint32_t ulHighFrequencyTimerTicks;



void vSetupSysInfoTool(void)
{
	Timer3_Init(71, 9);
	
}

void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001) 				// 判断是否更新中断
	{	
		ulHighFrequencyTimerTicks++;				
		TIM3->SR &= ~(1<<0);	    	// 清除中断标志位
	}

	
}
