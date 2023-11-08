#include "stm32f10x.h"                  // Device header
#include "w25q64.h"
#include "delay.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//  文 件 名   :W25Q64
//  版 本 号   : 
//  作    者   : Mr'K
//  生成日期   : 2022年 1月 16日
//  最近修改   : 
//  功能描述   :
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              CS 脚   接 PB0
//              DI 脚   接  PA7
//              CLK 脚  接 PA5
//              D0 脚   接 PA6	           
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 2022年 1月 16日
// 作    者   : Mr'K
// 修改内容   : 创建文件
//版权所有，盗版必究。
//此板块黄锦钊制作于2022年1月9日
//All rights reserved
//******************************************************************************/
//配置W25Q64 端口

void W25Q64_Init(void)//初始化W25Q64的IO口
{

//   SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO类型结构体变量
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2|RCC_APB1Periph_SPI3,ENABLE);
	
    RCC->APB2ENR |= 3 << 2; //使能 A B
    RCC->APB2ENR |= 1 << 12; //SPI1使能
	
		//RCC->APB1ENR |= 1<<15;  //使能SPI3

//    GPIOA->CRL &= ~(0XFFF << 5 * 4); //给  5 6 7脚清零
//    GPIOA->CRL |= 9 << (5 * 4); // 5脚 推挽复用输出，50M
//    GPIOA->CRL |= 4 << (6 * 4); // 6脚 浮空输入输出，50M
//    GPIOA->CRL |= 9 << (7 * 4); // 7脚 推挽复用输出，50M
//    GPIOB->CRL &= ~(0X0F << 0);
//    GPIOB->CRL |= 3 << (0 * 4); // 0脚 推挽复用输出，50M

    GPIOB->CRH &= ~(0X0F << (12-8) * 4);
    GPIOB->CRH |= 3 << ((12-8) * 4); // 12脚 推挽复用输出，50M
	
	
	
	
	
    GPIOB->CRH &= ~(0XFFF << (13-8) * 4); //给  13 14 15脚清零
    GPIOB->CRH |= 9 << ((13-8) * 4); // 13脚 推挽复用输出，50M
    GPIOB->CRH |= 4 << ((14-8) * 4); // 14脚 浮空输入输出，50M
    GPIOB->CRH |= 9 << ((15-8)* 4); //  15脚 推挽复用输出，50M
//    GPIOB->CRL &= ~(0X0F << 0);
//    GPIOB->CRL |= 3 << (0 * 4); // 0脚 推挽复用输出，50M	
//	
// 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);




	
//    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
//    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工
//    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
//   SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
//    SPI_InitStruct.SPI_CRCPolynomial = 7;
//    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
//  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
//    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
//    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
//    SPI_Init(SPI1, &SPI_InitStruct);
//    SPI_Cmd(SPI1, ENABLE);
    SPI2->CR1 |= 0 << 10; //全双工模式
    SPI2->CR1 |= 1 << 9; //软件nss管理
    SPI2->CR1 |= 1 << 8;
    SPI2->CR1 |= 1 << 2; //SPI主机
    SPI2->CR1 |= 0 << 11; //8bit数据格式
    SPI2->CR1 |= 1 << 1; //空闲模式下SCK为1 CPOL=1 空闲信号下的时钟极性
    SPI2->CR1 |= 1 << 0; //数据采样从第二个时间边沿开始,CPHA=1 时间相位(数据在第几个边沿（第几个周期结束的地方）被锁存)
    SPI2->CR1 |= 1 << 3; //Fsck=Fcpu/4
    SPI2->CR1 |= 0 << 7; //MSBfirst
    SPI2->CR1 |= 1 << 6; //SPI设备使能
    W25Q64_CS = 1; //我们 默认低电平 如果不拉高  就片选中器件
}
/**********************************************************************
*函数名：SPI3_WriteReadByte
*功  能：SPI读写一个字节
*参  数：写入字节
*返  回：读出字节
*备  注：u8 SPI3_WriteReadByte(u8 data)
**********************************************************************/
u8 SPI3_WriteReadByte(u8 data)
{
    while ((SPI2->SR & (1 << 1)) == 0);
    SPI2->DR = data;          //发送一个byte
    while ((SPI2->SR & (1 << 0)) == 0);
    return SPI2->DR;          //返回收到的数据
}




