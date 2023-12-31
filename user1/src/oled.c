#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"
#include "w25q64.h"
#include "stdio.h"

void OLED_SPI_Init(void)
{
    /***********SPI初始化***********/
    SPI_InitTypeDef SPI_InitStruct;
    GPIO_InitTypeDef  GPIO_InitStruct; //定义GPIO类型结构体变量
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //开启串口时钟 PA时钟
    //CLK
    GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin  =GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    //MOSI
    GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin  =GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStruct);


    SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//双线全双工
    SPI_InitStruct.SPI_Mode=SPI_Mode_Master;//主机模式
    SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;//八位
    SPI_InitStruct.SPI_CPOL=SPI_CPOL_High;//时钟极性
    SPI_InitStruct.SPI_CPHA=SPI_CPHA_2Edge;//时钟相位
    SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;//高位在前
    SPI_InitStruct.SPI_NSS=SPI_NSS_Soft;   //自己拉高拉低CS
    SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;//4分频
    SPI_InitStruct.SPI_CRCPolynomial=7;//校验
    SPI_Init(SPI2,&SPI_InitStruct);
    SPI_Cmd(SPI2, ENABLE);
}

void OLED_Init (void)
{
    OLED_SPI_Init();

    RCC->APB2ENR |= (1<<0);//AFIO时钟使能
    RCC->APB2ENR |= (1<<2);//PA时钟使能
    RCC->APB2ENR |= (1<<3);//PB时钟使能
    AFIO->MAPR &=~ (7<<24);
    AFIO->MAPR |=  (2<<24);   //关闭JTAG  使能SW

    //片选CS  PB7普通输出
    GPIOB->CRL &= ~(0xF0000000);
    GPIOB->CRL |=  (0x3<<28);
    //数据控制选择DC  PA15普通输出
    GPIOA->CRH &= ~(0x80000000);
    GPIOA->CRH |=  (0x3<<28);
    //复位RES  PB6普通输出
    GPIOB->CRL &= ~(0xf<<24);
    GPIOB->CRL |=  (0x3<<24);

    //给片选一个默认状态，不选中
    OLED_CS(1);

    OLED_RES(1);//硬复位OLED
    delay_ms(200);
    OLED_RES(0);
    delay_ms(200);
    OLED_RES(1);
    delay_ms(200);

    OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_WR_Byte(0x02,OLED_CMD);//---SET low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---SET high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--SET start line address  SET Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0x81,OLED_CMD);//--SET contrast control register
    OLED_WR_Byte(0xCF,OLED_CMD);// SET SEG Output Current Brightness
    OLED_WR_Byte(0xA1,OLED_CMD);//--SET SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WR_Byte(0xC8,OLED_CMD);//SET COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WR_Byte(0xA6,OLED_CMD);//--SET normal display
    OLED_WR_Byte(0xA8,OLED_CMD);//--SET multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_WR_Byte(0xD3,OLED_CMD);//-SET display offSET	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00,OLED_CMD);//-not offSET
    OLED_WR_Byte(0xd5,OLED_CMD);//--SET display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0x80,OLED_CMD);//--SET divide ratio, SET Clock as 100 Frames/Sec
    OLED_WR_Byte(0xD9,OLED_CMD);//--SET pre-charge period
    OLED_WR_Byte(0xF1,OLED_CMD);//SET Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDA,OLED_CMD);//--SET com pins hardware configuration
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);//--SET vcomh
    OLED_WR_Byte(0x40,OLED_CMD);//SET VCOM Deselect Level
    OLED_WR_Byte(0x20,OLED_CMD);//-SET Page Addressing Mode (0x00/0x01/0x02)
    OLED_WR_Byte(0x02,OLED_CMD);//
    OLED_WR_Byte(0x8D,OLED_CMD);//--SET Charge Pump enable/disable
    OLED_WR_Byte(0x14,OLED_CMD);//--SET(0x10) disable
    OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
    OLED_Clear();
}

//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
u8 OLED_WR_Byte(uint8_t dat,OLED_MODE cmd)
{

    u8 data;
    OLED_DC(cmd); //写命令
    OLED_CS(0);		//片选
    while((SPI2->SR&(1<<1))==0);		//等待发送区空
    SPI2->DR=dat;	 	  		//发送一个byte
    while((SPI2->SR&(1<<0))==0);
    data = SPI2->DR;
    OLED_CS(1);		  //取消片选
    return data;

}

