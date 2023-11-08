#ifndef __BSP_USART_H__
#define __BSP_USART_H__
#include "stm32f10x.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

extern u8 wifi_uart_buff[1024];
extern u16 wifi_uart_len;


void uart2_init(u32 bound) ;
void uart2_send_data(u8 data);
void wifi_fputc(char str);
void wifi_printf(char *str,...);

#endif
