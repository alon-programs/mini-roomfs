/****************************************************************************
* Copyright (C), 2011 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3,MINI上调试通过           
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com 
* 淘宝店铺：ourstm.taobao.com  
*
* 文件名: spi_flash.c
* 内容简述:
*       本例程包含了SST25VF016B的底层驱动函数
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-7-07 sun68  创建该文件
*
*/
#include <stdio.h>
//#include "demo.h"
#include "SPI_Flash.h"
//#include "headerfile.h"
#include "stm32f10x_lib.h"

typedef enum _E_SPIFLASH_CMD_
{
    eWritePage = 0x02,
    eWtiteDisable = 0x04,
    eReadStatue = 0x05,
    eWriteEn = 0x06,
    eFastRead = 0x0B,
    eEraseSector = 0x20,
    eWriteStatue = 0x50,
    eReadID = 0x90,
}eW25QxCmd;
/****************************************************************************
* 名    称：void wip(void)
* 功    能：忙检测
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void W25Qx_Wait_Busy(void);
unsigned char W25Qx_ReadStatue(void);

/****************************************************************************
* 名    称：void wen(void)
* 功    能：写使能
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void W25Qx_WriteEn(void)
{
	Select_Flash();
	SPI_Flash_SendByte(eWriteEn);
	NotSelect_Flash();
}
/****************************************************************************
* 名    称：void wsr(void)
* 功    能：写状态
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void W25Qx_WriteStatue(void)
{	
	Select_Flash();
	SPI_Flash_SendByte(eWriteStatue);
	NotSelect_Flash(); 
	Select_Flash();
	SPI_Flash_SendByte(0x01);
	SPI_Flash_SendByte(0x00); 
	NotSelect_Flash();
    W25Qx_Wait_Busy();
}
/****************************************************************************
* 名    称：void wip(void)
* 功    能：忙检测
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void W25Qx_Wait_Busy(void)
{
    unsigned char a = 1;
    while((a & 0x01) == 1) 
        a = W25Qx_ReadStatue();	

}
/****************************************************************************
* 名    称：void wdis(void)
* 功    能：写禁止
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void W25Qx_WriteDisable(void)
{
	Select_Flash();
	SPI_Flash_SendByte(eWtiteDisable); 
	NotSelect_Flash();
	W25Qx_Wait_Busy();
	
}	

/****************************************************************************
* 名    称：unsigned char rdsr(void)
* 功    能：读状态寄存器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
unsigned char W25Qx_ReadStatue(void)
{
	unsigned char busy;
	Select_Flash();
	SPI_Flash_SendByte(eReadStatue);
	busy = SPI_Flash_ReadByte();
	NotSelect_Flash();
	return(busy);
	
}
/****************************************************************************
* 名    称：void sect_clr(unsigned long a1)
* 功    能：页擦除
* 入口参数：unsigned long a1--页   
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void W25Qx_EraseSector(unsigned int SectorAddr)
{
    u32 DstAddr;
    
	//W25Qx_WriteStatue();
	DstAddr = SectorAddr * W25QX_SECTOR_SIZE;
	W25Qx_WriteEn();
	Select_Flash();
  	SPI_Flash_SendByte(eEraseSector);
	SPI_Flash_SendByte((DstAddr & 0xffffff)>>16);          //addh
	SPI_Flash_SendByte((DstAddr & 0xffff)>>8);          //addl 
	SPI_Flash_SendByte(DstAddr & 0xff);                 //wtt
	NotSelect_Flash();
	W25Qx_Wait_Busy();	
}
/****************************************************************************
* 名    称：void W25Qx_Read(unsigned long addr, unsigned char *readbuff, unsigned int NumOfByte)
* 功    能：页读
* 入口参数：unsigned long addr--页   unsigned char *readbuff--数组   unsigned int NumOfByte--长度
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void W25Qx_Read(unsigned long addr, unsigned char *readbuff, unsigned int NumOfByte)
{
	unsigned int i=0; 	
	Select_Flash();
	SPI_Flash_SendByte(eFastRead);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	SPI_Flash_SendByte(0);
	while(i<NumOfByte){	
		readbuff[i++]=SPI_Flash_ReadByte();
	}
	NotSelect_Flash();	 	
}
void W25Qx_WritePage(u32 addr, u8 *readbuff, u16 PageSize){
	u32 i;

  	W25Qx_WriteEn();
  	
	Select_Flash();    
	SPI_Flash_SendByte(eWritePage);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	
	i = 0;
	while(i < PageSize)
	{
		SPI_Flash_SendByte(readbuff[i]);
		//printf("%02X ",readbuff[i]);
		i++;
	}
	NotSelect_Flash();
		
	W25Qx_Wait_Busy();
	
	W25Qx_WriteDisable();	

}

void W25QXX_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u32 NumByteToWrite)   
{ 			 		 
	u32 pageremain;	   
	pageremain = W25QX_PAGE_SIZE - WriteAddr % W25QX_PAGE_SIZE; //单页剩余的字节数	
	
	if(NumByteToWrite <= pageremain)
	    pageremain = NumByteToWrite;//不大于256个字节
	    
	while(1)
	{	   
		W25Qx_WritePage(WriteAddr,pBuffer, pageremain);
		
		if(NumByteToWrite == pageremain)
		    break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;	
			NumByteToWrite -= pageremain;			  //减去已经写入了的字节数
			
			if(NumByteToWrite > W25QX_PAGE_SIZE)
			    pageremain = W25QX_PAGE_SIZE; //一次可以写入256个字节
			else 
			    pageremain = NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 

#if 1
/* 注意这个函数不回写SECTOR 中的数据，会丢数据的。在FATFS中使用因为它有BUF,所以不会出现非整数SECTOR的写入*/
void W25QX_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	//u16 i;

 	if(pBuffer == NULL|| NumByteToWrite == 0)
 	    return;
   	
 	secpos = WriteAddr / W25QX_SECTOR_SIZE;//扇区地址  
	secoff = WriteAddr % W25QX_SECTOR_SIZE;//在扇区内的偏移
	secremain = W25QX_SECTOR_SIZE - secoff;//扇区剩余空间大小
	
 	if(NumByteToWrite <= secremain)
 	    secremain = NumByteToWrite;//不大于4096个字节

	while(1) 
	{	
        W25Qx_EraseSector(secpos);//擦除这个扇区 
        W25QXX_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   

		if(NumByteToWrite == secremain)
		    break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff = 0;//偏移位置为0 	 

		   	pBuffer += secremain;  //指针偏移
			WriteAddr += secremain;//写地址偏移	   
		   	NumByteToWrite -= secremain;				//字节数递减
		   	
			if(NumByteToWrite > W25QX_SECTOR_SIZE)
			    secremain = W25QX_SECTOR_SIZE;	//下一个扇区还是写不完
			else 
			    secremain = NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 
}
#else 
u8 W25QXX_BUFFER[W25QX_SECTOR_SIZE];		 
void W25QX_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;
 	u8 *pSectorBuff;

 	if(pBuffer == NULL|| NumByteToWrite == 0)
 	    return;
   	
   	pSectorBuff = W25QXX_BUFFER;
 	secpos = WriteAddr / W25QX_SECTOR_SIZE;//扇区地址  
	secoff = WriteAddr % W25QX_SECTOR_SIZE;//在扇区内的偏移
	secremain = W25QX_SECTOR_SIZE - secoff;//扇区剩余空间大小
	
 	if(NumByteToWrite <= secremain)
 	    secremain = NumByteToWrite;//不大于4096个字节

	while(1) 
	{	
        //W25Qx_EraseSector(secpos);//擦除这个扇区 
        //W25QXX_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		W25Qx_Read(secpos*W25QX_SECTOR_SIZE, pSectorBuff, W25QX_SECTOR_SIZE);//读出整个扇区的内容
		
		for(i=0;i<secremain;i++)//校验数据
		{
			if(pSectorBuff[secoff+i] != 0XFF)
			    break;//需要擦除  	  
		}
		
		if(i<secremain)//需要擦除
		{
			W25Qx_EraseSector(secpos);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				pSectorBuff[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(secpos*W25QX_SECTOR_SIZE, pSectorBuff, W25QX_SECTOR_SIZE);//写入整个扇区  

		}
		else 
		    W25QXX_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   

		if(NumByteToWrite == secremain)
		    break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff = 0;//偏移位置为0 	 

		   	pBuffer += secremain;  //指针偏移
			WriteAddr += secremain;//写地址偏移	   
		   	NumByteToWrite -= secremain;				//字节数递减
		   	
			if(NumByteToWrite > W25QX_SECTOR_SIZE)
			    secremain = W25QX_SECTOR_SIZE;	//下一个扇区还是写不完
			else 
			    secremain = NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 
}
#endif
/****************************************************************************
* 名    称：void FlashReadID(void)
* 功    能：读工厂码及型号的函数
* 入口参数：  
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void FlashReadID(unsigned char *pFacID, unsigned char *pDevID)
{
	Select_Flash();	
  	SPI_Flash_SendByte(eReadID);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
  	*pFacID = SPI_Flash_ReadByte();		          //BFH: 工程码SST
	*pDevID = SPI_Flash_ReadByte();	              //41H: 器件型号SST25VF016B     
  	NotSelect_Flash();	
}

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
   
  /* 使能SPI1 时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	
	
  /* 配置 SPI1 引脚: SCK, MISO 和 MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* 配置PC4为SST25VF016B的片选  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);   
 
  /* SPI1配置 */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* 使能SPI1  */
  SPI_Cmd(SPI1, ENABLE);   
  NotSelect_Flash();
 
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
u8 SPI_Flash_ReadByte(void)
{
  return (SPI_Flash_SendByte(Dummy_Byte));
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_Flash_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  //NotSelect_Flash();  while(1);
  /* Send byte through the SPI2 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}/******************* (C) COPYRIGHT 2011 奋斗STM32 *****END OF FILE****/



