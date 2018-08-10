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
u16 FLASH_BUF[FLASH_PAGE_SIZE / 2]; // ���ڻ���һҳ������


/*
 ****************************************************************************
 *  @brief		����FPEC		
 *	@param		��
 *	@retva		����0��		�ѽ���
 				����0xFF:	������ʱ���踴λϵͳ�����½���
 *  @remark 	����FPEC,���ܶ�FLASH���б��/����
 ****************************************************************************
*/
u8 FALSH_Clear_LOCK(void)
{
	FLASH->KEYR = FLASH_KEY1;	 //	д���ֵ1
	FLASH->KEYR	= FLASH_KEY2;	 //	д���ֵ��
	return 1;
//	return FLASH_Wait_Unlock(0x5fff); // ����Ƿ�����ɹ�
}

/*
 ****************************************************************************
 *  @brief	�ȴ���������
 *	@param	time:		��ʱʱ������	
 *	@retva	����0:		�Ѿ�����
 			����0xFF:	������ʱ
 *  @remark 
 ****************************************************************************
*/
u8 FLASH_Wait_Unlock(u16 time)
{
	u32 rec;
	rec = FLASH->CR;		 // ��ȡ��״̬
	if(rec & (1<<7))		 // FLASH��δ����
	{
		do
		{	
			rec = FLASH->CR;
			if(!(rec & (1<<7)))	 // �ѽ���
			break;     
			delay_us(1);
			time --;  		
		}while(time);
		if(time == 0)
			return 0xff;		// ��ʱ
	}
	return 0;		   		// �����ɹ�
}


/*
 ****************************************************************************
 *  @brief		��FLASH����	
 *	@param		��
 *	@retva		��
 *  @remark 	����FLASH����д�����󣬶�FLASH��������
 ****************************************************************************
*/
void FLASH_LOCK(void)
{
	FLASH->CR |= (1<<7);	// FLASH����
}						 


/*
 ****************************************************************************
 *  @brief		��ȡFLASH_SR��Ϣ	
 *	@param		��
 *	@retva		����0	����/��������
 				����1	æ
				����2	��̴���
				����3	д��������
 *  @remark 
 ****************************************************************************
*/
u8 FLASH_Status(void)
{
	u32 status;
	status = FLASH->SR;
	if(status & (1 << 0)) 		// æ
		return 1;
	else if(status & (1 << 2)) 	// ��̴���
	{
		FLASH->SR &= ~(1 << 2); // �����λ״̬
		return 2;
	}
	else if(status & (1 << 4))	// д��������
	{
		FLASH->SR &= ~(1 << 4); // �����λ״̬
		return 3;			
	}
	else if(status & (1 << 5))	// �������������/���������1��
	{
		FLASH->SR &= ~(1 << 5); // �����λ״̬
		return 0;	 			
	}
	return 0;
}

/*
 ****************************************************************************
 *  @brief		�ȴ���������
 *	@param		time:		��ʱʱ������
 *	@retva		����0:		��������
 				����0xFF:	��ʱ
 *  @remark 
 ****************************************************************************
*/
u8 FLASH_Wait_EOP(u16 time)
{
   u8 reval;
   do
   {	reval = FLASH_Status();
		if(reval != 1)
			break;           // ��æ״̬
		delay_us(1);
		time --;  		
   }while(time);
   if(time == 0)
		reval =  0xff;		// ��ʱ
   	return reval;
}

