#include "system_stm32f10x.h"
#include "bsp_usart2.h"
 
#include <stdio.h>
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
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
    USART2->DR = (u8) ch;      
	return ch;
}
#endif 



void uart2_init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   	//使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;  	//使能串口时钟 
	GPIOA->CRL&=0xFFFF00FF;	//IO状态设置
	GPIOA->CRL|=0x00008B00;	//IO状态设置 
	RCC->APB1RSTR|=1<<17;   //复位串口1
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; 	// 波特率设置	 
	USART2->CR1|=0x200C;  	//1位停止,无校验位.
	USART2->CR1|=1<<5;    	//接收缓冲区非空中断使能	
	NVIC_Init(2, 0, 1, USART2_IRQn);    	
}