//定位OLED屏幕的坐标
//x -- 列坐标
//y -- 页坐标
void OLED_SetPos (uint16_t x, uint16_t y)
{
    //设置页地址和列地址
    OLED_WR_Byte(0xB0+y,OLED_CMD);								//设置页地址
    OLED_WR_Byte(0x00 + (x & 0x0f),OLED_CMD);       //列地址低4位
    OLED_WR_Byte(0x10 + ((x & 0xf0)>>4),OLED_CMD);  //列地址高4位
}

void Draw_Point(void)//测试
{
    OLED_SetPos (0, 2);
    OLED_WR_Byte(1,OLED_DATA);//写1显示  0关闭

//  for(int i=0;i<10;i++)   //画10个点
//  OLED_WR_Byte(1,OLED_DATA);//写数据
}


void OLED_Clear(void)//清屏函数
{
    uint8_t i = 0;
    uint8_t j = 0;
    for(i=0; i<OLED_PAGE; i++) //页
    {
        OLED_SetPos (0, i);     //起始坐标
        for(j=0; j<OLED_WIGH; j++) //宽
        {

            OLED_WR_Byte(0x00,OLED_DATA);//写数据
        }
    }
}

//画图片函数
void OLED_ShowPhoto(uint16_t x, uint16_t y, uint16_t width, uint16_t high, const unsigned char * photo)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint16_t count = 0;
    for(i=0; i<high; i++)
    {
        OLED_SetPos (x, y+i);
        for(j=0; j<width; j++)
        {

            OLED_WR_Byte(photo[count++],OLED_DATA);//写数据
        }
    }
}
void OLED_ShowChar(uint16_t x, uint16_t y, char c)//写字符
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint16_t count = 0;
    for(i=0; i<2; i++)
    {
        OLED_SetPos (x, y+i);
        for(j=0; j<8; j++)
        {
            OLED_WR_Byte(ascii_8_16[c - 32][count++],OLED_DATA);//写数据
        }
    }
}

void OLED_ShowStr(uint16_t x, uint16_t y, char * str)//写字符串
{
    uint16_t i = 0;
    while (str[i] != '\0')
    {
        OLED_ShowChar(x+=8, y, str[i]);
        if (x > OLED_WIGH - 8)
        {
            x = 0;
            y += 2;
        }
        if (y > OLED_PAGE - 2)
        {
            y = 0;
        }

        i++;
    }
}

void OLED_Showchinaese(u8 x,u8 y,u8 no)
{
    u8 t,adder=0;

    OLED_SetPos(x, y);
    for(t=0; t<16; t++)
    {
        OLED_WR_Byte(ziku[2*no][t],OLED_DATA);
        adder+=1;
    }
    OLED_SetPos(x,y+1);
    for(t=0; t<16; t++)
    {
        OLED_WR_Byte(ziku[2*no+1][t],OLED_DATA);
        adder+=1;
    }
}
void OLED_ShowCH (uint16_t x, uint16_t y,u8 *ch)
{
    //从字库中找出字模
//  int index = 0;
    //u8 buf[32]= {0x10,0x10,0x10,0x12,0x12,0x92,0x12,0xFA,0x1A,0x16,0x12,0x10,0x10,0x10,0x10,0x00,0x40,0x40,0x7E,0x42,0x42,0x7E,0x43,0x42,0x42,0x7E,0x42,0x42,0x7E,0x40,0x40,0x00};/*"孟",0---------- 字模放这里*/
    u8 buf[32];
    uint8_t i = 0;
    uint8_t j = 0;
    uint16_t count = 0;
    W25Q64_Read(buf,((*ch-0xa1)*94+(*(ch+1)-0xa1))*32,32);
    for(i=0; i<2; i++)
    {
        OLED_SetPos (x, y+i);
        for(j=0; j<16; j++)
        {
            //写数据
            OLED_WR_Byte(buf[count++],OLED_DATA);
        }
    }
}

void OLED_ShowChineseHz(uint16_t x, uint16_t y,u8 *ch)
{
    while(*ch!='\0')
    {
        if(*ch<='~')
        {
            OLED_SetPos(x, y);
            OLED_ShowChar(x,y,*ch++);
            x+=8;
//            if(x>120) {
//                x=0;
//                y+=2;
//            }
        } else {
            OLED_SetPos (x, y);
            OLED_ShowCH (x,y,ch);
            x+=16;
            ch++;
            ch++;
//					  if(x>120) {
//                x=0;
//                y+=2;
//            }
        }
    }
}