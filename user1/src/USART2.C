#include "USART2.h"
#include <string.h>
#include <stdio.h>
//#include "usart1.h"
USART2_TYPE usart2_type;

void USART2_Init()   //�⺯���÷�
{
    GPIO_InitTypeDef   GPIO_InitStructure;
    USART_InitTypeDef  USART_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStruct;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA,&GPIO_InitStructure);


    USART_InitStructure.USART_BaudRate=115200;
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;
    USART_InitStructure.USART_Parity=USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
    USART_Init(USART2,&USART_InitStructure);

    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
    USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
    USART_Cmd(USART2,ENABLE);

    //1������NVIC
    NVIC_InitStruct.NVIC_IRQChannel 	 								= USART2_IRQn;  //����1���ж�
    NVIC_InitStruct.NVIC_IRQChannelCmd 								= ENABLE;			  //ʹ��NVIC
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;            //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;            //��Ӧ���ȼ��������ȼ���
    NVIC_Init(&NVIC_InitStruct);

}



//int fputc(int ch,FILE *f)
//{
//   // USART1->SR;  //USART_GetFlagStatus(USART1, USART_FLAG_TC) �����һ���ַ�����ʧ�ܵ�����
//    //һ��һ�������ַ�
//   // USART_SendByte(USART1, (unsigned char) ch);
//    //�ȴ��������
//    //while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
//		while((USART2->SR&1<<6)==0);
//		USART2->DR=ch;
//    return(ch);
//}

u8 Usart2_Read_byte(void)
{
    //while(!(USART1->SR & 1<<5));//�ȴ��������
    // return USART1->DR;
    while(!USART_GetFlagStatus(USART2,USART_FLAG_RXNE));
    return  USART_ReceiveData(USART2);
}


/*****************************************************************************************
������  :
����    :����һ�ֽں���
���ڲ���:
��ڲ���:���͵�����
����ֵ  :
******************************************************************************************/

void Usart2_Send_byte(u8 data)
{
    // while(!(USART1->SR & 1<<6));//�ȴ��������
    //USART1->DR =data;           //��������һ���ֽ�
    while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
    USART_SendData(USART2, data);
}


void Usart2_Send_string(u8 *ch)
{
    while(*ch!='\0')
    {
        Usart2_Send_byte(*ch++);
    }
}



void USART2_IRQHandler(void)
{
    if(USART2->SR&1<<5)//�������ǽ����ж�
    {
        usart2_type.usart2_readBuf[usart2_type.usart2_readLen++] = Usart2_Read_byte(); //���������־;     //����յ����ֽ���
    }
    else if(USART2->SR&1<<4)//�������ǿ����ж�
    {
        usart2_type.usart2_readFlag =0x80; //������ɱ�־
        usart2_type.usart2_readBuf[usart2_type.usart2_readLen] = '\0';//���ӽ�����־
        //���������������λ(�ȶ�USART_SR��Ȼ���USART_DR)
        //USART_SendString(USART2,usart2_type.usart2_readBuf);			
				//printf("%s\r\n",usart2_type.usart2_readBuf);
        USART2->SR; //��SR
        USART2->DR; //��DR
    }
}
