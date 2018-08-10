#include "bsp_adc1.h"
#include "stdio.h"

char vol_value[10];
void ADC1_Init_BSP(void)
{
	RCC->APB2ENR |= 1 << 9;	// ����ADC1ʱ��
	RCC->APB2ENR |= 1 << 4; // ����GPIOCʱ�� 
 											 
	GPIOC->CRL &= 0xFFFFF0FF;
	GPIOC->CRL |= 0x00000000;
 
	ADC1->SMPR1 |= 7<<6;
	ADC1->JSQR |= 12<<15;
   	ADC1->CR1 |= 1<< 8;  
	ADC1->CR2 |= 1<<15;	// ����ע��ͨ���ⲿ����
	ADC1->CR2 |= 7<<12;	// ��������ע��ͨ������ⲿ�¼�JSWSTART
	ADC1->CR2 |= 1<<0;	// ����ADC������ת��

  	ADC1->CR2 |= 1<<3;	// ��ʼУ׼
	while(ADC1->CR2 & (1<<3));
	ADC1->CR2 |= 1<<2;	// ��ʼУ׼
	while(ADC1->CR2 & (1<<2));
}



unsigned short ADC1_StartConvert(void)
{
	ADC1->CR2 |= 1<<21;	// ����ע��ͨ��ת���ⲿ�¼�
	while(!(ADC1->SR & 1<<2));
	ADC1->SR &= ~(1<<2);
	sprintf(vol_value, "%d", ADC1->JDR1 & 0xFFF);	
	return (ADC1->JDR1& 0xFFF);
}
