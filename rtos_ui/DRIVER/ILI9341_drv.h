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

#define DATAOUT(data) 	GPIOB->ODR = data	// 数据输出
#define DATAIN		GPIOB->IDR							// 数据输入




typedef struct 
{
	u16 width;			// LCD宽度
	u16 height;			// LCD高度
	u16 id;					// LCD ID
	u8 dir;		   		// 显示方向	 0，纵 1，横
	u16 wramcmd;		// 开始写gram指令
	u16 setxcmd;	  // 设置X坐标指令
	u16 setycmd;	  // 设置Y坐标指令
}_lcd_dev;

// 扫描方向定义 
#define L2R_U2D  0 // 从左到右，从上到下
#define L2R_D2U  1 // 从左到右，从下往上
#define R2L_U2D  2 // 从右到左，从上到下
#define R2L_D2U  3 // 从右到左，从下往上

// 扫描方向定义
#define U2D_L2R  4 // 从上到下，从左到右
#define U2D_R2L  5 // 从上到下，从右往左
#define D2U_L2R  6 // 从下往上，从左往右
#define D2U_R2L  7 // 从下往上，从右往左



 



// LCD参数
extern _lcd_dev lcddev;	// 管理LCD参数
 
// LCD的画笔颜色和背景色
extern u16 POINT_COLOR;	// 默认黑色
extern u16 BACK_COLOR;	// 背景颜色，默认为白色

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
