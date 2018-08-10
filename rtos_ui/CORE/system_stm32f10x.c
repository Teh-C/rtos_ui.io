/**
  ******************************************************************************
  *			 			 		  stm32f10x系统初始化
  *                      (c) Copyright 2013-2020, xxx公司, 广州
  *                              All Rights Reserved
  *  
  * 
  * @文件名		system_stm32f10x.c
  * @作者  		 
  * @版本 		V1.0
  * @日期    	2017-03-15
  * @说明   	系统时钟初始化及NVIC中断优先级组配置
  ******************************************************************************
  * @注意事项
  *
  ******************************************************************************
  * @修改记录
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "system_stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*
*********************************************************************************************************
*                                        NVIC优先级组配置
*
* 描述  : 配置优先级组中的抢占优先级和子优先级。
*
* 参数  : NVIC_PriorityGroup	指定优先级组位的长度
*					@arg NVIC_PriorityGroup_0: 抢占优先级占用 0 位
*                                				子优先级占用 4 位
*   			  	@arg NVIC_PriorityGroup_1: 抢占优先级占用 1 位
*                                				子优先级占用 3 位
*     				@arg NVIC_PriorityGroup_2: 抢占优先级占用 2 位
*                                				子优先级占用 2 位
*   				@arg NVIC_PriorityGroup_3: 抢占优先级占用 3 位
*                                				子优先级占用 1 位
*     				@arg NVIC_PriorityGroup_4: 抢占优先级占用 4 位
*                                				子优先级占用 0 位
*
* 返回值: 无
*
* 备注  : 无
*********************************************************************************************************
*/
void NVIC_PriorityGroupConfig(u32 NVIC_PriorityGroup)
{	
	/* 设置NVIC寄存器中的应用中断控制与复位控制寄存AIRCR[10:8]PRIGROUP */
	SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;  /* 修改寄存器ARRCR寄存器内容，需先写入密钥 	*/
}


/*
*********************************************************************************************************
*                                        NVIC优先级组配置
*
* 描述  : 设置向量表的位置及偏移量。
*
* 参数  : NVIC_VectTab		指定向量表在FLASH中还是RAM中。
*								NVIC_VectTab & 0x10000000 = 1: 向量表在RAM中
*								NVIC_VectTab & 0x10000000 = 0：向量表在FLASH中
*		  Offset			偏移量,必须以0x200个字节对齐	
*
* 返回值: 无
*
* 备注  : 向量表地址必须以0x200字节对齐。
*********************************************************************************************************
*/
void NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
{
  SCB->VTOR = NVIC_VectTab | (Offset & (u32)0x1FFFFF80); /* 设置向量表位置及偏移量,向量表地址必须以0x200对齐 */
}




/*
*********************************************************************************************************
*                                        中断向量控制器初始化
*
* 描述  : 设置向量表的位置及偏移量。
*
* 参数  : NVIC_VectTab		指定向量表在FLASH中还是RAM中。
*								NVIC_VectTab & 0x10000000 = 1: 向量表在RAM中
*								NVIC_VectTab & 0x10000000 = 0：向量表在FLASH中
*		  Offset			偏移量,必须以0x200个字节对齐	
*
* 返回值: 无
*
* 备注  : 无
*********************************************************************************************************
*/
void NVIC_Init(u8 NVIC_Group, u8 NVIC_SubPriority, u8 NVIC_PreemptionPriority, u8 NVIC_Channel)
{
	u32 temp;
	/* 设置中断优先级组 */
	NVIC_PriorityGroupConfig(NVIC_Group<<8); 			/* 设置分组										*/
	temp = NVIC_PreemptionPriority << (4 - NVIC_Group);  
	temp |= NVIC_SubPriority & (0x0f >> NVIC_Group);
	temp &= 0x0f; 										/* 取低四位（抢占优先级和子优先级的组合，共4位）*/

	NVIC->ISER[NVIC_Channel /32] |= (1 << NVIC_Channel % 32); /* 使能响应的中断							*/
	NVIC->IP[NVIC_Channel] |= temp << 4; 				/* 设置子优先级和抢占优先级	  					*/
}


/**
  * @brief  			 			外部中断引脚复用配置
  * @param  GPIOx					GPIO组
  *					@arg 			AFIO_GPIOA
  *					@arg			...
  *					@arg			AFIO_GPIOB			
  *	@param	BITx 					GPIO口引脚号0~15
  *	@param	TRIM 					中断触发源类型，上升沿或下降沿
  *					@arg			0x00 下降沿
  *					@arg			0x01 上升沿
  * @retval 						无
  */
