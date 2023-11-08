#include "stm32f10x.h"                  // Device header
#include "w25q64.h"
#include "delay.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//  �� �� ��   :W25Q64
//  �� �� ��   : 
//  ��    ��   : Mr'K
//  ��������   : 2022�� 1�� 16��
//  ����޸�   : 
//  ��������   :
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              CS ��   �� PB0
//              DI ��   ��  PA7
//              CLK ��  �� PA5
//              D0 ��   �� PA6	           
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 2022�� 1�� 16��
// ��    ��   : Mr'K
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//�˰��ƽ���������2022��1��9��
//All rights reserved
//******************************************************************************/
//����W25Q64 �˿�

void W25Q64_Init(void)//��ʼ��W25Q64��IO��
{

//   SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef  GPIO_InitStructure; //����GPIO���ͽṹ�����
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2|RCC_APB1Periph_SPI3,ENABLE);
	
    RCC->APB2ENR |= 3 << 2; //ʹ�� A B
    RCC->APB2ENR |= 1 << 12; //SPI1ʹ��
	
		//RCC->APB1ENR |= 1<<15;  //ʹ��SPI3

//    GPIOA->CRL &= ~(0XFFF << 5 * 4); //��  5 6 7������
//    GPIOA->CRL |= 9 << (5 * 4); // 5�� ���츴�������50M
//    GPIOA->CRL |= 4 << (6 * 4); // 6�� �������������50M
//    GPIOA->CRL |= 9 << (7 * 4); // 7�� ���츴�������50M
//    GPIOB->CRL &= ~(0X0F << 0);
//    GPIOB->CRL |= 3 << (0 * 4); // 0�� ���츴�������50M

    GPIOB->CRH &= ~(0X0F << (12-8) * 4);
    GPIOB->CRH |= 3 << ((12-8) * 4); // 12�� ���츴�������50M
	
	
	
	
	
    GPIOB->CRH &= ~(0XFFF << (13-8) * 4); //��  13 14 15������
    GPIOB->CRH |= 9 << ((13-8) * 4); // 13�� ���츴�������50M
    GPIOB->CRH |= 4 << ((14-8) * 4); // 14�� �������������50M
    GPIOB->CRH |= 9 << ((15-8)* 4); //  15�� ���츴�������50M
//    GPIOB->CRL &= ~(0X0F << 0);
//    GPIOB->CRL |= 3 << (0 * 4); // 0�� ���츴�������50M	
//	
// 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);




	
//    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
//    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //ȫ˫��
//    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
//   SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
//    SPI_InitStruct.SPI_CRCPolynomial = 7;
//    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
//  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
//    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
//    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
//    SPI_Init(SPI1, &SPI_InitStruct);
//    SPI_Cmd(SPI1, ENABLE);
    SPI2->CR1 |= 0 << 10; //ȫ˫��ģʽ
    SPI2->CR1 |= 1 << 9; //���nss����
    SPI2->CR1 |= 1 << 8;
    SPI2->CR1 |= 1 << 2; //SPI����
    SPI2->CR1 |= 0 << 11; //8bit���ݸ�ʽ
    SPI2->CR1 |= 1 << 1; //����ģʽ��SCKΪ1 CPOL=1 �����ź��µ�ʱ�Ӽ���
    SPI2->CR1 |= 1 << 0; //���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1 ʱ����λ(�����ڵڼ������أ��ڼ������ڽ����ĵط���������)
    SPI2->CR1 |= 1 << 3; //Fsck=Fcpu/4
    SPI2->CR1 |= 0 << 7; //MSBfirst
    SPI2->CR1 |= 1 << 6; //SPI�豸ʹ��
    W25Q64_CS = 1; //���� Ĭ�ϵ͵�ƽ ���������  ��Ƭѡ������
}
/**********************************************************************
*��������SPI3_WriteReadByte
*��  �ܣ�SPI��дһ���ֽ�
*��  ����д���ֽ�
*��  �أ������ֽ�
*��  ע��u8 SPI3_WriteReadByte(u8 data)
**********************************************************************/
u8 SPI3_WriteReadByte(u8 data)
{
    while ((SPI2->SR & (1 << 1)) == 0);
    SPI2->DR = data;          //����һ��byte
    while ((SPI2->SR & (1 << 0)) == 0);
    return SPI2->DR;          //�����յ�������
}




