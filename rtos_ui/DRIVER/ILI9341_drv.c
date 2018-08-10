#include "ILI9341_DRV.h"
#include "bsp_systick.h"
 
// 8080 18-bit bus interface
//
//  IM3 IM2 IM1 IM0      	
//   1 	 0	 1	 0
//-----------------------------------------------------
//						    		CSX WRX RDX D/CX
//-----------------------------------------------------
//	Write CMDmand					  L   U   H	  L
//	Read Internal status		 	  L   H   U   H
//	Write parameter or display data   L   U   H   H
//  Read  parameter or display data	  L	  H   U   H
//-----------------------------------------------------


// 8080 18-bit patallel PIN instruction
// IM[3:0]:MCU Interface mode
// RESX:RESET,Signal is active low
// CSX:Chip select,Signal is active low
// D/CX(SCL):When DCX = ‘1’,data is selected
//			 When DCX = ‘0’,CMDmand is selected
// RDX:8080-Serve as a read signal and MCU read data at rsing edge.
// WRX:8080-Serve ad a write signal and MCU write data at rsing edge.
// D[17:0]:data bus and RGB interface mdoe.
// 

//static void LCD_WR_DATA(u16 data);
//static void LCD_WR_CMD(u16 data);

_lcd_dev lcddev;
 u16 POINT_COLOR;	// 默认红色
 u16 BACK_COLOR;		// 背景颜色，默认为白色

#define LCD_WR_CMD(data)	{\
														LCD_DCX_CLR;\
														LCD_RDX_SET;\
														LCD_DCX_CLR;\
														LCD_CSX_CLR;\
														DATAOUT(data);\
														LCD_WRX_CLR;\
														LCD_WRX_SET;\
														LCD_CSX_SET;\
													}

 
#define LCD_WR_DATA(data)	{\
														LCD_DCX_SET;\
														LCD_RDX_SET;\
														LCD_CSX_CLR;\
														DATAOUT(data);\
														LCD_WRX_CLR;\
														LCD_WRX_SET;\
														LCD_CSX_SET;\
													}
													
//// write CMDmand
//static void LCD_WR_CMD(u16 data)
//{
//	LCD_DCX_CLR;
//	LCD_RDX_SET;
//	LCD_DCX_CLR;
//	LCD_CSX_CLR;
//	DATAOUT(data);
//	LCD_WRX_CLR;
//	LCD_WRX_SET;
//	LCD_CSX_SET;
//}


//// write data
//static void LCD_WR_DATA(u16 data)
//{
// 
//	LCD_DCX_SET;
//	LCD_RDX_SET;
//	LCD_CSX_CLR;
//	DATAOUT(data);
//	LCD_WRX_CLR;
//	LCD_WRX_SET;
//	LCD_CSX_SET;
//}

// read status/data
u16 LCD_RD_DATA(void)
{		   
	u16 data;
	GPIOB->CRL &= 0x00000000;
	GPIOB->CRL |= 0x88888888;
	GPIOB->CRH &= 0x00000000;
	GPIOB->CRH |= 0x88888888;
	DATAOUT(0xFF);	 		// 上拉
	LCD_DCX_SET;
	LCD_WRX_SET;
	LCD_CSX_CLR;
	LCD_RDX_CLR;
		
	data = DATAIN;	// 根据芯片数据手册可知，数据输出在RDX上升沿无效，
									// 必须在RDX拉低之后，上升沿之前读取，否则数据会出错。
	LCD_RDX_SET;
	LCD_CSX_SET;
	GPIOB->CRL &= 0x00000000;
	GPIOB->CRL |= 0x33333333;
	GPIOB->CRH &= 0x00000000;
	GPIOB->CRH |= 0x33333333;
	return data;
}

// 写寄存器
void LCD_WriteReg(u16 LCD_CMD,u16 LCD_DATA)
{
	LCD_WR_CMD(LCD_CMD);
	LCD_WR_DATA(LCD_DATA);
}


// 读寄存器
u16 LCD_ReadReg(u16 LCD_CMD)                       
{
	LCD_WR_CMD(LCD_CMD);
	return LCD_RD_DATA();
}

// 设置坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WR_CMD(0x2A);
	LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0xFF);
	LCD_WR_CMD(0x2B);
	LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0xFF);
}


