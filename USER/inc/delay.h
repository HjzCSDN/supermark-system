#ifndef _DELAY_H_
#define _DELAY_H_
#include "stm32f10x.h"  
void Delay_Init(u16 systick);
void Delay_Us(u32 nus);
void Delay_Ms(u32 nms);
#endif