/*
 ****************************************************************************
 *  @brief		�����洢�����ҳ����	
 *	@param		page:		��Ҫ������ҳ��
 *	@retva		����0:		�����ɹ�
 				����0xFF:	������ʱ
 *  @remark 
 ****************************************************************************
*/
u8 FLASH_Page_Erase(u8 page)
{
	u8 res;
	res = FLASH_Wait_EOP(0x5FFF);  // �ȴ���һ�β�������
	if(res == 0)  				   // �ϴβ����Ƿ����
	{	
		FLASH->CR |= (1<<1);			// ѡ��ҳ����	
		FLASH->AR = (page * FLASH_PAGE_SIZE) + FLASH_MAIN_BLOCK_ADDR;// Ҫ������ַ��ҳ�׵�ַ
		FLASH->CR |= (1<<6);  			// ��ʼ����
		res = FLASH_Wait_EOP(0x5FFF);  	// �ȴ���������
		if(res != 1) 					// ��æ
			FLASH->CR &= ~(1<<1);		// ȡ��ҳ����		
	}	
	return res;	 
}


/*
 ****************************************************************************
 *  @brief		��FLASH����Ƭ����
 *	@param		��
 *	@retva		����0:		�����ɹ�
 				����0xFF:	������ʱ
 *  @remark 
 ****************************************************************************
*/
u8 FLASH_Chip_Erase(void)
{
	u8 res;
	res = FLASH_Wait_EOP(0x5FFF);  	  // �ȴ���һ�β�������
	if(res == 0)
	{
		FLASH->CR |= (1<<2);  		  // ѡ��Ƭ����												
		FLASH->CR |= (1<<6); 		  // ��ʼ���� 
		res = FLASH_Wait_EOP(0x5FFF); // �ȴ������������
		if(res != 1)
			FLASH->CR &= ~(1<<2); 	  // ȡ��Ƭ����
	}
	return res;
}


/*
 ****************************************************************************
 *  @brief	д�����	
 *	@param	WriteAddr:	Ҫд��ĵ�ַ
 			WriteData:	Ҫд�������
 *	@retva	����0:		д��ɹ�
 			����0xff:	д�볬ʱ	
 *  @remark ÿ��ֻ��д�����
 ****************************************************************************
*/
u8 FLASH_HalfWord_Write(u32 WriteAddr, u16 WriteData)
{
	u8 res;
	res = FLASH_Wait_EOP(0xFF); //�ȴ������������
	if(res == 0)
	{
		FLASH->CR |= (1<<0);
		*(volatile  u16 *)WriteAddr = WriteData;
		res = FLASH_Wait_EOP(0xFF);  //�ȴ������������
		if(res != 1)
			FLASH->CR &= ~(1<<0);		
	}
	return res;
}

  /*
 ****************************************************************************
 *  @brief		���ֶ�ȡ	
 *	@param		ReadAddr:	Ҫ��ȡ�ĵ�ַ
 *	@retva		���ض���������
 *  @remark 	ʵ�ʲ�������ֱ�Ӷ�ȡ1���ֵ����ݣ��˴���Ϊ������������ݵ�У�鷽��
 ****************************************************************************
*/
u16 FLASH_HalfWord_Read(u32 ReadAddr)
{
	return *(volatile u16 *) ReadAddr;	
}

