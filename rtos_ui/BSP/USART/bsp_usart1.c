#include "bsp_usart1.h"	 
#include "stdio.h"
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
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
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


//void USART1_IRQHandler(void)
//{
//	u8 ch;
//	if(USART1->SR&1<<5)
//	{
//		ch = USART1->DR;
//		USART1->DR = ch;	
//	}	 
//}


void debug(char * s)
{
	while(*s != '\0')
	{
		while((USART1->SR & 0x40) == 0);
			USART1->DR = (u8) *s++;
	}
}



/*
 ****************************************************************************
 *  @brief	 ��ʼ��USART1
 *	@param	 pclk2:	PCLK2ʱ��Ƶ��(Mhz)
 			 bound:	������ 
 *	@retva	 ��
 *  @remark  ��
 ****************************************************************************
*/
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   	//ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  	//ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0xFFFFF00F;	//IO״̬����
	GPIOA->CRH|=0x000008B0;	//IO״̬���� 
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; 	// ����������	 
	USART1->CR1|=0X200C;  	//1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
//	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	
//	NVIC_Init(2, 3, 3, USART1_IRQn);    	
}
