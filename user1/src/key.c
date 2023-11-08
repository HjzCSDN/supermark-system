#include "key.h"



void Led_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);//��ʱ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);



    GPIO_InitStruct.GPIO_Mode =GPIO_Mode_Out_PP;//��������ģʽ
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;     //��������
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//����IO�ڷ�ת�ٶ�
    GPIO_Init(GPIOB, &GPIO_InitStruct);         //��ʼ��
    GPIO_SetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);            //PB1�øߵ�ƽ

    GPIO_InitStruct.GPIO_Mode =GPIO_Mode_Out_PP;//��������ģʽ
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_15;     //��������
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//����IO�ڷ�ת�ٶ�
    GPIO_Init(GPIOA, &GPIO_InitStruct);         //��ʼ��
    GPIO_SetBits(GPIOA, GPIO_Pin_15);            //PB1�øߵ�ƽ


}

void KEY_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //����GPIOA��ʱ��
   // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;        //��ʼ������
    GPIO_InitStruct.GPIO_Mode  =  GPIO_Mode_IN_FLOATING; //��������ģʽ
    GPIO_Init(GPIOA,&GPIO_InitStruct);						  //��ʼ��GPIOA
}