// 画点
void LCD_DrawPoint(u16 x, u16 y)
{
//	LCD_SetCursor(x,y);				// 设置坐标
	LCD_WR_CMD(0x2A);
	LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0xFF);
	LCD_WR_CMD(0x2B);
	LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0xFF);
	LCD_WR_CMD(0x2C);					// 设置写RAM命令
	LCD_WR_DATA(POINT_COLOR); // 写入数据
}
void LCD_ClearPoint(u16 x, u16 y, u16 color)
{
//	LCD_SetCursor(x,y);				// 设置坐标
	LCD_WR_CMD(0x2A);
	LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0xFF);
	LCD_WR_CMD(0x2B);
	LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0xFF);
	LCD_WR_CMD(0x2C);					// 设置写RAM命令
	LCD_WR_DATA(color); // 写入数据
}



void LCD_Brush(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
	u16 i,j;
	u16 xlen = 0;
	xlen = ex - sx + 1;
	for(i = sy; i <= ey; i++)
	{
		LCD_SetCursor(sx,i);
		LCD_WR_CMD(0x2C);
		for(j = 0; j < xlen; j++)
		LCD_WR_DATA(color);
	}
}

// 读点
u16 LCD_ReadPoint(u16 x, u16 y)
{
	u16 r,g,b;
	LCD_SetCursor(x,y);
	LCD_WR_CMD(0x2E);
	GPIOB->CRL = 0x88888888;
	GPIOB->CRH = 0x88888888;
	GPIOB->ODR = 0xFFFF;
	LCD_RD_DATA();
	r = LCD_RD_DATA();
	b = LCD_RD_DATA();
	g = r&0xFF;
	g<<=8;
	GPIOB->CRL = 0x33333333;
	GPIOB->CRH = 0x33333333;
	GPIOB->ODR = 0xFFFF;
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
}


// 填充
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
	u16 i,j;
	u16 xlen = 0;
	xlen = ex - sx + 1;
	for(i = sy; i <= ey; i++)
	{
		LCD_SetCursor(sx,i);
		LCD_WR_CMD(0x2C);
		for(j = 0; j < xlen; j++)
			LCD_WR_DATA(color);
	}
}
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 		 
	LCD_SetCursor(0x00,0x0000); 
	LCD_WR_CMD(0x2C); 		   
	for(index=0;index<totalpoint;index++)LCD_WR_DATA(color);	
} 

