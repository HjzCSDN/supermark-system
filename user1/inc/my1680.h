#ifndef MY1680_H
#define MY1680_H
#include "stm32f10x.h"
//#include "io_bit.h"

extern u8 buff3[100];
extern u8 flag3;
extern int page ;

void voice_send_command(u8 command,u8 grama);
void voice_send_command2(u8 gq,u8 jy );
void Speech_algorithm(u8 i) ;

u8 usart3_recv_byte(void);
void usart3_send_byte(u8 data);
void usart3_init(u32 bps);
void usart3_recv_string(u8 *data);
void usart3_send_string(u8 *data);
void data_judge(u8 *buff);
void cloth_remind(void);
void data_judge_tem(u8 str1,u8 str2);
void data_judge_hum(u8 str1,u8 str2);
#endif