/*
*********************************************************************************************************
*                                      外部中断引脚复用配置
*
* 描述  : 外部中断引脚复用配置。
*
* 参数  : GPIOx			GPIO组 
*							@arg 			AFIO_GPIOA
*							@arg			...
*							@arg			AFIO_GPIOB		
*		  BITx			GPIO口引脚号0~15
*
*		  TRIM		   	中断触发源类型，上升沿或下降沿
*							@arg			0x00 下降沿
*							@arg			0x01 上升沿
*
* 返回值: 无
*
* 备注  : 无
*********************************************************************************************************
*/
void EXTI_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM)
{
	u8 EXTI_ADDR;
	u8 EXTI_OFFSET;
	EXTI_ADDR = BITx / 4;				/* 获取寄存器编号												*/
	EXTI_OFFSET = (BITx % 4) * 4; 		/* AFIO_EXTICR中引脚对应的区域号								*/
	RCC->APB2ENR |= 0x01;				/* 使能AFIO时钟												    */
	AFIO->EXTICR[EXTI_ADDR] &= ~(0x000F << EXTI_OFFSET);  /* 清除相应的位								*/
	AFIO->EXTICR[EXTI_ADDR] |= GPIOx << EXTI_OFFSET;   	  /* EXTI.BITx映射到GPIO.BITx					*/

	EXTI->IMR |= 1 << BITx; /* 对应的外部中断/事件线，不屏蔽中断										*/
 //	EXTI->EMR |= 1 << BITx; /* 对应的外部中断/事件线，不屏蔽事件										*/
	EXTI->RTSR &= ~(1 << BITx);
	EXTI->FTSR &= ~(1 << BITx);
  if(TRIM)
		EXTI->RTSR |= 1 << BITx; /* 上升沿																*/
	else
		EXTI->FTSR |= 1 << BITx; /* 下降沿																*/
}

/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemCoreClock variable.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
/*
*********************************************************************************************************
*                                      系统时钟配置
*
* 描述  : 设置微控制器系统时钟
*
* 参数  : PLL		
*
* 返回值: 无
*
* 备注  : 无
*********************************************************************************************************
*/
void SystemInit (u8 PLL)
{
  u32 temp = 0;
  /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
  /* Set HSION bit */
  /* 设置HSION bit, 使能内部高速时钟 */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
  /* 重设定 SW, HPRE, PPRE1, PPRE2, ADCPRE 和 MCO 位 */
#ifndef STM32F10X_CL
  /* SW[1:0]= 00; HPRE[3:0] = 0000; PPRE1[2:0] = 000; PPRE2[2:0] = 000;  ADCPRE[15:14]=00; MCO = 100 */
  /* HSI作为系统时钟源, AHB不预分频, APB1不预分频, APB2不预分频, ADC预分频：PCLK2 2分频, MCO SYSCLK时钟输出 */
  /* MCO输出不超50MHz,因为引脚的最大输出频率为50MHz */
  RCC->CFGR &= (uint32_t)0xF8FF0000;
#else
  RCC->CFGR &= (uint32_t)0xF0FF0000;
#endif /* STM32F10X_CL */   
  
  /* Reset HSEON, CSSON and PLLON bits */
  /* 重设定 HSEON CSSON 和 PLLON 位 */
  /* 关闭HSE 关闭时钟安全系统  关闭PLL*/
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  /* 重设定 HSEBYP */
  /* 不旁路外部时钟晶振 */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  /* 重设定PLLSRC PLLXTPRE PLLMUL 和 USBPRE/OTGFSPRE 位 */
  /*  */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

#ifdef STM32F10X_CL
  /* Reset PLL2ON and PLL3ON bits */
  RCC->CR &= (uint32_t)0xEBFFFFFF;

  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x00FF0000;

  /* Reset CFGR2 register */
  RCC->CFGR2 = 0x00000000;
#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || (defined STM32F10X_HD_VL)
  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;

  /* Reset CFGR2 register */
  RCC->CFGR2 = 0x00000000;      
#else
  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;
#endif /* STM32F10X_CL */
    
  NVIC_SetVectorTable(0x08000000, 0x00);	 // 设置向量表
  /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  /* Configure the Flash Latency cycles and enable prefetch buffer */
  /* 配置系统时钟频率，HCLK,PCLK2 和 PCLK1 分频 */
  /* 配置FLASH 延时周期 和 使能预缓存 */

#if (defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined (STM32F10X_LD))
  RCC->CR |= 0x00010000;  	// 外部高速时钟使能HSE
  while(!(RCC->CR >> 17));	// 等待外部时钟就绪

  /* 在SYSCLK为最大72MHz时（超72MHz系统容易不稳定，可根据测试进行适当超频） */
  /* APB1最大时钟不超过36MHz */
  /* AHB 最大时钟不超过72MHz */
  /* APB2最大时钟不超过72MHz */
  RCC->CFGR = 0x00000400; 	// APB1 = DIV2; APB2 = DIV1; AHB = DIV1;
  PLL-=2;			
  RCC->CFGR|=PLL<<18;	 	// 设置倍频系数
  RCC->CFGR|=1<<16;	 		// HSE作为PLL输入时钟源
  FLASH->ACR|=0x32;	 		// 开启FALSH预缓存
  RCC->CR|=0x01000000; 		// 使能PLL
  while(!(RCC->CR>>25)); 	// 检查PLL时钟就绪标志
  RCC->CFGR|=0x00000002;	// 选择HSE作为系统时钟				
  while(temp!=0x02)     	// 检查系统时钟切换状态
  {   
	temp=RCC->CFGR>>2;
	temp&=0x03;
  } 
#else
// 其他类型
#endif 
}



// THUMB指令不支持汇编内联
// 采用如下方法实现执行汇编指令WFI


// 休眠并且在发生中断时被唤醒  
__asm void WFI_SET(void)
{
	WFI;		  
}

// 关闭所有中断（但是不包括FAULT和NMI中断）
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
// 开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
// 设置栈顶地址
// addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

