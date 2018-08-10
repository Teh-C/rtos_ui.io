/**
  ******************************************************************************
  *			 			 		  stm32f10xϵͳ��ʼ��
  *                      (c) Copyright 2013-2020, xxx��˾, ����
  *                              All Rights Reserved
  *  
  * 
  * @�ļ���		system_stm32f10x.c
  * @����  		 
  * @�汾 		V1.0
  * @����    	2017-03-15
  * @˵��   	ϵͳʱ�ӳ�ʼ����NVIC�ж����ȼ�������
  ******************************************************************************
  * @ע������
  *
  ******************************************************************************
  * @�޸ļ�¼
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
*                                        NVIC���ȼ�������
*
* ����  : �������ȼ����е���ռ���ȼ��������ȼ���
*
* ����  : NVIC_PriorityGroup	ָ�����ȼ���λ�ĳ���
*					@arg NVIC_PriorityGroup_0: ��ռ���ȼ�ռ�� 0 λ
*                                				�����ȼ�ռ�� 4 λ
*   			  	@arg NVIC_PriorityGroup_1: ��ռ���ȼ�ռ�� 1 λ
*                                				�����ȼ�ռ�� 3 λ
*     				@arg NVIC_PriorityGroup_2: ��ռ���ȼ�ռ�� 2 λ
*                                				�����ȼ�ռ�� 2 λ
*   				@arg NVIC_PriorityGroup_3: ��ռ���ȼ�ռ�� 3 λ
*                                				�����ȼ�ռ�� 1 λ
*     				@arg NVIC_PriorityGroup_4: ��ռ���ȼ�ռ�� 4 λ
*                                				�����ȼ�ռ�� 0 λ
*
* ����ֵ: ��
*
* ��ע  : ��
*********************************************************************************************************
*/
void NVIC_PriorityGroupConfig(u32 NVIC_PriorityGroup)
{	
	/* ����NVIC�Ĵ����е�Ӧ���жϿ����븴λ���ƼĴ�AIRCR[10:8]PRIGROUP */
	SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;  /* �޸ļĴ���ARRCR�Ĵ������ݣ�����д����Կ 	*/
}


/*
*********************************************************************************************************
*                                        NVIC���ȼ�������
*
* ����  : �����������λ�ü�ƫ������
*
* ����  : NVIC_VectTab		ָ����������FLASH�л���RAM�С�
*								NVIC_VectTab & 0x10000000 = 1: ��������RAM��
*								NVIC_VectTab & 0x10000000 = 0����������FLASH��
*		  Offset			ƫ����,������0x200���ֽڶ���	
*
* ����ֵ: ��
*
* ��ע  : �������ַ������0x200�ֽڶ��롣
*********************************************************************************************************
*/
void NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
{
  SCB->VTOR = NVIC_VectTab | (Offset & (u32)0x1FFFFF80); /* ����������λ�ü�ƫ����,�������ַ������0x200���� */
}




/*
*********************************************************************************************************
*                                        �ж�������������ʼ��
*
* ����  : �����������λ�ü�ƫ������
*
* ����  : NVIC_VectTab		ָ����������FLASH�л���RAM�С�
*								NVIC_VectTab & 0x10000000 = 1: ��������RAM��
*								NVIC_VectTab & 0x10000000 = 0����������FLASH��
*		  Offset			ƫ����,������0x200���ֽڶ���	
*
* ����ֵ: ��
*
* ��ע  : ��
*********************************************************************************************************
*/
void NVIC_Init(u8 NVIC_Group, u8 NVIC_SubPriority, u8 NVIC_PreemptionPriority, u8 NVIC_Channel)
{
	u32 temp;
	/* �����ж����ȼ��� */
	NVIC_PriorityGroupConfig(NVIC_Group<<8); 			/* ���÷���										*/
	temp = NVIC_PreemptionPriority << (4 - NVIC_Group);  
	temp |= NVIC_SubPriority & (0x0f >> NVIC_Group);
	temp &= 0x0f; 										/* ȡ����λ����ռ���ȼ��������ȼ�����ϣ���4λ��*/

	NVIC->ISER[NVIC_Channel /32] |= (1 << NVIC_Channel % 32); /* ʹ����Ӧ���ж�							*/
	NVIC->IP[NVIC_Channel] |= temp << 4; 				/* ���������ȼ�����ռ���ȼ�	  					*/
}


/**
  * @brief  			 			�ⲿ�ж����Ÿ�������
  * @param  GPIOx					GPIO��
  *					@arg 			AFIO_GPIOA
  *					@arg			...
  *					@arg			AFIO_GPIOB			
  *	@param	BITx 					GPIO�����ź�0~15
  *	@param	TRIM 					�жϴ���Դ���ͣ������ػ��½���
  *					@arg			0x00 �½���
  *					@arg			0x01 ������
  * @retval 						��
  */
