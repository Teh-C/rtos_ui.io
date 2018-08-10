#include "bsp_beep.h"
#include "bsp_systick.h"

// ��������ʼ��
void Beep_Init(void)
{
	RCC->APB2ENR |= 1<<2;		// ʹ��GPIOA��ʱ��
 	    
	GPIOA->CRH &= 0xFFF0FFFF;	 
	GPIOA->CRH |= 0x00030000;
	GPIOA->ODR &= ~(1<<12);	// Ĭ������͵�ƽ����������Ĭ�ϲ��� 
}

void Beep_Set(u8 state)
{
	if(state)

		GPIOE->CRL |= 0x00030000;
	else
		GPIOE->CRL &= 0xFFF0FFFF;
}

// ��������Ӧ
// c: ��Ӧ����
// t: ��ӦƵ��
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