u16 W25Q64_ReadID(void)//读取芯片ID
{
    u16 Temp = 0;
    W25Q64_CS = 0;
    SPI3_WriteReadByte(W25Q64_Read_ID);//发送读取ID命令
    SPI3_WriteReadByte(0x00);
    SPI3_WriteReadByte(0x00);
    SPI3_WriteReadByte(0x00);
    Temp |= SPI3_WriteReadByte(0xFF) << 8;
    Temp |= SPI3_WriteReadByte(0xFF);
    W25Q64_CS = 1;
    return Temp;
}
//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大32768)
void W25Q64_Read(u8 *buffer, u32 read_address, u16 data)
{
    u16 i;
    W25Q64_CS = 0;                          //使能器件
    SPI3_WriteReadByte(W25Q64_ReadData);         //发送读取命令
    SPI3_WriteReadByte((u8)((read_address) >> 16)); //发送24bit地址
    SPI3_WriteReadByte((u8)((read_address) >> 8));
    SPI3_WriteReadByte((u8)read_address);
    for (i = 0; i < data; i++)
    {
        buffer[i] = SPI3_WriteReadByte(0XFF); //循环读数 ，发送空字节
    }
    W25Q64_CS = 1;
}
//SPI在一页(0~32768)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据 （不能跨扇区写）
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void W25Q64_Write_Page(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u16 i;
    W25Q64_Write_Enable();                  //SET WEL   发送写使能，WEL位自动置1
    W25Q64_CS = 0;                          //使能器件
    SPI3_WriteReadByte(W25Q64_PageProgram);      //发送写页命令
    SPI3_WriteReadByte((u8)((WriteAddr) >> 16)); //发送24bit地址
    SPI3_WriteReadByte((u8)((WriteAddr) >> 8));
    SPI3_WriteReadByte((u8)WriteAddr);
    for (i = 0; i < NumByteToWrite; i++)
        SPI3_WriteReadByte(pBuffer[i]);                     //循环写数
    W25Q64_CS = 1;                           //取消片选
    W25Q64_Wait_Busy();                                   //等待写入结束
}
//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能  （可以跨扇区写）
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大32768)
//CHECK OK
void W25Q64_Write_NoCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u16 pageremain;
    pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
    if (NumByteToWrite <= pageremain) pageremain = NumByteToWrite; //不大于256个字节
    while (1)
    {
        W25Q64_Write_Page(pBuffer, WriteAddr, pageremain); //先写页剩余的字节数
        if (NumByteToWrite == pageremain)break; //写入结束了
        else //NumByteToWrite>pageremain
        {
            pBuffer += pageremain; // buf指针加上  上一次写入的字节数
            WriteAddr += pageremain; // flash地址加上 上一次写入的字节数

            NumByteToWrite -= pageremain;         //减去已经写入了的字节数
            if (NumByteToWrite > 256)pageremain = 256; //一次可以写入256个字节
            else pageremain = NumByteToWrite;     //不够256个字节了
        }
    }
}


