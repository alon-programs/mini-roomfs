#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

#include "stm32f10x_type.h"

#define W25QX128JV (1)

#define Dummy_Byte 0xA5

#define W25QX_PAGE_SIZE             (256)
#define W25QX_PAGES_PER_SECTOR      (16)
#define W25QX_SECTOR_SIZE          (W25QX_PAGE_SIZE*W25QX_PAGES_PER_SECTOR)
#define W25QX_SECTOR_PER_BLOCK      (16)
#define W25QX_BLOCK_SIZE           (W25QX_SECTOR_SIZE*W25QX_SECTOR_PER_BLOCK) 

#ifdef W25QX32JV
    #define W25QX_SECTOR_NUM        (1024)
#elif  W25QX128JV
    #define W25QX_SECTOR_NUM        (4096)
#endif

/* Select SPI FLASH: ChipSelect pin low  */
#define Select_Flash()          GPIO_ResetBits(GPIOB, GPIO_Pin_8)
/* Deselect SPI FLASH: ChipSelect pin high */
#define NotSelect_Flash()       GPIO_SetBits(GPIOB, GPIO_Pin_8)

#define SPI_Flash_ResetOFF()    GPIO_SetBits(GPIOB, GPIO_Pin_6)



extern void SPI_Flash_Init(void);	         //SPI初始化
extern u8 SPI_Flash_ReadByte(void);		//flash操作基本函数，读一个字节
extern u8 SPI_Flash_SendByte(u8 byte);		//	FLASH操作基本函数，发送一个字节
/****************************************************************************
* 名    称：void FlashReadID(void)
* 功    能：读工厂码及型号的函数
* 入口参数：  
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
extern void FlashReadID(unsigned char *pFacID, unsigned char *pDevID);

/****************************************************************************
* 名    称：void W25Qx_Read(unsigned long addr, unsigned char *readbuff, unsigned int NumOfByte)
* 功    能：页读
* 入口参数：unsigned long addr--页   unsigned char *readbuff--数组   unsigned int NumOfByte--长度
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
extern void W25Qx_EraseSector(unsigned int SectorAddr);

extern void W25Qx_Read(unsigned long addr, unsigned char *readbuff, unsigned int NumOfByte);
extern void W25QX_Write(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);   
extern void W25Qx_EraseSector(unsigned int SectorAddr);

extern void W25QXX_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u32 NumByteToWrite); 
#endif


