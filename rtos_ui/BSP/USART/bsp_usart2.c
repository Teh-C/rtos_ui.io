#include "system_stm32f10x.h"
#include "bsp_usart2.h"
 
#include <stdio.h>
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
void _ttywrch()
{}
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 



void uart2_init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   	//ʹ��PORTA��ʱ��  
	RCC->APB1ENR|=1<<17;  	//ʹ�ܴ���ʱ�� 
	GPIOA->CRL&=0xFFFF00FF;	//IO״̬����
	GPIOA->CRL|=0x00008B00;	//IO״̬���� 
	RCC->APB1RSTR|=1<<17;   //��λ����1
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; 	// ����������	 
	USART2->CR1|=0x200C;  	//1λֹͣ,��У��λ.
	USART2->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	
	NVIC_Init(2, 0, 1, USART2_IRQn);    	
}






