#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"
#include "io_bit.h"
#define KEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define KEY2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define KEY3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)


void Led_Init(void);

#define LED1 PBout(5)	// PB5
#define LED2 PBout(4)	// PB4
#define LED3 PBout(3)	// PB3
#define LED4 PAout(15)	// PA8	

void KEY_Init(void);
#endif