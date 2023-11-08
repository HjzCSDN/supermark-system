#ifndef _USART2_H_
#define _USART2_H_
#include "stm32f10x.h"

typedef struct{
	u8 usart2_readBuf[256];//��������յ����ַ���
	u8 usart2_readLen; 		//����1���յ������ݳ���
	u16 usart2_readFlag;   //����1����һ֡���ݱ�־
}USART2_TYPE;
extern USART2_TYPE usart2_type;

void USART2_Init();

void Usart2_Send_byte(u8 data);
void Usart2_Send_string(u8 *ch);
	
#endif
