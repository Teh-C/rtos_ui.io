#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "system_stm32f10x.h"



#define KEY1	GPIOA_In(15)			// 1     
#define KEY0	GPIOC_In(5)	  	// 2		 
//#define KEY3	GPIOA_In(15)	  // 3	


 

void Key_Init(void);
char Key_Scan(char mode);
void Power_Check_On(void); 
void Power_Check_Off(void);

 

#endif
