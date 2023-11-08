#ifndef USART_H
#define USART_H
#include "stm32f10x.h"
#include <stdio.h>

typedef struct{
	u8 usart1_readBuf[256];//用来存放收到的字符串
	u8 usart1_readLen; 		//串口1接收到的数据长度
	u8 usart1_readFlag;   //串口1接收一帧数据标志
}USART1_TYPE;

extern USART1_TYPE usart1_type;
void USART1_Init(u32 bound);
u8 USART_RecByte(USART_TypeDef* USARTx);
void USART_SendByte(USART_TypeDef* USARTx, uint8_t dat);
void USART_SendString(USART_TypeDef* USARTx, uint8_t *str);
void USART_ReceiveString(USART_TypeDef* USARTx);

#endif



