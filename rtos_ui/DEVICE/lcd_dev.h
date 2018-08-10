#ifndef __LCD_DEV_H
#define __LCD_DEV_H
#include "ili9341_drv.h"

// RGB565
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         		 0x001F  
#define BRED             0XF81F
#define GRED 						 0XFFE0
#define GBLUE						 0X07FF
#define RED      	     	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW     	   	 0xFFE0
#define BROWN 					 0XBC40 // 棕色
#define BRRED 					 0XFC07 // 棕红色
#define GRAY  					 0X8430 // 灰色
#define DARKBLUE      	 0X01CF	// 深蓝色
#define LIGHTBLUE      	 0X7D7C	// 浅蓝色  
#define GRAYBLUE       	 0X5458 // 灰蓝色
#define LIGHTGREEN     	 0X841F // 浅绿色
#define LGRAY 					 0XC618 // 浅灰色
#define LGRAYBLUE        0XA651 // 浅灰蓝色
#define LBBLUE           0X2B12 // 浅棕蓝色

// RGB24


extern unsigned char LCD_CACHE[1024];


extern void LCD_Scan_Dir(u8 dir);
extern void LCD_DrawPoint(u16 x, u16 y);
extern void LCD_Clear(u16 color);
extern u16 LCD_ReadPoint(u16 x, u16 y);
extern void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);

extern void LCD_SetDirection(u8 dir);

void LCD_ShowChar(u16 x, u16 y, u8 size, u8 c);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);	
unsigned char LCD_ShowString(u16 x, u16 y, u8 size, char * str_ptr);
void LCD_SetBrush(unsigned short color);
void LCD_SetPen(unsigned short color);
void LCD_SetWidth(unsigned char width);
unsigned char LCD_ShowStringTail(u8 size, char * str_ptr, unsigned char isNewRow);
void LCD_SetShowStringIndex(u16 sx, u16 sy, u16 x, u16 y);


#endif
