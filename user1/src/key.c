#include "key.h"



void Led_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);//开时钟

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);



    GPIO_InitStruct.GPIO_Mode =GPIO_Mode_Out_PP;//设置推挽模式
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;     //设置引脚
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//设置IO口翻转速度
    GPIO_Init(GPIOB, &GPIO_InitStruct);         //初始化
    GPIO_SetBits(GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);            //PB1置高电平

    GPIO_InitStruct.GPIO_Mode =GPIO_Mode_Out_PP;//设置推挽模式
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_15;     //设置引脚
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//设置IO口翻转速度
    GPIO_Init(GPIOA, &GPIO_InitStruct);         //初始化
    GPIO_SetBits(GPIOA, GPIO_Pin_15);            //PB1置高电平


}

void KEY_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //开启GPIOA的时钟
   // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;        //初始化引脚
    GPIO_InitStruct.GPIO_Mode  =  GPIO_Mode_IN_FLOATING; //浮空输入模式
    GPIO_Init(GPIOA,&GPIO_InitStruct);						  //初始化GPIOA
}