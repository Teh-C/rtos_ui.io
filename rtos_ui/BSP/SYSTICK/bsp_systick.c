#include "bsp_systick.h"

static u8  fac_us=0;							// us延时倍乘数			   
static u16 fac_ms=0;							// ms延时倍乘数
	
			   
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟

/*
 ****************************************************************************
 *  @brief	延迟函数初始化(时钟SysTick定时器)
 *	@param	SYSCLK:	系统时钟频率(MHz)
 *	@retva	无
 ****************************************************************************
*/
void delay_init(u8 SYSCLK)
{
 	SysTick->CTRL&=~(1<<2);					// 设置SysTick使用外部参考时钟	 
	fac_us=SYSCLK/8;						// 外部时钟源HCLK/8分频后作为SysTick的外部参考时钟
											// 可获得1us的时钟数

	fac_ms=(u16)fac_us*1000;				// 代表每个ms需要的systick时钟数   
}								    

/*
 ****************************************************************************
 *  @brief	us级延迟
 *	@param	nus: 延时的us数
 *	@retva	无
 ****************************************************************************
*/	    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				// 重装值（计数值）	  		 
	SysTick->VAL = 0x00;        			// 清空计数器,清除COUNTFLAG标记位
	SysTick->CTRL |= (1<<0) ;      			// 开始向下计数 	 
	do
	{
		temp=SysTick->CTRL;					// 读取当前的计数值
	}while((temp&0x01)&&!(temp&(1<<16)));	// 等待时间到达，判断COUNTFLAG位是否置1，每次读取COUNTFLAG位都会清除该位  
	SysTick->CTRL &= ~(1<<0);      	 		// 关闭计数器
	SysTick->VAL = 0x00;       				// 清空计数器	 
}

/*
 ****************************************************************************
 *  @brief	 ms级延迟
 *	@param	 mus: 延时的us数
 *	@retva	 无
 *  @remark  1、计数器为24位，装载值不应超过 2^24
 			 2、参数nms不应超过 2^24 * 8 * 1000	/ SYSCLK
			 3、SYSCLK = 72, nms <=	1864
 ****************************************************************************
*/
void delay_ms(u16 nms)
{		  	  
	u32 temp;	
			   
	SysTick->LOAD=(u32)nms*fac_ms;			//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           			//清空计数器
	SysTick->CTRL=0x01 ;          			//开始倒数  
	do
	{
  		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达   
	SysTick->CTRL=0x00;      	 			//关闭计数器
	SysTick->VAL =0X00;       				//清空计数器	  	  	    
} 



// 时间戳功能相关

// 开启时间戳
// 1.装值、清空计数器、开始计数
void Timestamp_Enable(void)
{
 
}



