//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//buffer:数据存储区
//write_address:开始写入的地址(24bit)
//data:要写入的字节数(最大32768)
u8 w25q64_buffer[4096]; //定义一个4K的缓冲区。
void W25Q64_Write(u8 *buffer, u32 write_address, u16 data)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;
    u8 *w25q64_buff;
    w25q64_buff = w25q64_buffer;
    secpos = write_address / 4096; //扇区地址 ,第几个扇区
    secoff = write_address % 4096; //在扇区内的偏移
    secremain = 4096 - secoff; //扇区剩余空间大小
    if (data <= secremain) //不大于4096个字节  写不完一个扇区内的空间大小,即不跨扇区
        secremain = data; //假设一个扇区原来已经写了3K的数据，后来再写入0.5K,结果还有0.5K空间没写,
    //这样就不用全部擦除剩余的1K空间，,只要擦除要写入的0.5K的空间就可以了
    while (1)
    {
        W25Q64_Read(w25q64_buff, secpos * 4096, 4096); //读出整个扇区的内容,保存到W25Q64_BUF
        for (i = 0; i < secremain; i++) //  校验数据 对W25Q64_BUF进行校验
        {
            if (w25q64_buff[secoff + i] != 0XFF)break; //需要擦除，从secoff的位置开始 读出来的内容如果不是0xff，需要擦除，手册上有说明P38
        }
        if (i < secremain) //需要擦除   假设写入10个数，而写到第8个的时候，上面的校验出错，那么就要擦除了
        {
            W25Q64_Erase_Sector(secpos); //擦除这个扇区,这时候这个扇区的内容已经被存放在W25Q64_BUF，故可以擦除
            for (i = 0; i < secremain; i++)     //复制
            {
                w25q64_buff[i + secoff] = buffer[i]; //之前存放的数据在上面也已经存放到W25Q64_BUF中了，所以从  secoff 处开始存放数据(要写入的数据),
                //这样就把之前存放的数据和后来写入的数据都存入W25Q64_BUF中了。
            }
            //把存放到W25Q64_BUF 中的数据写到扇区里
            W25Q64_Write_NoCheck(w25q64_buff, secpos * 4096, 4096); //写入整个扇区  secpos*4096就是地址
        }
        else
        {
            W25Q64_Write_NoCheck(buffer, write_address, secremain); //写已经擦除了的,直接写入扇区剩余区间.
        }

        if (data == secremain)
            break;//写入结束了

        else//写入未结束        写满了一个扇区，开始往第二个扇区写数据

        {
            secpos++;//扇区地址增1          已经是下一个扇区了，所以++
            secoff = 0; //偏移位置为0   已经是一个新的扇区了，所以为0
            buffer += secremain; //源数据指针偏移    已经写了secremain个数据，所以需要偏移
            write_address += secremain; //写地址偏移
            data -= secremain;              //字节数递减    已经写了secremain个数据 故减去secremain个数据
            if (data > 4096)    //下一个扇区还是写不完
                secremain = 4096;               //下一下扇区的内容就是4096
            else
                secremain = data;       //下一个扇区可以写完了
        }
    }
}




//擦除整个芯片
//等待时间超长...
void W25Q64_Erase_Chip(void)
{
    W25Q64_Write_Enable();                  //SET WEL
    W25Q64_Wait_Busy();
    W25Q64_CS = 0;                          //使能器件
    SPI3_WriteReadByte(W25Q64_ChipErase);        //发送片擦除命令
    W25Q64_CS = 1;                          //取消片选
    W25Q64_Wait_Busy();                    //等待芯片擦除结束
}


//擦除一个扇区
//Dst_Addr: 扇区编号，是第几个扇区，不是绝对地址

void W25Q64_Erase_Sector(u32 Dst_Addr)  //1 //擦除一个扇区的最少时间:150ms
{
    Dst_Addr *= 4096;
    W25Q64_Write_Enable();                  //SET WEL    写使能
    W25Q64_Wait_Busy();                                      //等待空闲
    W25Q64_CS = 0;                          //使能器件
    SPI3_WriteReadByte(W25Q64_SectorErase);      //发送扇区擦除指令
    SPI3_WriteReadByte((u8)((Dst_Addr) >> 16)); //发送24bit地址   ,先写高位
    SPI3_WriteReadByte((u8)((Dst_Addr) >> 8));
    SPI3_WriteReadByte((u8)Dst_Addr);
    W25Q64_CS = 1;                          //取消片选
    W25Q64_Wait_Busy();                    //等待擦除完成
}



//等待空闲
void W25Q64_Wait_Busy(void)
{
    while ((W25Q64_ReadSR() & 0x01) == 0x01); // 等待BUSY位清空
}

//读取W25Q64的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
u8 W25Q64_ReadSR(void)
{
    u8 byte = 0;
    W25Q64_CS = 0;                          //使能器件
    SPI3_WriteReadByte(W25Q64_ReadStatusReg);    //发送读取状态寄存器命令
    byte = SPI3_WriteReadByte(0Xff);           //读取一个字节
    W25Q64_CS = 1;                          //取消片选
    return byte;
}

//W25Q64写使能
//将WEL置位
void W25Q64_Write_Enable(void)  //W25Q64写使能
{
    W25Q64_CS = 0;                          //使能器件
    SPI3_WriteReadByte(W25Q64_WriteEnable);      //发送写使能
    W25Q64_CS = 1;                          //取消片选
}



