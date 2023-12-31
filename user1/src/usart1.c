#include "usart1.h"
/*****************************************************************************************
函数名  :
功能    :串口1初始化
出口参数:
入口参数:bound 波特率
返回值  :
说明    :IO PA9 PA10
******************************************************************************************/
void Usart1_Init(u32 bound)
{
#if 0
    u16    DIV_Mantissa;//整数部分
    float  DIV_Fraction;//小数部分

    RCC->APB2ENR |=1<<2;//PA时钟
    RCC->APB2ENR |=1<<14;//串口一时钟
    //PA9
    GPIOA->CRH   &=0XFFFFFF0F;//清0
    GPIOA->CRH   |=0X000000B0;//复用推挽输出
    //PA10
    GPIOA->CRH   &=0XFFFFF0FF;//清0
    GPIOA->CRH   |=0X00000400;//复用推挽输出


    DIV_Fraction=(float)72000000/16/bound;//计算波特率
    DIV_Mantissa=(u16)DIV_Fraction;//整数部分
    DIV_Fraction=DIV_Fraction-DIV_Mantissa;//得出小数部分
    USART1->BRR = (DIV_Mantissa<<4 )+(DIV_Fraction*16);

    USART1->CR1 |= 1<<2|1<<3|1<<13;//开启接收使能 发送使能 串口使能
#else
    USART_InitTypeDef USART_InitStruct;//定义串口类型结构体变量
    GPIO_InitTypeDef  GPIO_InitStruct; //定义GPIO类型结构体变量
    NVIC_InitTypeDef  NVIC_InitStruct; //定义中断向量控制器的变量
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);//开启串口时钟 PA时钟
    /*****TX********/
    GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin  =GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*****RX********/
    GPIO_InitStruct.GPIO_Mode =GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin  =GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitStruct.USART_BaudRate  =bound;//设置波特率
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode      = USART_Mode_Tx|USART_Mode_Rx;//全双工模式
    USART_InitStruct.USART_Parity    =USART_Parity_No;           //无校验
    USART_InitStruct.USART_StopBits  =USART_StopBits_1;         //一个停止位
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;    //8位数据
    USART_Init(USART1, &USART_InitStruct);

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);              //使能串口接收中断
	USART_ITConfig(USART1,USART_IT_IDLE ,ENABLE);              //使能串口空闲中断
    USART_Cmd(USART1, ENABLE);                                //使能串口

	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;//通道选择
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;  //使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
#endif

}
/*****************************************************************************************
函数名  :
功能    :发送一字节函数
出口参数:
入口参数:发送的数据
返回值  :
******************************************************************************************/

void Usart1_Send_byte(u8 data)
{
    // while(!(USART1->SR & 1<<6));//等待发送完成
    //USART1->DR =data;           //发送数据一个字节
    while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
    USART_SendData(USART1, data);
}
/*****************************************************************************************
函数名  :
功能    :读取一个字节
出口参数:
入口参数:
返回值  :返回读到的字节
说明    :堵塞式接收
******************************************************************************************/
u8 Usart1_Read_byte(void)
{
    //while(!(USART1->SR & 1<<5));//等待接收完成
    // return USART1->DR;
    while(!USART_GetFlagStatus(USART1,USART_FLAG_RXNE));
    return  USART_ReceiveData(USART1);
}

void Usart1_Send_string(u8 *ch)
{
    while(*ch!='\0')
    {
        Usart1_Send_byte(*ch++);
    }
}

u8 Usart1_Readbuff[100];//保存来自电脑的数据
void Usart1_Read_string(void)
{
    u8 ch;
    u8 Usart1_Readlen=0;//接收长度
    while(1)
    {
        ch=Usart1_Read_byte();                //读取一个字节
        if(ch!='\r'&&ch!='\n')                //判断是否是结束符
        {
            Usart1_Readbuff[Usart1_Readlen++]=ch;//循环保存字符 组成字符串
        }
        if(ch=='\n')                          //最后一个字符 结束接收
        {
            Usart1_Readbuff[Usart1_Readlen]='\0';//添加结束符
            break;
        }
    }
}
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

#endif


/*************************实现printf*************************/
int fputc( int ch, FILE *stream )
{
    Usart1_Send_byte(ch);//把数据映射到串口通道
    return ch;
}
Usart1 Read;

void USART1_IRQHandler(void)
{
	if( USART_GetITStatus(USART1,USART_IT_RXNE)==SET)//接收中断
	{
	     Read.USART1_BUFF[Read.USART1_FLAG++]=USART_ReceiveData(USART1);
		 if(Read.USART1_FLAG>255) Read.USART1_FLAG=0;//判断超出数组长度 重新接收
	}
	else if(USART_GetITStatus(USART1,USART_IT_IDLE)==SET)//没有数据接收产生空闲中断
	{
	   Read.USART1_BUFF[Read.USART1_FLAG]=0;//添加结束符
	   Read.USART1_FLAG|=0x80;            	//接收完成标志 最高位置1
	   USART_ReceiveData(USART1);           //清除标志
	}

}












