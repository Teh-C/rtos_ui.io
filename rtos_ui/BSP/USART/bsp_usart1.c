#include "bsp_usart1.h"	 
#include "stdio.h"
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
void _ttywrch()
{}
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
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
 *  @brief	 初始化USART1
 *	@param	 pclk2:	PCLK2时钟频率(Mhz)
 			 bound:	波特率 
 *	@retva	 无
 *  @remark  无
 ****************************************************************************
*/
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   	//使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  	//使能串口时钟 
	GPIOA->CRH&=0xFFFFF00F;	//IO状态设置
	GPIOA->CRH|=0x000008B0;	//IO状态设置 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; 	// 波特率设置	 
	USART1->CR1|=0X200C;  	//1位停止,无校验位.
	//使能接收中断 
//	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	
//	NVIC_Init(2, 3, 3, USART1_IRQn);    	
}
