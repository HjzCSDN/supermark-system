#include "usart.h"
#include <string.h>
#include <stdio.h>

/*******************************************************************************************************
**** �������ƣ�   �ж�֪ʶ
**** �����:      Mr'K    ���ڣ�2022�� 1�� 15��
**** �޸���:      Mr'K    ���ڣ�2022�� 1�� 15��
**** ��  �ܣ�			�����ж���ռ���ȼ�����Ӧ���ȼ���ͬʱ�� ������Ӧ���ȼ�ִ���ж�ʱ
**** �汾��Ϣ:  	������Ӧ���ȼ����ܴ�ϣ����������ж�ͬʱ����ʱ������Ӧ���ȼ���ִ���ж�
**** ˵  ����    	��������ж� ��ռ����Ӧ���ȼ���ͬʱ���������ж�������
****			     		�����жϣ�����������һ���ж��飬һ������ֻ������һ���ж��飬 �ڡ�8-10λ��
****							��0					0x700    111
****							��1					0x600		 110
****							��2					0x500		 101
****							��3					0x400		 100
****							��4					0x300		 011
****					�ж��а˸�λ��256���жϣ�0-255����������16���ж�Ϊ��������,240���жϿ�����
****					��ռ����Ӧ���ȼ���������Ϊ16��2^4��,8��λ�ĸ���λ����ռ�ڡ�6-7��λ����Ӧ�ڡ�4-5��λ
********************************************************************************************************/

USART1_TYPE usart1_type; //����һ���ṹ�������

void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef   GPIO_InitStruct;
	USART_InitTypeDef  USART_InitStruct;
	//NVIC_InitTypeDef   NVIC_InitStruct;
	//1.ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //����GPIOA��ʱ��
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;        //��ʼ������9
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;  //����ٶ�Ϊ10MHz
	GPIO_InitStruct.GPIO_Mode  =  GPIO_Mode_AF_PP;  //����������� 
	GPIO_Init(GPIOA,&GPIO_InitStruct);						  //��ʼ��GPIOA
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;        		//��ʼ������10
	GPIO_InitStruct.GPIO_Mode  =  GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA,&GPIO_InitStruct);						  		//��ʼ��GPIOA
	
	//2.���ô���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); //����USART1��ʱ��
	USART_InitStruct.USART_BaudRate 					 = bound;   	//������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��ʹ��Ӳ��������
	USART_InitStruct.USART_Mode 							 = USART_Mode_Rx | USART_Mode_Tx; //���ͺͽ���ģʽ
	USART_InitStruct.USART_Parity 						 = USART_Parity_No ;              //��У��
	USART_InitStruct.USART_StopBits						 = USART_StopBits_1;              //1��ֹͣλ
	USART_InitStruct.USART_WordLength					 = USART_WordLength_8b ;          //8������λ
	//USART_StructInit(&USART_InitStruct);
	USART_Init(USART1,&USART_InitStruct);
	
	//1������NVIC
//	NVIC_InitStruct.NVIC_IRQChannel 	 								= USART1_IRQn;  //����1���ж�
//	NVIC_InitStruct.NVIC_IRQChannelCmd 								= ENABLE;			  //ʹ��NVIC
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;            //��ռ���ȼ�
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;            //��Ӧ���ȼ��������ȼ���
//  NVIC_Init(&NVIC_InitStruct);
	
	//2)�������ڵĽ����ж�
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
//	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);

		USART1->CR1 |=1<<4; //�����ж�
		USART1->CR1 |=1<<5;//�����ж�

 		NVIC->ISER[1] |= 1<<(37-32); //����1���ж�
		NVIC->IP[37]   = 0X30; //��ռ0����Ӧ2										
	USART_Cmd(USART1,ENABLE);
}

//3)��д�жϷ�����    
void USART1_IRQHandler(void)
{
  //BEEPON
//	if(USART1->SR&1<<5)//�������ǽ����ж�
//	{		
//		usart1_type.usart1_readBuf[usart1_type.usart1_readLen++] = USART_RecByte(USART1); //���������־;     //����յ����ֽ���
//	}else if(USART1->SR&1<<4)//�������ǿ����ж�
//	{
//		usart1_type.usart1_readFlag =0x80; //������ɱ�־
//		usart1_type.usart1_readBuf[usart1_type.usart1_readLen] = '\0';//��ӽ�����־ 
//		//��������������λ(�ȶ�USART_SR��Ȼ���USART_DR)
//		USART_SendString(USART1,usart1_type.usart1_readBuf);
//		USART1->SR; //��SR
//		USART1->DR; //��DR
//	}
}


/*****�������´���,֧��printf����,������Ҫѡ��use MicroLIB*****/	  
#if 1
	#pragma import(__use_no_semihosting)             
	//��׼����Ҫ��֧�ֺ���                 
	struct __FILE 
	{ 
		int handle; 
	}; 

	FILE __stdout;       
	//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
	void _sys_exit(int x) 
	{ 
		x = x; 
	} 
	void _ttywrch(int ch)
	{
		ch = ch;
	}
	//�ض���fputc���� 
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
		// SET    -->�Ѿ��������
		// RESET  -->���ڷ���
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET)
		{
			; //��ȡ������ɵ�״̬���ȴ����������
		}
		USART_SendData(USARTx,dat);
}

u8 USART_RecByte(USART_TypeDef* USARTx)
{
		// SET    -->�Ѿ��յ�����
		// RESET  -->���ڽ�������
		while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)==RESET)
		{
			; //��ȡ������ɵ�״̬���ȴ����������
		}	
		// "hello world\r\n"
	return USART_ReceiveData(USARTx); //����һ���ֽ�
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
			if(dat != '\r' && dat != '\n')   //�յ������ݲ���'\r' '\n'
			{
				Usart1_readBuf[readLen] = dat;     //����յ����ֽ���
				readLen++;
			}
			if(dat == '\n') //�������
			{
				break;   //�˳�ѭ��
			}
	 }
}


		
		
		