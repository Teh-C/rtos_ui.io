/**
  ******************************************************************************
  * @file    inter_flash.c
  * @author  
  * @version 
  * @date    23-March-2017
  * @brief   
  *
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

#include "bsp_flash.h"
 
#include "string.h"
u16 FLASH_BUF[FLASH_PAGE_SIZE / 2]; // 用于缓存一页的数据


/*
 ****************************************************************************
 *  @brief		解锁FPEC		
 *	@param		无
 *	@retva		返回0：		已解锁
 				返回0xFF:	解锁超时，需复位系统，重新解锁
 *  @remark 	解锁FPEC,才能对FLASH进行编程/擦除
 ****************************************************************************
*/
u8 FALSH_Clear_LOCK(void)
{
	FLASH->KEYR = FLASH_KEY1;	 //	写入键值1
	FLASH->KEYR	= FLASH_KEY2;	 //	写入键值２
	return 1;
//	return FLASH_Wait_Unlock(0x5fff); // 检查是否解锁成功
}

/*
 ****************************************************************************
 *  @brief	等待解锁结束
 *	@param	time:		超时时间设置	
 *	@retva	返回0:		已经解锁
 			返回0xFF:	解锁超时
 *  @remark 
 ****************************************************************************
*/
u8 FLASH_Wait_Unlock(u16 time)
{
	u32 rec;
	rec = FLASH->CR;		 // 获取锁状态
	if(rec & (1<<7))		 // FLASH仍未解锁
	{
		do
		{	
			rec = FLASH->CR;
			if(!(rec & (1<<7)))	 // 已解锁
			break;     
			delay_us(1);
			time --;  		
		}while(time);
		if(time == 0)
			return 0xff;		// 超时
	}
	return 0;		   		// 解锁成功
}


/*
 ****************************************************************************
 *  @brief		对FLASH上锁	
 *	@param		无
 *	@retva		无
 *  @remark 	当对FLASH进行写结束后，对FLASH进行上锁
 ****************************************************************************
*/
void FLASH_LOCK(void)
{
	FLASH->CR |= (1<<7);	// FLASH上锁
}						 


/*
 ****************************************************************************
 *  @brief		获取FLASH_SR信息	
 *	@param		无
 *	@retva		返回0	无误/操作结束
 				返回1	忙
				返回2	编程错误
				返回3	写保护错误
 *  @remark 
 ****************************************************************************
*/
u8 FLASH_Status(void)
{
	u32 status;
	status = FLASH->SR;
	if(status & (1 << 0)) 		// 忙
		return 1;
	else if(status & (1 << 2)) 	// 编程错误
	{
		FLASH->SR &= ~(1 << 2); // 清除该位状态
		return 2;
	}
	else if(status & (1 << 4))	// 写保护错误
	{
		FLASH->SR &= ~(1 << 4); // 清除该位状态
		return 3;			
	}
	else if(status & (1 << 5))	// 操作结束（编程/擦除完成置1）
	{
		FLASH->SR &= ~(1 << 5); // 清除该位状态
		return 0;	 			
	}
	return 0;
}

/*
 ****************************************************************************
 *  @brief		等待操作结束
 *	@param		time:		超时时间设置
 *	@retva		返回0:		操作结束
 				返回0xFF:	超时
 *  @remark 
 ****************************************************************************
*/
u8 FLASH_Wait_EOP(u16 time)
{
   u8 reval;
   do
   {	reval = FLASH_Status();
		if(reval != 1)
			break;           // 非忙状态
		delay_us(1);
		time --;  		
   }while(time);
   if(time == 0)
		reval =  0xff;		// 超时
   	return reval;
}

