#include "delay.h"
u8 fac_us=0;
u16 fac_ms=0;

void Delay_Init(u16 systick)//
{
    fac_us=systick/8;//1US时间
    fac_ms=fac_us*1000;//1ms时间
    SysTick->CTRL &=~(1<<2);//选择外部时钟
}

void Delay_Us(u32 nus)
{
	u32 temp;
    SysTick->LOAD=nus*fac_us;
	SysTick->VAL = 0X00;//清除当前寄存器的值 清空计数器
	SysTick->CTRL|=1<<0;//使能系统滴答
	do{
	      temp=SysTick->CTRL;//获取整个寄存器的值
	}while(!(temp & 1<<16)&&(temp & 1<<0));//判断时间到 判断是否使能
	SysTick->CTRL &=~(1<<0); //关闭系统滴答
	SysTick->VAL = 0X00;//清除当前寄存器的值 清空计数器
}

void Delay_Ms(u32 nms)
{
	u32 temp;
    SysTick->LOAD=nms*fac_ms;
	SysTick->VAL = 0X00;//清除当前寄存器的值 清空计数器
	SysTick->CTRL|=1<<0;//使能系统滴答
	do{
	      temp=SysTick->CTRL;//获取整个寄存器的值
	}while(!(temp & 1<<16)&&(temp & 1<<0));//判断时间到 判断是否使能
	SysTick->CTRL &=~(1<<0); //关闭系统滴答
	SysTick->VAL = 0X00;//清除当前寄存器的值 清空计数器
}


