#ifndef _USART1_H_
#define _USART1_H_
#include "stm32f10x.h" 
#include "stdio.h"
typedef struct
{
   u8  USART1_BUFF[255];//���յ�����
   u8 USART1_FLAG;     //���ձ�־
} Usart1;
extern  Usart1 Read;
extern u8 Usart1_Readbuff[100];//�������Ե��Ե�����
void Usart1_Init(u32 bound);
void Usart1_Send_byte(u8 data);
u8   Usart1_Read_byte(void);
void Usart1_Send_string(u8 *ch);
void Usart1_Read_string(void);

#endif

