#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK miniSTM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 



#define KEY12   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//��ȡ����1
#define KEY13   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//��ȡ����2 
#define KEY14   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//��ȡ����2  

#define KEY12_PRES	1		//KEY0  
#define KEY13_PRES	2		//KEY1 
#define KEY14_PRES	3		//KEY3 

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8 mode);  	//����ɨ�躯��					    
#endif