// 控制芯片初始化
void ILI9341_Init(void)
{
	u16 LCD_ID = 0;
	RCC->APB2ENR |= 1<<4;	// GPIOC时钟开启
	RCC->APB2ENR |= 1<<3;	// GPIOB时钟开启
	RCC->APB2ENR |= 1<<0;
	AFIO->MAPR |= 4<<24;   	// 关闭JATG，否则PB3-4等引脚冲突
	GPIOC->CRL &= 0x00FFFFFF;	// GPIO6-7
	GPIOC->CRL |= 0x33000000;	// 推挽输出
	GPIOC->CRH &= 0xFFFFF000;	// GPIO8-10
	GPIOC->CRH |= 0x00000333;	// 推挽输出
	GPIOB->CRL &= 0x00000000;	// GPIOB0-7
	GPIOB->CRL |= 0x33333333;	// 推挽输出
	GPIOB->CRH &= 0x00000000;	// GPIOB8-15
	GPIOB->CRH |= 0x33333333;	// 推挽输出

	LCD_WR_CMD(0xD3);					// ID4
	LCD_RD_DATA();						// 参数1，空
	LCD_RD_DATA();						// 参数2，00h
	LCD_ID = LCD_RD_DATA()<<8;	// 参数3，93h
	LCD_ID |= LCD_RD_DATA();		// 参数4，41h
	lcddev.id = LCD_ID;
//	printf("%x",LCD_ID);  		// 打印ID号

	LCD_WR_CMD(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xC1); 
	LCD_WR_DATA(0X30); 
	LCD_WR_CMD(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	LCD_WR_CMD(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x7A); 
	LCD_WR_CMD(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	LCD_WR_CMD(0xF7);  
	LCD_WR_DATA(0x20); 
	LCD_WR_CMD(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_CMD(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	LCD_WR_CMD(0xC1);    //Power control 
	LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
	LCD_WR_CMD(0xC5);    //VCM control 
	LCD_WR_DATA(0x30); 	 //3F
	LCD_WR_DATA(0x30); 	 //3C
	LCD_WR_CMD(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); 
	LCD_WR_CMD(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x48); 
	LCD_WR_CMD(0x3A);   
	LCD_WR_DATA(0x55); 
	LCD_WR_CMD(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A); 
	LCD_WR_CMD(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_CMD(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_CMD(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_CMD(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x0E); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x54); 
	LCD_WR_DATA(0XA9); 
	LCD_WR_DATA(0x43); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 		 
	LCD_WR_CMD(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x15); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x06); 
	LCD_WR_DATA(0x2B); 
	LCD_WR_DATA(0x56); 
	LCD_WR_DATA(0x3C); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_CMD(0x2B);		// Set Page Address 
	LCD_WR_DATA(0x00); 	// [Start High bits]
	LCD_WR_DATA(0x00);	// [Start Low bits]
	LCD_WR_DATA(0x01);	// [End High bits]
	LCD_WR_DATA(0x3f);	// [End Low bits]
	LCD_WR_CMD(0x2A);		// Set Column Address 
	LCD_WR_DATA(0x00);	// [Start High bits]
	LCD_WR_DATA(0x00);	// [Start Low bits]
	LCD_WR_DATA(0x00);	// [End High bits]
	LCD_WR_DATA(0xef);	// [End Low bits]
	LCD_WR_CMD(0x11); 	// Exit Sleep
	delay_ms(120);
	LCD_WR_CMD(0x29); 	// display on	
	LCD_LED=1;	
	LCD_SetDirection(1);		
}


void LCD_ScanDirection(u8 dir)
{
	u16 regval = 0x08;
	u16 dirreg = 0x36;	 	// 方向寄存器地址
	if(lcddev.dir == 1)	    // 是否采用横向显示，若是，需要转换
	{
		switch(dir)
		{
			case 0: dir = 6; break;
			case 1: dir = 7; break;
			case 2: dir = 4; break;
			case 3: dir = 5; break;
			case 4: dir = 1; break;
			case 5: dir = 0; break;
			case 6: dir = 3; break;
			case 7: dir = 2; break;
		}
	}
	if(lcddev.id==0x9341)	// 判断驱动芯片类型 
	{
		switch(dir)		 
		{
			case L2R_U2D: // 从左到右，从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U: // 从左到右，从下往上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D: // 从右往左，从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U: // 从右往左，从下往上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R: // 从上到下，从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L: // 从上到下，从右往左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R: // 从下往上，从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L: // 从下往上，从右往左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		LCD_WriteReg(dirreg,regval);

		LCD_WR_CMD(lcddev.setxcmd);	// 设置行长度
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
		LCD_WR_CMD(lcddev.setycmd); // 设置列长度
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
	}
}

// LCD显示方向设置
void LCD_SetDirection(u8 dir)
{
 	if(dir == 0)  			// 竖向
	{
		lcddev.dir = 0;	    // 标记为竖向
		lcddev.width = 240; // 宽度 240
		lcddev.height = 320;  // 高度 320
		if(lcddev.id == 0x9341)	// 判断LCD ID芯片型号
		{
			lcddev.wramcmd = 0x2c;	 // 写RAM命令
			lcddev.setxcmd = 0x2a;	 // 行写命令
			lcddev.setycmd = 0x2b;	 // 列写命令
		}
	}
	else
	{
		lcddev.dir = 1;	   // 横向
		lcddev.width = 320; // 宽度 320
		lcddev.height = 240;  // 高度 240
		if(lcddev.id == 0x9341) // 判断LCD ID芯片型号
		{
			lcddev.wramcmd = 0x2c;	 // 写RAM命令
			lcddev.setxcmd = 0x2a;	 // 行写入命令
			lcddev.setycmd = 0x2b;	 // 列写入命令
		}	
	}
	LCD_ScanDirection(0); // 设置扫描方向
}


// 画指定位置、大小的圆
// （x,y）:中心点
// 		 r:半径

