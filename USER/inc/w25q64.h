#ifndef __FLASH_H
#define __FLASH_H			    
#include "stm32f10x.h" 
#include "io_bit.h"


extern u16 W25Q64_TYPE;		//定义我们使用的flash芯片型号		   
#define	W25Q64_CS PBout(12)  //选中FLASH	
				 
////////////////////////////////////////////////////////////////////////////
 
//指令表
#define W25Q64_WriteEnable			0x06 
#define W25Q64_ReadStatusReg		0x05 
#define W25Q64_ReadData				0x03 
#define W25Q64_PageProgram			0x02 
#define W25Q64_SectorErase			0x20 
#define W25Q64_ChipErase			0xC7 
#define W25Q64_Read_ID				0x90 


void W25Q64_Init(void);//W25Q64 初始化
void W25Q64_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写数据存东西

void W25Q64_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//具有自动换页功能  （可以跨扇区写）

void W25Q64_Read(u8* buffer,u32 read_address,u16 data) ;  //读取数据

void W25Q64_Write(u8* buffer,u32 write_address,u16 data);//块存储


void W25Q64_Erase_Chip(void);//擦除整个芯片		  
void W25Q64_Wait_Busy(void);  //读25q64忙
void W25Q64_Write_Enable(void); //W25Q64写使能	
void W25Q64_Erase_Sector(u32 Dst_Addr);//擦除一个扇区
u16  W25Q64_ReadID(void);  	 //读ID
u8	 W25Q64_ReadSR(void);   ////读取W25Q64的状态寄存器
u8 SPI3_WriteReadByte(u8 data);//SPI3读写

#endif





















