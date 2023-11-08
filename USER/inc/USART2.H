#ifndef _USART2_H_
#define _USART2_H_
#include "stm32f10x.h"

typedef struct{
	u8 usart2_readBuf[256];//用来存放收到的字符串
	u8 usart2_readLen; 		//串口1接收到的数据长度
	u16 usart2_readFlag;   //串口1接收一帧数据标志
}USART2_TYPE;
extern USART2_TYPE usart2_type;

void USART2_Init();
#endif

