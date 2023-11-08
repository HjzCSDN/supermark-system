#include "AT24C04.h"
#include "delay.h"
#include <stdio.h>
void IIC_StartA(void);				//发送IIC开始信号
void IIC_StopA(void);	  			//发送IIC停止信号
void IIC_Send_ByteA(u8 txd);			//IIC发送一个字节
u8 IIC_Read_ByteA(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_AckA(void); 				//IIC等待ACK信号
void IIC_AckA(void);					//IIC发送ACK信号
void IIC_NAckA(void);				//IIC不发送ACK信号




u8 AT24C04_Write_Byte(u8 pageSet, u8 addr, u8 data)
{
    IIC_StartA();
    IIC_Send_ByteA(AT24C04_ADDR | AT24C04_WRITE_BIT | pageSet);
    if(!IIC_Wait_AckA())
    {
        return 0;
    }
    IIC_Send_ByteA(addr);
    if(!IIC_Wait_AckA())
    {
        return 0;
    }
    IIC_Send_ByteA(data);
    if(!IIC_Wait_AckA())
    {
        return 0;
    }
    IIC_StopA();
    return 1;
}



u8 AT24C04_Write_Page(u8 pageSet, u8 addr, u8 * pData, u8 lenth)
{
    u8 i;
    IIC_StartA();

    IIC_Send_ByteA(AT24C04_ADDR | AT24C04_WRITE_BIT | pageSet);
    if(!IIC_Wait_AckA())
    {
        return 0;
    }

    IIC_Send_ByteA(addr);

    if(!IIC_Wait_AckA())
    {
        return 0;
    }

    for(i = 0; i < lenth; i++)
    {
        IIC_Send_ByteA(*(pData + i));

        if(!IIC_Wait_AckA())
        {
            return 0;
        }
    }

    IIC_StopA();
    return 1;
}

u8 AT24C04_Read_Current_Addr_Byte(u8 pageSet)
{
    u8 temp;
    IIC_StartA();
    IIC_Send_ByteA(AT24C04_ADDR | AT24C04_READ_BIT | pageSet);
    if(!IIC_Wait_AckA())
    {
        return 0;
    }
    temp = IIC_Read_ByteA(0);
    IIC_StopA();
    return temp;
}

u8 AT24C04_Read_Byte(u8 pageSet, u8 addr)
{
    u8 temp;
    IIC_StartA();
    IIC_Send_ByteA(AT24C04_ADDR | AT24C04_WRITE_BIT | pageSet);
    if(!IIC_Wait_AckA())
    {
        return 0;
    }
    IIC_Send_ByteA(addr);
    if(!IIC_Wait_AckA())
    {
        return 0;
    }
    temp = AT24C04_Read_Current_Addr_Byte(pageSet);
    return temp;
}



u8 * AT24C04_Read_Page(u8 pageSet, u8 addr, u8 lenth)
{
    u8 i;
    static u8 receiveBuf[256];

    IIC_StartA();

    IIC_Send_ByteA(AT24C04_ADDR | AT24C04_WRITE_BIT | pageSet);
    if(!IIC_Wait_AckA())
    {

        return 0;
    }

    IIC_Send_ByteA(addr);
    if(!IIC_Wait_AckA())
    {
        return 0;
    }
    IIC_StartA();
    IIC_Send_ByteA(AT24C04_ADDR | AT24C04_READ_BIT | pageSet);
    if(!IIC_Wait_AckA())
    {
        return 0;
    }
    for(i = 0; i < lenth; i++)
    {
        receiveBuf[i] = IIC_Read_ByteA(1);
    }



    return receiveBuf;
}






void AT24C02_I2C_Configuration()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );

    GPIO_InitStructure.GPIO_Pin = IIC_SDA_Pin | IIC_SCL_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IIC_SDA_Port, &GPIO_InitStructure);

    IIC_SCL_H;
    IIC_SDA_H;

}



//产生IIC起始信号
void IIC_StartA(void)
{
    SDA_OUT();     //sda线输出
    IIC_SDA_H;
    IIC_SCL_H;
    delay_us(4);
    IIC_SDA_L;//START:when CLK is high,DATA change form high to low
    delay_us(4);
    IIC_SCL_L;//钳住I2C总线，准备发送或接收数据


}
//产生IIC停止信号
void IIC_StopA(void)
{
    SDA_OUT();//sda线输出
    IIC_SCL_L;
    IIC_SDA_L;//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC_SCL_H;
    IIC_SDA_H;//发送I2C总线结束信号
    delay_us(4);
}
//等待应答信号到来
//返回值：0，接收应答失败
//        1，接收应答成功
u8 IIC_Wait_AckA(void)
{
    u8 ucErrTime=0;
    SDA_IN();      //SDA设置为输入
    IIC_SDA_H;
    delay_us(1);
    IIC_SCL_H;
    delay_us(1);
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_StopA();
            return 0;
        }
    }
    IIC_SCL_L;//时钟输出0
    return 1;
}
//产生ACK应答
void IIC_AckA(void)
{
    IIC_SCL_L;
    SDA_OUT();
    IIC_SDA_L;
    delay_us(2);
    IIC_SCL_H;
    delay_us(2);
    IIC_SCL_L;
    IIC_SDA_H;
}
//不产生ACK应答
void IIC_NAckA(void)
{
    IIC_SCL_L;
    SDA_OUT();
    IIC_SDA_H;
    delay_us(2);
    IIC_SCL_H;
    delay_us(2);
    IIC_SCL_L;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_ByteA(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL_L;//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        if((txd&0x80)>>7)
        {
            IIC_SDA_H;
        }
        else {
            IIC_SDA_L;
        }

        txd<<=1;
        delay_us(2);   //对TEA5767这三个延时都是必须的
        IIC_SCL_H;
        delay_us(2);
        IIC_SCL_L;
        delay_us(2);
    }
}


//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_Read_ByteA(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        IIC_SCL_L;
        delay_us(2);
        IIC_SCL_H;
        receive<<=1;
        if(READ_SDA)receive++;
        delay_us(1);
    }
    if (!ack)
        IIC_NAckA();//发送nACK
    else
        IIC_AckA(); //发送ACK
    return receive;
}


void AT24C02_I2C_WriteByte(uint8_t addr,uint8_t data)
{
    IIC_StartA();
    IIC_Send_ByteA(addr);
    if(!IIC_Wait_AckA())
    {
        return;
    }
    IIC_Send_ByteA(data);
    if(!IIC_Wait_AckA())
    {
        return;
    }
    IIC_StopA();

}












