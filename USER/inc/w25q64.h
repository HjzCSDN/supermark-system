#ifndef __FLASH_H
#define __FLASH_H			    
#include "stm32f10x.h" 
#include "io_bit.h"


extern u16 W25Q64_TYPE;		//��������ʹ�õ�flashоƬ�ͺ�		   
#define	W25Q64_CS PBout(12)  //ѡ��FLASH	
				 
////////////////////////////////////////////////////////////////////////////
 
//ָ���
#define W25Q64_WriteEnable			0x06 
#define W25Q64_ReadStatusReg		0x05 
#define W25Q64_ReadData				0x03 
#define W25Q64_PageProgram			0x02 
#define W25Q64_SectorErase			0x20 
#define W25Q64_ChipErase			0xC7 
#define W25Q64_Read_ID				0x90 


void W25Q64_Init(void);//W25Q64 ��ʼ��
void W25Q64_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//д���ݴ涫��

void W25Q64_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//�����Զ���ҳ����  �����Կ�����д��

void W25Q64_Read(u8* buffer,u32 read_address,u16 data) ;  //��ȡ����

void W25Q64_Write(u8* buffer,u32 write_address,u16 data);//��洢


void W25Q64_Erase_Chip(void);//��������оƬ		  
void W25Q64_Wait_Busy(void);  //��25q64æ
void W25Q64_Write_Enable(void); //W25Q64дʹ��	
void W25Q64_Erase_Sector(u32 Dst_Addr);//����һ������
u16  W25Q64_ReadID(void);  	 //��ID
u8	 W25Q64_ReadSR(void);   ////��ȡW25Q64��״̬�Ĵ���
u8 SPI3_WriteReadByte(u8 data);//SPI3��д

#endif





