u16 W25Q64_ReadID(void)//��ȡоƬID
{
    u16 Temp = 0;
    W25Q64_CS = 0;
    SPI3_WriteReadByte(W25Q64_Read_ID);//���Ͷ�ȡID����
    SPI3_WriteReadByte(0x00);
    SPI3_WriteReadByte(0x00);
    SPI3_WriteReadByte(0x00);
    Temp |= SPI3_WriteReadByte(0xFF) << 8;
    Temp |= SPI3_WriteReadByte(0xFF);
    W25Q64_CS = 1;
    return Temp;
}
//��ȡSPI FLASH
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���32768)
void W25Q64_Read(u8 *buffer, u32 read_address, u16 data)
{
    u16 i;
    W25Q64_CS = 0;                          //ʹ������
    SPI3_WriteReadByte(W25Q64_ReadData);         //���Ͷ�ȡ����
    SPI3_WriteReadByte((u8)((read_address) >> 16)); //����24bit��ַ
    SPI3_WriteReadByte((u8)((read_address) >> 8));
    SPI3_WriteReadByte((u8)read_address);
    for (i = 0; i < data; i++)
    {
        buffer[i] = SPI3_WriteReadByte(0XFF); //ѭ������ �����Ϳ��ֽ�
    }
    W25Q64_CS = 1;
}
//SPI��һҳ(0~32768)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ����� �����ܿ�����д��
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!
void W25Q64_Write_Page(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u16 i;
    W25Q64_Write_Enable();                  //SET WEL   ����дʹ�ܣ�WELλ�Զ���1
    W25Q64_CS = 0;                          //ʹ������
    SPI3_WriteReadByte(W25Q64_PageProgram);      //����дҳ����
    SPI3_WriteReadByte((u8)((WriteAddr) >> 16)); //����24bit��ַ
    SPI3_WriteReadByte((u8)((WriteAddr) >> 8));
    SPI3_WriteReadByte((u8)WriteAddr);
    for (i = 0; i < NumByteToWrite; i++)
        SPI3_WriteReadByte(pBuffer[i]);                     //ѭ��д��
    W25Q64_CS = 1;                           //ȡ��Ƭѡ
    W25Q64_Wait_Busy();                                   //�ȴ�д�����
}
//�޼���дSPI FLASH
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ����  �����Կ�����д��
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���32768)
//CHECK OK
void W25Q64_Write_NoCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u16 pageremain;
    pageremain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���
    if (NumByteToWrite <= pageremain) pageremain = NumByteToWrite; //������256���ֽ�
    while (1)
    {
        W25Q64_Write_Page(pBuffer, WriteAddr, pageremain); //��дҳʣ����ֽ���
        if (NumByteToWrite == pageremain)break; //д�������
        else //NumByteToWrite>pageremain
        {
            pBuffer += pageremain; // bufָ�����  ��һ��д����ֽ���
            WriteAddr += pageremain; // flash��ַ���� ��һ��д����ֽ���

            NumByteToWrite -= pageremain;         //��ȥ�Ѿ�д���˵��ֽ���
            if (NumByteToWrite > 256)pageremain = 256; //һ�ο���д��256���ֽ�
            else pageremain = NumByteToWrite;     //����256���ֽ���
        }
    }
}


