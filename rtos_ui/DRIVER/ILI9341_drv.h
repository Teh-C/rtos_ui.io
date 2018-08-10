#ifndef __ILI9341_DRV_H
#define __ILI9341_DRV_H
#include "stm32f10x.h"
#include "system_stm32f10x.h"

#define LCD_LED				GPIOC_Out(10)
#define LCD_CSX_SET		GPIOC->BSRR |= 1<<9
#define LCD_DCX_SET		GPIOC->BSRR |= 1<<8
#define LCD_WRX_SET		GPIOC->BSRR |= 1<<7
#define LCD_RDX_SET		GPIOC->BSRR |= 1<<6

#define LCD_CSX_CLR	  GPIOC->BRR |= 1<<9
#define LCD_DCX_CLR	  GPIOC->BRR |= 1<<8
#define LCD_WRX_CLR	  GPIOC->BRR |= 1<<7
#define LCD_RDX_CLR	  GPIOC->BRR |= 1<<6

#define DATAOUT(data) 	GPIOB->ODR = data	// �������
#define DATAIN		GPIOB->IDR							// ��������




typedef struct 
{
	u16 width;			// LCD���
	u16 height;			// LCD�߶�
	u16 id;					// LCD ID
	u8 dir;		   		// ��ʾ����	 0���� 1����
	u16 wramcmd;		// ��ʼдgramָ��
	u16 setxcmd;	  // ����X����ָ��
	u16 setycmd;	  // ����Y����ָ��
}_lcd_dev;

// ɨ�跽���� 
#define L2R_U2D  0 // �����ң����ϵ���
#define L2R_D2U  1 // �����ң���������
#define R2L_U2D  2 // ���ҵ��󣬴��ϵ���
#define R2L_D2U  3 // ���ҵ��󣬴�������

// ɨ�跽����
#define U2D_L2R  4 // ���ϵ��£�������
#define U2D_R2L  5 // ���ϵ��£���������
#define D2U_L2R  6 // �������ϣ���������
#define D2U_R2L  7 // �������ϣ���������



 



// LCD����
extern _lcd_dev lcddev;	// ����LCD����
 
// LCD�Ļ�����ɫ�ͱ���ɫ
extern u16 POINT_COLOR;	// Ĭ�Ϻ�ɫ
extern u16 BACK_COLOR;	// ������ɫ��Ĭ��Ϊ��ɫ

void LCD_ScanDirection(u8 dir);
//void LCD_WR_CMD(u16 data);
//void LCD_WR_DATA(u16 data);
u16 LCD_RD_DATA(void);
void ILI9341_Init(void);
void LCD_DrawPoint(u16 x, u16 y);
void LCD_Clear(u16 color);
 		
u16 LCD_ReadPoint(u16 x, u16 y);
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);
 void LCD_ClearPoint(u16 x, u16 y, u16 color);
void LCD_SetDirection(u8 dir);

#endif
