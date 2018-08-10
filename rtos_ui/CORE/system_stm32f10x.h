/**
  ******************************************************************************
  * @file    system_config.h
  * @author  
  * @version 
  * @date    15-March-2017
  * @brief   
  *
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
#ifndef __SYSTEM_CONFIG_H
#define __SYSTEM_CONFIG_H

#include "stm32f10x.h" // 外设寄存器地址定义


/* 函数
 * 1. 优先级组配置
 * 2. 向量表设置
 * 3. NVIC初始化
 * 4. 外部中断配置
 * 5. 系统时钟初始化
 */



// 位带操作
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 


// GPIO
/////////////////////////////////////////////////////////////////
// GPIO寄存器地址映射
// GPIOG 0x40012000 - 0x400123FF
// GPIOF 0x40012000 - 0x400123FF 
// GPIOE 0x40011800 - 0x40011BFF 
// GPIOD 0x40011400 - 0x400117FF 
// GPIOC 0x40011000 - 0x400113FF 
// GPIOB 0X40010C00 - 0x40010FFF 
// GPIOA 0x40010800 - 0x40010BFF 

// GPIOx_IDR  偏移0x08
#define GPIOA_IDR_ADDR (GPIOA_BASE + 8) // 0x40010800 + 0x08
#define GPIOB_IDR_ADDR (GPIOB_BASE + 8)
#define GPIOC_IDR_ADDR (GPIOC_BASE + 8)
#define GPIOD_IDR_ADDR (GPIOD_BASE + 8)
#define GPIOE_IDR_ADDR (GPIOE_BASE + 8)
#define GPIOF_IDR_ADDR (GPIOF_BASE + 8)
#define GPIOG_IDR_ADDR (GPIOG_BASE + 8)

// GPIO_ODR	  偏移0x0c
#define GPIOA_ODR_ADDR (GPIOA_BASE + 12) // 0x40010800 + 0x08
#define GPIOB_ODR_ADDR (GPIOB_BASE + 12)
#define GPIOC_ODR_ADDR (GPIOC_BASE + 12)
#define GPIOD_ODR_ADDR (GPIOD_BASE + 12)
#define GPIOE_ODR_ADDR (GPIOE_BASE + 12)
#define GPIOF_ODR_ADDR (GPIOF_BASE + 12)
#define GPIOG_ODR_ADDR (GPIOG_BASE + 12)

// GPIO 位操作
// Pin: 0~15
#define GPIOA_Out(pin) BIT_ADDR(GPIOA_ODR_ADDR, pin)  // GPIO OutPut
#define GPIOA_In(pin) BIT_ADDR(GPIOA_IDR_ADDR, pin)  // GPIO PutIn

#define GPIOB_Out(pin) BIT_ADDR(GPIOB_ODR_ADDR, pin)
#define GPIOB_In(pin) BIT_ADDR(GPIOB_IDR_ADDR, pin)

#define GPIOC_Out(pin) BIT_ADDR(GPIOC_ODR_ADDR, pin)
#define GPIOC_In(pin) BIT_ADDR(GPIOC_IDR_ADDR, pin)

#define GPIOD_Out(pin) BIT_ADDR(GPIOD_ODR_ADDR, pin)
#define GPIOD_In(pin) BIT_ADDR(GPIOD_IDR_ADDR, pin)

#define GPIOE_Out(pin) BIT_ADDR(GPIOE_ODR_ADDR, pin)
#define GPIOE_In(pin) BIT_ADDR(GPIOE_IDR_ADDR, pin)

#define GPIOF_Out(pin) BIT_ADDR(GPIOF_ODR_ADDR, pin)
#define GPIOF_In(pin) BIT_ADDR(GPIOF_IDR_ADDR, pin)

#define GPIOG_Out(pin) BIT_ADDR(GPIOG_ODR_ADDR, pin)
#define GPIOG_In(pin) BIT_ADDR(GPIOG_IDR_ADDR, pin)


// 复用映射
#define AFIO_GPIOA 0
#define AFIO_GPIOB 1
#define AFIO_GPIOC 2
#define AFIO_GPIOD 3
#define AFIO_GPIOE 4
#define AFIO_GPIOG 5
#define AFIO_GPIOF 6

#define EXTI_Trigger_Falling 0	
#define EXTI_Trigger_Rising	 1
////////////////////////////////////////////////////////////// GPIO


// NVIC
#define AIRCR_VECTKEY_MASK 	  0x05FA0000
#define NVIC_PriorityGroup_0 (7 << 8)
#define NVIC_PriorityGroup_1 (6 << 8)
#define NVIC_PriorityGroup_2 (5 << 8)
#define NVIC_PriorityGroup_3 (4 << 8)
#define NVIC_PriorityGroup_4 (3 << 8)


void NVIC_PriorityGroupConfig(u32 NVIC_PriorityGroup);
void NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);
void NVIC_Init(u8 NVIC_Group, u8 NVIC_SubPriority, u8 NVIC_PreemptionPriority, u8 NVIC_Channel);
void EXTI_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM);
void SystemInit (u8 PLL);
void WFI_SET(void);
void MSR_MSP(u32 addr) ;

#endif

