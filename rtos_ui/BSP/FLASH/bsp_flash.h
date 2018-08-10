#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H
#include "system_stm32f10x.h"
#include "bsp_systick.h"
 
// FLASH主存储块的基地址
#define FLASH_MAIN_BLOCK_ADDR 	0x08000000


/*	STM32F103X_LD:		0x0800 0000 - 0x0800 7FFF 		...   32Page ,  32K
 *	STM32F103X_MD:		0x0800 0000 - 0x0801 FFFF		...	  128Page, 	128K
 *	STM32F103X_HD:		0x0800 0000 - 0X0807 FFFF		...   256Page,  512K
 */

#ifdef STM32F10X_HD
	#define FLASH_MAIN_BLOCK_SIZE 	512			// 512 KB
	#define FLASH_PAGE_NUM			256					// 页数
	#define FLASH_PAGE_SIZE			2048				// 页大小, 2048B
	#define FLASH_PAGE_SIZE_K		2						// 页大小, 2K

#elif defined STM32F10X_MD
	#define FLASH_MAIN_BLOCK_SIZE 	128			// 128KB
	#define FLASH_PAGE_NUM			128					// 
	#define FLASH_PAGE_SIZE			1024				// 1024 byte
	#define FLASH_PAGE_SIZE_K		1						// 1K

#elif defined STM32F10X_LD
	#define FLASH_MAIN_BLOCK_SIZE	32				// 32K
	#define FLASH_PAGE_NUM			32					// 
	#define FLASH_PAGE_SIZE			1024				// 1024 byte
	#define FLASH_PAGE_SIZE_K		1						// 1K
#else
#error "According to the MCU, please add the corresponding parameters."
#endif


#define FLASH_MAX_ADDR			(FLASH_MAIN_BLOCK_ADDR  + (FLASH_MAIN_BLOCK_SIZE * 1024))
#define FLASH_MIN_ADDR		   	FLASH_MAIN_BLOCK_SIZE

#define FLASH_RDPRT 			0x000000A5
#define FLASH_KEY1 				0x45670123
#define FLASH_KEY2				0xCDEF89AB

u8 FLASH_HalfWord_Write(u32 WriteAddr, u16 WriteData);
u8 FLASH_Write(u32 WriteAddr, u16 *buf, u16 WriteNum);
void FLASH_Write_Called(u32 WriteAddr, u16 *buf, u16 WriteNum);
void FLASH_Read(u32 ReadAddr, u16 *buf, u16 ReadNum);
u16 FLASH_HalfWord_Read(u32 ReadAddr);

u8 FLASH_Chip_Erase(void);
u8 FLASH_Page_Erase(u8 page);
u8 FALSH_Clear_LOCK(void);
void FLASH_LOCK(void);
u8 FLASH_Wait_Unlock(u16 time);

u8 FLASH_Status(void);
u8 FLASH_Wait_EOP(u16 time);
#endif
