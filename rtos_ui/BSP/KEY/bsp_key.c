#include "bsp_key.h"
#include "bsp_systick.h" 
 

 
 
 
volatile u32 timestampcount = 0;
// ������ʼ��
void Key_Init(void)
{
	RCC->APB2ENR |= 1<<2;		// ʹ��GPIOA��ʱ��
	RCC->APB2ENR |= 1<<4;		// ʹ��GPIOC��ʱ��
	
 
//	GPIOA->CRH &= 0xFFFFFFFF;	// ���GPIOA_4��5��6��Ӧ�Ŀ���λ
//	GPIOA->CRH |= 0x80000000;	// ����GPIOA_4��5��6��Ӧ�Ŀ���λ
//	GPIOA->BSRR |= 1<<15;
	
 
	GPIOC->CRL &= 0xFF0FFFFF;
	GPIOC->CRL |= 0x00800000;
  GPIOC->BSRR |= 1<<5;
}


// ����ɨ��
// �����˵�Դ��
char Key_Scan(char mode)
{
	static u8 key_up = 1; 	// ��ǰ�����������
	unsigned char key_value = 0;
	if(key_up && (KEY0 == 0))
	{
//		delay_ms(10);
// 	vTaskDelay(20);
		key_up = 0;
 
		if(KEY0 == 0)		// ����1
		{
			key_value = 1;
		}
//		else 	if(KEY1 == 0)	// ����2
//		{
//			key_value = 2;
//		}
	}
	// ���������ͷ�״̬�������key_up��־
	else if(KEY0 == 1)
	{
			key_up = 1;
	}
	return key_value;
}
 

 

 