/*
 ****************************************************************************
 *  @brief		对主存储块进行页擦除	
 *	@param		page:		需要擦除的页号
 *	@retva		返回0:		擦除成功
 				返回0xFF:	擦除超时
 *  @remark 
 ****************************************************************************
*/
u8 FLASH_Page_Erase(u8 page)
{
	u8 res;
	res = FLASH_Wait_EOP(0x5FFF);  // 等待上一次操作结束
	if(res == 0)  				   // 上次操作是否结束
	{	
		FLASH->CR |= (1<<1);			// 选择页擦除	
		FLASH->AR = (page * FLASH_PAGE_SIZE) + FLASH_MAIN_BLOCK_ADDR;// 要擦除地址的页首地址
		FLASH->CR |= (1<<6);  			// 开始擦除
		res = FLASH_Wait_EOP(0x5FFF);  	// 等待操作结束
		if(res != 1) 					// 非忙
			FLASH->CR &= ~(1<<1);		// 取消页擦除		
	}	
	return res;	 
}


/*
 ****************************************************************************
 *  @brief		对FLASH进行片擦除
 *	@param		无
 *	@retva		返回0:		擦除成功
 				返回0xFF:	擦除超时
 *  @remark 
 ****************************************************************************
*/
u8 FLASH_Chip_Erase(void)
{
	u8 res;
	res = FLASH_Wait_EOP(0x5FFF);  	  // 等待上一次操作结束
	if(res == 0)
	{
		FLASH->CR |= (1<<2);  		  // 选择片擦除												
		FLASH->CR |= (1<<6); 		  // 开始擦除 
		res = FLASH_Wait_EOP(0x5FFF); // 等待闪存操作结束
		if(res != 1)
			FLASH->CR &= ~(1<<2); 	  // 取消片擦除
	}
	return res;
}


/*
 ****************************************************************************
 *  @brief	写入半字	
 *	@param	WriteAddr:	要写入的地址
 			WriteData:	要写入的数据
 *	@retva	返回0:		写入成功
 			返回0xff:	写入超时	
 *  @remark 每次只能写入半字
 ****************************************************************************
*/
u8 FLASH_HalfWord_Write(u32 WriteAddr, u16 WriteData)
{
	u8 res;
	res = FLASH_Wait_EOP(0xFF); //等待闪存操作结束
	if(res == 0)
	{
		FLASH->CR |= (1<<0);
		*(volatile  u16 *)WriteAddr = WriteData;
		res = FLASH_Wait_EOP(0xFF);  //等待闪存操作结束
		if(res != 1)
			FLASH->CR &= ~(1<<0);		
	}
	return res;
}

  /*
 ****************************************************************************
 *  @brief		半字读取	
 *	@param		ReadAddr:	要读取的地址
 *	@retva		返回读到的数据
 *  @remark 	实际操作可以直接读取1个字的数据，此处是为了下面进行数据的校验方便
 ****************************************************************************
*/
u16 FLASH_HalfWord_Read(u32 ReadAddr)
{
	return *(volatile u16 *) ReadAddr;	
}

/*
 ****************************************************************************
 *  @brief		对FLASH进行读数据		
 *	@param		ReadAddr:	要读数据的地址
 				buf:	   	缓存首地址
				ReadNum:	要读取的半字数
 *	@retva		无
 *  @remark 	读到的数据为半字
 ****************************************************************************
*/
void FLASH_Read(u32 ReadAddr, u16 *buf, u16 ReadNum)
{
	u16 i;
	for(i = 0; i < ReadNum; i++)
	{
	 	buf[i]	= FLASH_HalfWord_Read(ReadAddr);
		ReadAddr += 2;
	}
}

/*
 ****************************************************************************
 *  @brief		对FLASH进行多半字写		
 *	@param		WriteAddr:	要写入的地址
 				buf:	    要写入FLASH的数据的缓存首地址
				WriteNum:	要写入的半字数
 *	@retva		无	
 *  @remark 	无
 ****************************************************************************
*/
void FLASH_Write_Called(u32 WriteAddr, u16 *buf, u16 WriteNum)
{
	u16 i;
	for(i =0; i < WriteNum; i++)
	{
		FLASH_HalfWord_Write(WriteAddr, buf[i]);
		WriteAddr += 2; 
	}
}