//дSPI FLASH
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//buffer:���ݴ洢��
//write_address:��ʼд��ĵ�ַ(24bit)
//data:Ҫд����ֽ���(���32768)
u8 w25q64_buffer[4096]; //����һ��4K�Ļ�������
void W25Q64_Write(u8 *buffer, u32 write_address, u16 data)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;
    u8 *w25q64_buff;
    w25q64_buff = w25q64_buffer;
    secpos = write_address / 4096; //������ַ ,�ڼ�������
    secoff = write_address % 4096; //�������ڵ�ƫ��
    secremain = 4096 - secoff; //����ʣ��ռ��С
    if (data <= secremain) //������4096���ֽ�  д����һ�������ڵĿռ��С,����������
        secremain = data; //����һ������ԭ���Ѿ�д��3K�����ݣ�������д��0.5K,�������0.5K�ռ�ûд,
    //�����Ͳ���ȫ������ʣ���1K�ռ䣬,ֻҪ����Ҫд���0.5K�Ŀռ�Ϳ�����
    while (1)
    {
        W25Q64_Read(w25q64_buff, secpos * 4096, 4096); //������������������,���浽W25Q64_BUF
        for (i = 0; i < secremain; i++) //  У������ ��W25Q64_BUF����У��
        {
            if (w25q64_buff[secoff + i] != 0XFF)break; //��Ҫ��������secoff��λ�ÿ�ʼ �������������������0xff����Ҫ�������ֲ�����˵��P38
        }
        if (i < secremain) //��Ҫ����   ����д��10��������д����8����ʱ�������У�������ô��Ҫ������
        {
            W25Q64_Erase_Sector(secpos); //�����������,��ʱ����������������Ѿ��������W25Q64_BUF���ʿ��Բ���
            for (i = 0; i < secremain; i++)     //����
            {
                w25q64_buff[i + secoff] = buffer[i]; //֮ǰ��ŵ�����������Ҳ�Ѿ���ŵ�W25Q64_BUF���ˣ����Դ�  secoff ����ʼ�������(Ҫд�������),
                //�����Ͱ�֮ǰ��ŵ����ݺͺ���д������ݶ�����W25Q64_BUF���ˡ�
            }
            //�Ѵ�ŵ�W25Q64_BUF �е�����д��������
            W25Q64_Write_NoCheck(w25q64_buff, secpos * 4096, 4096); //д����������  secpos*4096���ǵ�ַ
        }
        else
        {
            W25Q64_Write_NoCheck(buffer, write_address, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        }

        if (data == secremain)
            break;//д�������

        else//д��δ����        д����һ����������ʼ���ڶ�������д����

        {
            secpos++;//������ַ��1          �Ѿ�����һ�������ˣ�����++
            secoff = 0; //ƫ��λ��Ϊ0   �Ѿ���һ���µ������ˣ�����Ϊ0
            buffer += secremain; //Դ����ָ��ƫ��    �Ѿ�д��secremain�����ݣ�������Ҫƫ��
            write_address += secremain; //д��ַƫ��
            data -= secremain;              //�ֽ����ݼ�    �Ѿ�д��secremain������ �ʼ�ȥsecremain������
            if (data > 4096)    //��һ����������д����
                secremain = 4096;               //��һ�����������ݾ���4096
            else
                secremain = data;       //��һ����������д����
        }
    }
}




//��������оƬ
//�ȴ�ʱ�䳬��...
void W25Q64_Erase_Chip(void)
{
    W25Q64_Write_Enable();                  //SET WEL
    W25Q64_Wait_Busy();
    W25Q64_CS = 0;                          //ʹ������
    SPI3_WriteReadByte(W25Q64_ChipErase);        //����Ƭ��������
    W25Q64_CS = 1;                          //ȡ��Ƭѡ
    W25Q64_Wait_Busy();                    //�ȴ�оƬ��������
}


//����һ������
//Dst_Addr: ������ţ��ǵڼ������������Ǿ��Ե�ַ

void W25Q64_Erase_Sector(u32 Dst_Addr)  //1 //����һ������������ʱ��:150ms
{
    Dst_Addr *= 4096;
    W25Q64_Write_Enable();                  //SET WEL    дʹ��
    W25Q64_Wait_Busy();                                      //�ȴ�����
    W25Q64_CS = 0;                          //ʹ������
    SPI3_WriteReadByte(W25Q64_SectorErase);      //������������ָ��
    SPI3_WriteReadByte((u8)((Dst_Addr) >> 16)); //����24bit��ַ   ,��д��λ
    SPI3_WriteReadByte((u8)((Dst_Addr) >> 8));
    SPI3_WriteReadByte((u8)Dst_Addr);
    W25Q64_CS = 1;                          //ȡ��Ƭѡ
    W25Q64_Wait_Busy();                    //�ȴ��������
}



//�ȴ�����
void W25Q64_Wait_Busy(void)
{
    while ((W25Q64_ReadSR() & 0x01) == 0x01); // �ȴ�BUSYλ���
}

//��ȡW25Q64��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
u8 W25Q64_ReadSR(void)
{
    u8 byte = 0;
    W25Q64_CS = 0;                          //ʹ������
    SPI3_WriteReadByte(W25Q64_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������
    byte = SPI3_WriteReadByte(0Xff);           //��ȡһ���ֽ�
    W25Q64_CS = 1;                          //ȡ��Ƭѡ
    return byte;
}

//W25Q64дʹ��
//��WEL��λ
void W25Q64_Write_Enable(void)  //W25Q64дʹ��
{
    W25Q64_CS = 0;                          //ʹ������
    SPI3_WriteReadByte(W25Q64_WriteEnable);      //����дʹ��
    W25Q64_CS = 1;                          //ȡ��Ƭѡ
}



