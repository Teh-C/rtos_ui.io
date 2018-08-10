#include "lcd_dev.h"
#include "lcd_font.h"

unsigned char LCD_CACHE[1024];
unsigned char pen_width = 1;

unsigned short xStart;
unsigned short yStart;
unsigned short xIndex;
unsigned short yIndex;


void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;
	b=r;	  
	di=3-(r<<1);             // 判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  	LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		// 使用Breasenham算法画圆   
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

 
void LCD_ShowChar(u16 x, u16 y, u8 size, u8 c)
{
	u8 i;
	u8 j;
	u8 temp;
	c -= ' ';
	for(i = 0; i < 16; i++)
	{
		for(j =0; j < 8; j++)
		{
			temp = ASCII_0816[c*16+i];
			temp >>= (7-j);
			if(temp&0x01)
			{
				
				LCD_DrawPoint(x+j, y+i);
			}
			else
			{
				LCD_ClearPoint(x+j, y+i, BACK_COLOR);
			}
		}
	}
}

unsigned char LCD_ShowString(u16 x, u16 y, u8 size, char * str_ptr)
{
		while(*str_ptr != '\0')
		{
			LCD_ShowChar(x,y,size,*str_ptr++);
			x+=8;
		}
		return 0;
}

void LCD_SetShowStringIndex(u16 sx, u16 sy, u16 x, u16 y)
{
	xStart = sx;
	yStart = sy;
	xIndex = x;
	yIndex = y;

}
unsigned char LCD_ShowStringTail(u8 size, char * str_ptr, unsigned char isCover)
{
		if(isCover == 1)
		{
				xIndex = xStart;
		}
		if(isCover == 2)
		{
				xIndex = xStart;
				yIndex += 16;
		}
		while(*str_ptr != '\0')
		{
			if(*str_ptr == '\r')
			{
				str_ptr++;
			}
			if(*str_ptr == '\n')
			{
				xIndex = xStart;
				yIndex += 16;
				str_ptr++;
			}
			else if(*str_ptr == '\t')
			{
				LCD_ShowChar(xIndex,yIndex,size, ' ');
				xIndex += 8;
				LCD_ShowChar(xIndex,yIndex,size, ' ');
				xIndex += 8;
				str_ptr++;
			}
			else
			{
				LCD_ShowChar(xIndex,yIndex,size, *str_ptr++);
				xIndex+=8;
				
			}
			if(xIndex > lcddev.width)
			{
				xIndex = xStart;
				yIndex += 16;
				
			}
			if(yIndex > lcddev.height)
					yIndex = yStart;
		}

		return 0;
}


void LCD_SetBrush(unsigned short color)
{
	BACK_COLOR = color;
}
 

void LCD_SetPen(unsigned short color)
{
	POINT_COLOR = color;
}

void LCD_SetWidth(unsigned char width)
{	
	pen_width = width;
}
 