/*
 ****************************************************************************
 *  @brief		��FLASH���ж�����		
 *	@param		ReadAddr:	Ҫ�����ݵĵ�ַ
 				buf:	   	�����׵�ַ
				ReadNum:	Ҫ��ȡ�İ�����
 *	@retva		��
 *  @remark 	����������Ϊ����
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
 *  @brief		��FLASH���ж����д		
 *	@param		WriteAddr:	Ҫд��ĵ�ַ
 				buf:	    Ҫд��FLASH�����ݵĻ����׵�ַ
				WriteNum:	Ҫд��İ�����
 *	@retva		��	
 *  @remark 	��
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
// 1��д2���ֽ�
// re_val : 0 ����
// 			1 �����Ƿ���ַ
// 
/*
 ****************************************************************************
 *  @brief		��FLASH����������д����	
 *	@param		WriteAddr:	Ҫд��ĵ�ַ
 				buf:	   	���ݻ����׵�ַ
				WriteNum:	Ҫд��İ�����
 *	@retva		����0:		д��ɹ�
 				����1:		�����Ƿ���ַ
				����0xFF:	������ʱ
 *  @remark 	��
 ****************************************************************************
*/			
u8 FLASH_Write(u32 WriteAddr, u16 *buf, u16 WriteNum)
{
	u16 i;
	u32 pageaddr;  	// ҳ��ַ
	u16 pageoffset;	// ҳ��ƫ�Ƶ�ַ
	u16 pageremain; // ҳʣ���ַ
	u32	real_page;	// ���洢����ƫ�Ƶ�ַ
	/* ��������ַ�Ƿ�Ƿ� */
	if((WriteAddr < FLASH_MIN_ADDR) || (WriteAddr > FLASH_MAX_ADDR))
		return 1; //���ش�����
	// FALSH����

	if(FALSH_Clear_LOCK() == 0xFF)
	{
		return 0xFF;   // ��ʱ
	}	
	real_page  = WriteAddr - FLASH_MAIN_BLOCK_ADDR; // ��ȡ���洢����ƫ�Ƶ�ַ
	pageaddr   = real_page / FLASH_PAGE_SIZE;		// ��ȡҳ��ַ
	pageoffset = (real_page % FLASH_PAGE_SIZE) / 2; 	// ��ȡҳ��ƫ�Ƶ�ַ��1��ƫ�ư����
	pageremain = FLASH_PAGE_SIZE / 2 - pageoffset;  // ҳʣ��ռ䣨���ֽڣ�						   		
	
	/* ���ҳʣ��ռ��ܷ����ɴ�д������� */
	if(WriteNum <= pageremain)
		pageremain = WriteNum; // ��д��

	while(1)
	{  	/* ����ǰҳ������ */
		FLASH_Read(pageaddr * FLASH_PAGE_SIZE + FLASH_MAIN_BLOCK_ADDR, FLASH_BUF, FLASH_PAGE_SIZE /2); 
		/* ��鵱ǰҳ��д���Ƿ��Ѿ������� */
		for(i = 0; i < pageremain; i++)
		{
			if(FLASH_BUF[pageoffset + i] != 0xFFFF)
				break; 							  // ��鵽��д��δ����
		}
		/* δ���� */
		if(i < pageremain)
		{   /* ������ǰҳ */
			FLASH_Page_Erase(pageaddr);
			/* ����1ҳ���ݣ�����д�뵱ǰҳ������ԭ�������ݣ� */
			for(i = 0; i < pageremain; i++)
			{	
				FLASH_BUF[pageoffset +i] = buf[i];// ��������
			}
			/* д��1ҳ������ */
			FLASH_Write_Called(pageaddr * FLASH_PAGE_SIZE + FLASH_MAIN_BLOCK_ADDR,
						FLASH_BUF, FLASH_PAGE_SIZE / 2);
		}else /* ��ǰҳ��д���Ѿ���������ֱ��ִ��д���� */
		{
			FLASH_Write_Called(WriteAddr, buf, pageremain);
		}
		if(pageremain == WriteNum) 				// ���д�д����д�����
			break; 								// �˳�����д����ѭ��
		/* ��д��������δд�꣬�޸���Ӧָ�룬����ִ��д���� */
		else
		{
			pageaddr ++;   							 // ָ����һҳ
			pageoffset = 0;				 			 // ҳ��ƫ����0
			buf += pageremain; 						 // ��д����ָ��ƫ�Ƹ���
			WriteAddr += pageremain * 2;				 // ��д��ַָ��ƫ�Ƹ���
			WriteNum -= pageremain;		 			 // ��д�����������
			if(WriteNum > (FLASH_PAGE_SIZE / 2))		 // ��д��������Ƿ�����1ҳд��
				pageremain = FLASH_PAGE_SIZE / 2;
			else
				pageremain = WriteNum;				 // ���ԣ���ʣ�����������pageremain
		}			
	}
	/* д��������FLASH�������� */
	FLASH_LOCK();
	return 0;	
}






