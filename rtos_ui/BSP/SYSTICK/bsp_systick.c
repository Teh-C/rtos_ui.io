#include "bsp_systick.h"

static u8  fac_us=0;							// us��ʱ������			   
static u16 fac_ms=0;							// ms��ʱ������
	
			   
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��

/*
 ****************************************************************************
 *  @brief	�ӳٺ�����ʼ��(ʱ��SysTick��ʱ��)
 *	@param	SYSCLK:	ϵͳʱ��Ƶ��(MHz)
 *	@retva	��
 ****************************************************************************
*/
void delay_init(u8 SYSCLK)
{
 	SysTick->CTRL&=~(1<<2);					// ����SysTickʹ���ⲿ�ο�ʱ��	 
	fac_us=SYSCLK/8;						// �ⲿʱ��ԴHCLK/8��Ƶ����ΪSysTick���ⲿ�ο�ʱ��
											// �ɻ��1us��ʱ����

	fac_ms=(u16)fac_us*1000;				// ����ÿ��ms��Ҫ��systickʱ����   
}								    

/*
 ****************************************************************************
 *  @brief	us���ӳ�
 *	@param	nus: ��ʱ��us��
 *	@retva	��
 ****************************************************************************
*/	    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				// ��װֵ������ֵ��	  		 
	SysTick->VAL = 0x00;        			// ��ռ�����,���COUNTFLAG���λ
	SysTick->CTRL |= (1<<0) ;      			// ��ʼ���¼��� 	 
	do
	{
		temp=SysTick->CTRL;					// ��ȡ��ǰ�ļ���ֵ
	}while((temp&0x01)&&!(temp&(1<<16)));	// �ȴ�ʱ�䵽��ж�COUNTFLAGλ�Ƿ���1��ÿ�ζ�ȡCOUNTFLAGλ���������λ  
	SysTick->CTRL &= ~(1<<0);      	 		// �رռ�����
	SysTick->VAL = 0x00;       				// ��ռ�����	 
}

/*
 ****************************************************************************
 *  @brief	 ms���ӳ�
 *	@param	 mus: ��ʱ��us��
 *	@retva	 ��
 *  @remark  1��������Ϊ24λ��װ��ֵ��Ӧ���� 2^24
 			 2������nms��Ӧ���� 2^24 * 8 * 1000	/ SYSCLK
			 3��SYSCLK = 72, nms <=	1864
 ****************************************************************************
*/
void delay_ms(u16 nms)
{		  	  
	u32 temp;	
			   
	SysTick->LOAD=(u32)nms*fac_ms;			//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           			//��ռ�����
	SysTick->CTRL=0x01 ;          			//��ʼ����  
	do
	{
  		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;      	 			//�رռ�����
	SysTick->VAL =0X00;       				//��ռ�����	  	  	    
} 



// ʱ����������

// ����ʱ���
// 1.װֵ����ռ���������ʼ����
void Timestamp_Enable(void)
{
 
}



