/*
*********************************************************************************************************
*                                      �ⲿ�ж����Ÿ�������
*
* ����  : �ⲿ�ж����Ÿ������á�
*
* ����  : GPIOx			GPIO�� 
*							@arg 			AFIO_GPIOA
*							@arg			...
*							@arg			AFIO_GPIOB		
*		  BITx			GPIO�����ź�0~15
*
*		  TRIM		   	�жϴ���Դ���ͣ������ػ��½���
*							@arg			0x00 �½���
*							@arg			0x01 ������
*
* ����ֵ: ��
*
* ��ע  : ��
*********************************************************************************************************
*/
void EXTI_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM)
{
	u8 EXTI_ADDR;
	u8 EXTI_OFFSET;
	EXTI_ADDR = BITx / 4;				/* ��ȡ�Ĵ������												*/
	EXTI_OFFSET = (BITx % 4) * 4; 		/* AFIO_EXTICR�����Ŷ�Ӧ�������								*/
	RCC->APB2ENR |= 0x01;				/* ʹ��AFIOʱ��												    */
	AFIO->EXTICR[EXTI_ADDR] &= ~(0x000F << EXTI_OFFSET);  /* �����Ӧ��λ								*/
	AFIO->EXTICR[EXTI_ADDR] |= GPIOx << EXTI_OFFSET;   	  /* EXTI.BITxӳ�䵽GPIO.BITx					*/

	EXTI->IMR |= 1 << BITx; /* ��Ӧ���ⲿ�ж�/�¼��ߣ��������ж�										*/
 //	EXTI->EMR |= 1 << BITx; /* ��Ӧ���ⲿ�ж�/�¼��ߣ��������¼�										*/
	EXTI->RTSR &= ~(1 << BITx);
	EXTI->FTSR &= ~(1 << BITx);
  if(TRIM)
		EXTI->RTSR |= 1 << BITx; /* ������																*/
	else
		EXTI->FTSR |= 1 << BITx; /* �½���																*/
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
*                                      ϵͳʱ������
*
* ����  : ����΢������ϵͳʱ��
*
* ����  : PLL		
*
* ����ֵ: ��
*
* ��ע  : ��
*********************************************************************************************************
*/
void SystemInit (u8 PLL)
{
  u32 temp = 0;
  /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
  /* Set HSION bit */
  /* ����HSION bit, ʹ���ڲ�����ʱ�� */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
  /* ���趨 SW, HPRE, PPRE1, PPRE2, ADCPRE �� MCO λ */
#ifndef STM32F10X_CL
  /* SW[1:0]= 00; HPRE[3:0] = 0000; PPRE1[2:0] = 000; PPRE2[2:0] = 000;  ADCPRE[15:14]=00; MCO = 100 */
  /* HSI��Ϊϵͳʱ��Դ, AHB��Ԥ��Ƶ, APB1��Ԥ��Ƶ, APB2��Ԥ��Ƶ, ADCԤ��Ƶ��PCLK2 2��Ƶ, MCO SYSCLKʱ����� */
  /* MCO�������50MHz,��Ϊ���ŵ�������Ƶ��Ϊ50MHz */
  RCC->CFGR &= (uint32_t)0xF8FF0000;
#else
  RCC->CFGR &= (uint32_t)0xF0FF0000;
#endif /* STM32F10X_CL */   
  
  /* Reset HSEON, CSSON and PLLON bits */
  /* ���趨 HSEON CSSON �� PLLON λ */
  /* �ر�HSE �ر�ʱ�Ӱ�ȫϵͳ  �ر�PLL*/
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  /* ���趨 HSEBYP */
  /* ����·�ⲿʱ�Ӿ��� */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  /* ���趨PLLSRC PLLXTPRE PLLMUL �� USBPRE/OTGFSPRE λ */
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
    
  NVIC_SetVectorTable(0x08000000, 0x00);	 // ����������
  /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  /* Configure the Flash Latency cycles and enable prefetch buffer */
  /* ����ϵͳʱ��Ƶ�ʣ�HCLK,PCLK2 �� PCLK1 ��Ƶ */
  /* ����FLASH ��ʱ���� �� ʹ��Ԥ���� */

#if (defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined (STM32F10X_LD))
  RCC->CR |= 0x00010000;  	// �ⲿ����ʱ��ʹ��HSE
  while(!(RCC->CR >> 17));	// �ȴ��ⲿʱ�Ӿ���

  /* ��SYSCLKΪ���72MHzʱ����72MHzϵͳ���ײ��ȶ����ɸ��ݲ��Խ����ʵ���Ƶ�� */
  /* APB1���ʱ�Ӳ�����36MHz */
  /* AHB ���ʱ�Ӳ�����72MHz */
  /* APB2���ʱ�Ӳ�����72MHz */
  RCC->CFGR = 0x00000400; 	// APB1 = DIV2; APB2 = DIV1; AHB = DIV1;
  PLL-=2;			
  RCC->CFGR|=PLL<<18;	 	// ���ñ�Ƶϵ��
  RCC->CFGR|=1<<16;	 		// HSE��ΪPLL����ʱ��Դ
  FLASH->ACR|=0x32;	 		// ����FALSHԤ����
  RCC->CR|=0x01000000; 		// ʹ��PLL
  while(!(RCC->CR>>25)); 	// ���PLLʱ�Ӿ�����־
  RCC->CFGR|=0x00000002;	// ѡ��HSE��Ϊϵͳʱ��				
  while(temp!=0x02)     	// ���ϵͳʱ���л�״̬
  {   
	temp=RCC->CFGR>>2;
	temp&=0x03;
  } 
#else
// ��������
#endif 
}



// THUMBָ�֧�ֻ������
// �������·���ʵ��ִ�л��ָ��WFI


// ���߲����ڷ����ж�ʱ������  
__asm void WFI_SET(void)
{
	WFI;		  
}

// �ر������жϣ����ǲ�����FAULT��NMI�жϣ�
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
// ���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
// ����ջ����ַ
// addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