// 1Page = 2KB
// 1次写2个字节
// re_val : 0 无误
// 			1 操作非法地址
// 
/*
 ****************************************************************************
 *  @brief		对FLASH进行完整的写操作	
 *	@param		WriteAddr:	要写入的地址
 				buf:	   	数据缓存首地址
				WriteNum:	要写入的半字数
 *	@retva		返回0:		写入成功
 				返回1:		操作非法地址
				返回0xFF:	解锁超时
 *  @remark 	无
 ****************************************************************************
*/			
u8 FLASH_Write(u32 WriteAddr, u16 *buf, u16 WriteNum)
{
	u16 i;
	u32 pageaddr;  	// 页地址
	u16 pageoffset;	// 页内偏移地址
	u16 pageremain; // 页剩余地址
	u32	real_page;	// 主存储块内偏移地址
	/* 检查操作地址是否非法 */
	if((WriteAddr < FLASH_MIN_ADDR) || (WriteAddr > FLASH_MAX_ADDR))
		return 1; //返回错误码
	// FALSH解锁

	if(FALSH_Clear_LOCK() == 0xFF)
	{
		return 0xFF;   // 超时
	}	
	real_page  = WriteAddr - FLASH_MAIN_BLOCK_ADDR; // 获取主存储块内偏移地址
	pageaddr   = real_page / FLASH_PAGE_SIZE;		// 获取页地址
	pageoffset = (real_page % FLASH_PAGE_SIZE) / 2; 	// 获取页内偏移地址，1个偏移半个字
	pageremain = FLASH_PAGE_SIZE / 2 - pageoffset;  // 页剩余空间（半字节）						   		
	
	/* 检查页剩余空间能否容纳待写入半字数 */
	if(WriteNum <= pageremain)
		pageremain = WriteNum; // 能写入

	while(1)
	{  	/* 读当前页的内容 */
		FLASH_Read(pageaddr * FLASH_PAGE_SIZE + FLASH_MAIN_BLOCK_ADDR, FLASH_BUF, FLASH_PAGE_SIZE /2); 
		/* 检查当前页待写区是否已经擦除过 */
		for(i = 0; i < pageremain; i++)
		{
			if(FLASH_BUF[pageoffset + i] != 0xFFFF)
				break; 							  // 检查到待写区未擦除
		}
		/* 未擦除 */
		if(i < pageremain)
		{   /* 擦除当前页 */
			FLASH_Page_Erase(pageaddr);
			/* 复制1页数据，用于写入当前页（包含原有用数据） */
			for(i = 0; i < pageremain; i++)
			{	
				FLASH_BUF[pageoffset +i] = buf[i];// 复制数据
			}
			/* 写入1页的数据 */
			FLASH_Write_Called(pageaddr * FLASH_PAGE_SIZE + FLASH_MAIN_BLOCK_ADDR,
						FLASH_BUF, FLASH_PAGE_SIZE / 2);
		}else /* 当前页待写区已经擦除过，直接执行写操作 */
		{
			FLASH_Write_Called(WriteAddr, buf, pageremain);
		}
		if(pageremain == WriteNum) 				// 所有待写数据写入完成
			break; 								// 退出整个写操作循环
		/* 待写入数据仍未写完，修改相应指针，继续执行写操作 */
		else
		{
			pageaddr ++;   							 // 指向下一页
			pageoffset = 0;				 			 // 页内偏移清0
			buf += pageremain; 						 // 待写数据指针偏移更新
			WriteAddr += pageremain * 2;				 // 待写地址指针偏移更新
			WriteNum -= pageremain;		 			 // 待写入半字数更新
			if(WriteNum > (FLASH_PAGE_SIZE / 2))		 // 待写入半字数是否能用1页写完
				pageremain = FLASH_PAGE_SIZE / 2;
			else
				pageremain = WriteNum;				 // 可以，将剩余半字数赋给pageremain
		}			
	}
	/* 写结束，对FLASH进行上锁 */
	FLASH_LOCK();
	return 0;	
}






