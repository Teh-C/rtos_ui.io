#include "system_stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "bsp_systick.h"
#include "bsp_flash.h" 
 
#include "bsp_key.h"
#include "bsp_beep.h"
#include "bsp_adc1.h"
#include "bsp_led.h"
#include "sysinfotool.h"
#include "bsp_timer4.h"
#include "ILI9341_DRV.H"
#include "lcd_dev.h"
#include <stdio.h>

// 函数声明
static void vTaskTaskUserIF(void *pvParameters);
static void vTaskLED(void *pvParmeters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate(void);

static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;


int main()
{
	SystemInit(9);	 											// 系统初始化								
	delay_init(72);												// 内核定时器初始化
	uart1_init(72, 115200);								// 串口1初始化
	ILI9341_Init();
	LCD_Clear(BLACK);
	LCD_SetPen(RED);
	
 
	LCD_SetShowStringIndex(0,0,0,0);
 
	LCD_ShowStringTail(0, "[ OK ]\tSystem Init...\n", 0);
	LCD_ShowStringTail(0, "[ OK ]\tUsart1 Init...\n", 0);
	

 
//	LCD_Draw_Circle(160,120,50);
 
	
	Key_Init();
	LCD_ShowStringTail(0, "[Wait]\tKey Init...", 0);
	LED_Init();
	LCD_ShowStringTail(0, "[ OK ]\tLED Init...\n", 1);
	vSetupSysInfoTool();
		 __set_PRIMASK(1);
	 LCD_ShowStringTail(0, "[Wait]\tTask Create...", 0);
	 AppTaskCreate();
	 LCD_ShowStringTail(0, "[ OK ]\tTask Create...\n", 1);
	 LCD_ShowStringTail(0, "\nReady to start OS...\n", 0);
	 delay_ms(1000);
	 LCD_Clear(BLACK);
	 vTaskStartScheduler();
	while(1);
	
}
static void vTaskTaskUserIF(void *pvParameters)
 {
		uint8_t ucKeyCode;
		uint8_t pcWriteBuffer[500];
	 
		while(1)
		{
			ucKeyCode = Key_Scan(0);
			if(ucKeyCode != 0)
			{
				switch(ucKeyCode)
				{
					case 1:							
							printf("=====================================================\r\n");
							printf("任务名     任务状态  优先级   剩余栈 任务序号\r\n");
						  vTaskList((char *) &pcWriteBuffer);
							printf("%s\r\n", pcWriteBuffer); 
							printf("\r\n任务名        运行计数         使用率\r\n");
							vTaskGetRunTimeStats((char *) &pcWriteBuffer);
						  printf("%s\r\n", pcWriteBuffer);
						break;
					default:
						break;
				}
			}
 
			vTaskDelay(20);
		}
 
 }

  
 
 static void vTaskLED(void *pvParameters)
 {
 		while(1)
		{
			LED1 = ~LED1;
			vTaskDelay(150);			
		}
 }
 
 
 static void vTaskMsgPro(void *pvParameters)
 {
	 	uint8_t pcWriteBuffer[500];
		while(1)
		{
//			LCD_Clear(BLACK);
			LCD_SetShowStringIndex(0,0,0,0);
			LCD_ShowStringTail(0, "---------------------", 2);
//			vTaskDelay(50);
//			vTaskList((char *) &pcWriteBuffer);
//			vTaskDelay(50);
//			LCD_ShowStringTail(0, (char *) &pcWriteBuffer, 2);
//			vTaskDelay(50);
			vTaskList((char *) &pcWriteBuffer);
			vTaskDelay(50);
			vTaskGetRunTimeStats((char *) &pcWriteBuffer);
			vTaskDelay(50);
			LCD_ShowStringTail(0, (char *) &pcWriteBuffer, 2);
			vTaskDelay(50);
			LCD_ShowStringTail(0, "---------------------", 2);
			vTaskDelay(1500);
//			vTaskDelay(300);
		}
 }
 
 static void vTaskStart(void *pvParameters)
 {
 		while(1)
		{
			vTaskDelay(400);
		}
 }
 
 
static void AppTaskCreate(void)
 {
	xTaskCreate(vTaskTaskUserIF,
								"vTaskUserIF",
								512,
								NULL,
								1,
								&xHandleTaskUserIF);
	
		xTaskCreate(vTaskLED,
		"vTaskLED",
	 512,
	 NULL,
	 3,
	 &xHandleTaskLED);
	 
	 
	 xTaskCreate(vTaskMsgPro,
	 "vTaskMsgPro",
		512,
	 NULL,
	 1,
	 &xHandleTaskMsgPro);
	 
	 xTaskCreate(vTaskStart,
	 "vTaskStart",
	 512,
	 NULL,
	 4,
	 &xHandleTaskStart);
}
	 
 
 
