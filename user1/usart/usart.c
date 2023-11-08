#include "usart.h"
#include <string.h>
#include <stdio.h>

/*******************************************************************************************************
**** 程序名称：   中断知识
**** 设计者:      Mr'K    日期：2022年 1月 15日
**** 修改者:      Mr'K    日期：2022年 1月 15日
**** 功  能：			两个中断抢占优先级和响应优先级不同时， 当低响应优先级执行中断时
**** 版本信息:  	，高响应优先级不能打断，但是两个中断同时触发时，高响应优先级先执行中断
**** 说  明：    	如果两个中断 抢占和响应优先级相同时触发，看中断向量，
****			     		配置中断，必须先配置一个中断组，一个工程只能配置一个中断组， 在【8-10位】
****							组0					0x700    111
****							组1					0x600		 110
****							组2					0x500		 101
****							组3					0x400		 100
****							组4					0x300		 011
****					中断有八个位，256个中断（0-255）数，其中16个中断为不可配置,240个中断可配置
****					抢占和响应优先级可配置数为16（2^4）,8个位的高四位，抢占在【6-7】位，响应在【4-5】位
********************************************************************************************************/

USART1_TYPE usart1_type; //定义一个结构体变量，

void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef   GPIO_InitStruct;
	USART_InitTypeDef  USART_InitStruct;
	//NVIC_InitTypeDef   NVIC_InitStruct;
	//1.使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //开启GPIOA的时钟
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;        //初始化引脚9
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;  //输出速度为10MHz
	GPIO_InitStruct.GPIO_Mode  =  GPIO_Mode_AF_PP;  //复用推挽输出 
	GPIO_Init(GPIOA,&GPIO_InitStruct);						  //初始化GPIOA
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;        		//初始化引脚10
	GPIO_InitStruct.GPIO_Mode  =  GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStruct);						  		//初始化GPIOA
	
	//2.配置串口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); //开启USART1的时钟
	USART_InitStruct.USART_BaudRate 					 = bound;   	//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //不使用硬件流控制
	USART_InitStruct.USART_Mode 							 = USART_Mode_Rx | USART_Mode_Tx; //发送和接收模式
	USART_InitStruct.USART_Parity 						 = USART_Parity_No ;              //无校验
	USART_InitStruct.USART_StopBits						 = USART_StopBits_1;              //1个停止位
	USART_InitStruct.USART_WordLength					 = USART_WordLength_8b ;          //8个数据位
	//USART_StructInit(&USART_InitStruct);
	USART_Init(USART1,&USART_InitStruct);
	
	//1）配置NVIC
//	NVIC_InitStruct.NVIC_IRQChannel 	 								= USART1_IRQn;  //串口1的中断
//	NVIC_InitStruct.NVIC_IRQChannelCmd 								= ENABLE;			  //使能NVIC
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;            //抢占优先级
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;            //响应优先级（子优先级）
//  NVIC_Init(&NVIC_InitStruct);
	
	//2)开启串口的接收中断
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);

		USART1->CR1 |=1<<4; //空闲中断
		USART1->CR1 |=1<<5;//接收中断

 		NVIC->ISER[1] |= 1<<(37-32); //串口1的中断
		NVIC->IP[37]   = 0X30; //抢占0，响应2										
	USART_Cmd(USART1,ENABLE);
}

//3)编写中断服务函数    
void USART1_IRQHandler(void)
{
  //BEEPON
//	if(USART1->SR&1<<5)//触发的是接收中断
//	{		
//		usart1_type.usart1_readBuf[usart1_type.usart1_readLen++] = USART_RecByte(USART1); //读数据清标志;     //存放收到的字节数
//	}else if(USART1->SR&1<<4)//触发的是空闲中断
//	{
//		usart1_type.usart1_readFlag =0x80; //接收完成标志
//		usart1_type.usart1_readBuf[usart1_type.usart1_readLen] = '\0';//添加结束标志 
//		//由软件序列清除该位(先读USART_SR，然后读USART_DR)
//		USART_SendString(USART1,usart1_type.usart1_readBuf);
//		USART1->SR; //读SR
//		USART1->DR; //读DR
//	}
}


/*****加入以下代码,支持printf函数,而不需要选择use MicroLIB*****/	  
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
	void _ttywrch(int ch)
	{
		ch = ch;
	}
	//重定义fputc函数 
	int fputc(int ch, FILE *f)
	{ 	
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1, ch);	
		return ch;
	}
/**********************printf support end**********************/	  
#endif


void USART_SendByte(USART_TypeDef* USARTx, uint8_t dat)
{
		// SET    -->已经发送完成
		// RESET  -->正在发送
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET)
		{
			; //获取发送完成的状态并等待它发送完成
		}
		USART_SendData(USARTx,dat);
}

u8 USART_RecByte(USART_TypeDef* USARTx)
{
		// SET    -->已经收到数据
		// RESET  -->正在接收数据
		while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)==RESET)
		{
			; //获取接收完成的状态并等待它接收完成
		}	
		// "hello world\r\n"
	return USART_ReceiveData(USARTx); //接收一个字节
}

//u8 str[] = "hello world";
//u8 dat[] = {0x01,0x02,0x03,0x04,0x05};
void USART_SendString(USART_TypeDef* USARTx, uint8_t *str)
{
	while(*str != '\0')
	{
		USART_SendByte(USARTx,*str);
		str++;
	}
}

u8 Usart1_readBuf[100];
void USART_ReceiveString(USART_TypeDef* USARTx)
{
		u8 readLen = 0;
		u8 dat;
		while(1)
		{
			dat = USART_RecByte(USARTx);
			if(dat != '\r' && dat != '\n')   //收到的数据不是'\r' '\n'
			{
				Usart1_readBuf[readLen] = dat;     //存放收到的字节数
				readLen++;
			}
			if(dat == '\n') //接收完成
			{
				break;   //退出循环
			}
	 }
}


		
		
		