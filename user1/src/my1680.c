#include "my1680.h"
#include <stdio.h>
#include "usart1.h"
#include "delay.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*******************************************************************************************************
**** 程序名称：
**** 设计者:      Mr'K    日期：2022年 1月 21日
**** 修改者:      Mr'K    日期：2022年 1月 21日
**** 功  能：			语音播放函数
**** 版本信息:   void Speech_algorithm(u8 i) 指定歌曲，校验码要异或两次，校验码不对指定歌曲不能发送
**** 说  明：    指定歌曲校验这里一定要注意啊，模块手册知识轻描淡写的概述，没有很明确的说明，这里也是我的导师告诉我才知道的，之前一直弄不出来
****编程思路：   提前放好语音，然后语音发送
********************************************************************************************************/
/*
函数功能：指令发送
返回值：无
参数：u8 i
*/
void voice_send_command(u8 command,u8 grama)
{
	usart3_send_byte(0X7E);
	usart3_send_byte(0X03);
	usart3_send_byte(command);
	usart3_send_byte(grama);
	usart3_send_byte(0XEF);
}
/*
函数功能：指令发送
返回值：无
参数：u8 i
*/
void Speech_algorithm(u8 i)
{
	u8 j;
	usart3_send_byte(0X7E);
	usart3_send_byte(0X05);
	usart3_send_byte(0x41);
	usart3_send_byte(0x00);
	usart3_send_byte(i);
	j=0x41^0X05^i;				//校验，具体去看my1680手册，这里就不展开了
	usart3_send_byte(j);
	usart3_send_byte(0XEF);
}




/*
函数功能：串口3初始化
返回值：无
参数：u32 bps
*/
void usart3_init(u32 bps)
{
	GPIO_InitTypeDef GPIO_InitStructure;//声明GPIO结构体变量
	USART_InitTypeDef USART_InitStruct;//声USART结构体变量
	NVIC_InitTypeDef NVIC_InitStruct;//声明NVIC结构体变量

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);

	/* Configure USART3 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //?′
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //????ê?è??￡ê?
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	USART_InitStruct.USART_BaudRate=bps;//设置波特率
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//不用硬件流
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;//设置为接收和发送
	USART_InitStruct.USART_Parity=USART_Parity_No;//无奇偶校验
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//设置停止位1
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//设置数据位长8位

	USART_Init(USART3, &USART_InitStruct);//初始化串口

	//中断配置
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启接收中断使能
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启空闲中断使能
	//USART1->CR1 |= 1<<4;//开启空闲中断使能
//	NVIC_InitStruct.NVIC_IRQChannel=USART3_IRQn;//选择通道
//	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//使能NVIC控制器
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//设置强制式优先级
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;//设置响应式优先级
//	NVIC_Init(&NVIC_InitStruct);//初始化NVIC结构体
//	//NVIC_EnableIRQ(USART1_IRQn);//选择中断源

	USART_Cmd(USART3, ENABLE);//使能串口

}
/*
函数功能：中断服务函数
参数：无
返回值：无
备注：中断服务函数无返回值，无参数
*/
u8 buff3[100];
u8 flag3=0;
void USART3_IRQHandler(void)
{
	u8 data=0;
	static u8 cnt=0;//计算有多少个字节
	if(USART_GetITStatus(USART3, USART_IT_RXNE)==SET)//判断接收状态标志位是不是置1
	{
		USART_ClearFlag(USART3, USART_IT_RXNE);//清标志位
		//实现中断要干的事情
		//data=USART1->DR;
		data=USART_ReceiveData(USART3);
		if(flag3)
		{
			return;
		}
		if(cnt<100)
		{
			buff3[cnt++]=data;//将接收一个字节存储在数组里面
		}
	}
	if(USART_GetITStatus(USART3, USART_IT_IDLE)==SET)//检测接收状态是不是空闲
	{
		data=USART_ReceiveData(USART3);//清零
		buff3[cnt]='\0';//保证字符串是以'\0'结尾
		flag3=cnt;
		cnt=0;
	}
}
/*
函数功能：发送一个字符
返回值：无
参数：u8 data
*/
void usart3_send_byte(u8 data)
{
	while(!USART_GetFlagStatus(USART3,USART_FLAG_TXE))
	{

	}
	USART_SendData(USART3, data);//发送数据
}
/*
函数功能：接收一个字符
返回值：u8
参数：无
*/
u8 usart3_recv_byte(void)
{
	while(!USART_GetFlagStatus(USART3,USART_FLAG_RXNE))
	{

	}
	return USART_ReceiveData(USART3);//将接收的数据返回
}
/*
函数功能：发送字符串
返回值：无
参数：u8 *data
*/
void usart3_send_string(u8 *data)
{
	while(*data != '\0')
	{
		usart3_send_byte(*data);
		data++;
	}
	usart3_send_byte('\0');
}
/*
函数功能：接收字符串
返回值：无
参数：u8 *data
*/
void usart3_recv_string(u8 *data)
{
	u8 recv=0;
	while(1)
	{
		recv=usart3_recv_byte();
		if(recv == '\r' || recv == '\n')
		{
			break;
		}
		*data=recv;
		data++;
	}
	usart3_send_byte('\r');
	usart3_send_byte('\n');
}


////////////////////////////////////////////////////////////////////
////加入以下代码,支持printf函数,而不需要选择use MicroLIB
//#if 1
//#pragma import(__use_no_semihosting)
////标准库需要的支持函数
//struct __FILE
//{
//	int handle;

//};

//FILE __stdout;
////定义_sys_exit()以避免使用半主机模式
//void _sys_exit(int x)
//{
//	x = x;
//}
////重定义fputc函数
//int fputc(int ch, FILE *f)
//{
//	  while((USART3->SR&0X40)==0);//循环发送,直到发送完毕
//    USART3->DR = (u8) ch;
//	  return ch;
//}
//#endif

int cloth;
u8 ff;
/*******************************************************************************************************
**** 程序名称：
**** 设计者:      Mr'K    起始日期：2022年 1月 23日
**** 修改者:      Mr'K    末止日期：2022年 1月 23日
**** 功  能：			算法函数
**** 版本信息:  	1.0
**** 说  明：
**** 编程思路：		通过数组接收数据并处理，用标志位判断是 万、千、百、十、个位等等,小数处理是变量c处理，精确两位小数，
**** 注意事项：   小数点这里不能乱给延时和printf,不然会导致语音播报错误
********************************************************************************************************/
void data_judge(u8 *buff)
{
 float a, c;							//a 接收数据 , c 取数据的小数部分，精确到两位
    int i = 5, j, num_flag; // i最多只运算到万，所以i给5，   numflag 一个接收的标志位
    int money[5]={0} 		//判断是总共多少位数： 
																//money[4]=5 万位    money[3]=4 千位    money[2]=3 百位    money[1]=2 十位    money[0]=1 个位    
				, sound[5]={0};		//位数上的数值大小，范围x：0~9， 
																//sound[x]=5 万位    sound[x]=4 千位    sound[x]=3 百位    sound[x]=2 十位    sound[x]=1 个位
		
		

        a = strtod((char *)buff, NULL);       //字符类型转浮点型数据
        c = (a - (int)a) * 100;                                     //取小数
//            printf("a=%.2f\r\n", a);
				Speech_algorithm(16);  //内容：请支付
        while (i--)
        {
            if (((int)a) / ((int)pow(10, i)) % 10 > 0)   						 // (int)a)强制转换数据类型，如果这里看不懂，好好用C语言编译器练练
            {
                money[i] = i + 1;
                for (j = i; j >= 0; j--)
                {
                    sound[j] = ((int)a) / ((int)pow(10, j)) % 10;   // ((int)pow(10, j))如果j=4  10000  万位，j- 依次类推  %10这里就是取位数的数值
                }																																					//j=3  1000   千位
                break;																																		//j=2  100    百位
            }																																							//j=1  10   	十位
        }																																									//j=0  1   		万位
        num_flag = money[i]-1; 																			//判断位数，这里赋值时要-1，不然会给你多判断一个位数      		
				i==-1?num_flag=0:0;																					//if(i==-1)num_flag=0;  这里是三目运算与后面的语句等同，   
//            printf("money[%d]=%d\r\n\r\n", i, money[i]);					//											大家可以学一下三目运算运用到程序里面
//            printf("num_flag=%d\r\n\r\n", num_flag);							//											与if结构相同

        if (num_flag >= 4) //万
        {
            printf("万sound[4]=%d\r\n", sound[4]);
            Speech_algorithm(sound[4]);
            delay_ms(6);
            Speech_algorithm(13);

        }
        if (num_flag >= 3) //千
        {
            printf("千sound[3]=%d\r\n", sound[3]);
            Speech_algorithm(sound[3]);
            delay_ms(6);
            Speech_algorithm(12);
        }
        if (num_flag >= 2) //百
        {
            printf("百sound[2]=%d\r\n", sound[2]);
            Speech_algorithm(sound[2]);
            delay_ms(6);
            Speech_algorithm(11);
        }
        if (num_flag >= 1) //十
        {
            printf("十sound[1]=%d\r\n", sound[1]);
            Speech_algorithm(sound[1]);
            delay_ms(6);
            Speech_algorithm(10);
        }
        if (num_flag >= 0) //（个）元
        {
            printf("个sound[0]=%d\r\n", sound[0]);
            Speech_algorithm(sound[0]);
            delay_ms(6);
            if (c != 0)
            {
                Speech_algorithm(14);
                delay_ms(6);
                Speech_algorithm((u8)c / 10 % 10);
                delay_ms(6);
                if ((u8)c % 10 != 0)
                {
                    Speech_algorithm((u8)c % 10);
                    delay_ms(6);
                }
            }
            Speech_algorithm(15);
        }
        i = 5;   																													//重装载值，以便下次计算

}






int page=1;
void data_judge_tem(u8 str1,u8 str2)
{

    float a, c;							//a 接收数据 , c 取数据的小数部分，精确到两位
    int i = 5, j, num_flag; // i最多只运算到万，所以i给5，   numflag 一个接收的标志位
    int money[5]={0} 		//判断是总共多少位数：
												//money[4]=5 万位    money[3]=4 千位    money[2]=3 百位    money[1]=2 十位    money[0]=1 个位
				, sound[5]={0};		//位数上的数值大小，范围x：0~9，
																//sound[x]=5 万位    sound[x]=4 千位    sound[x]=3 百位    sound[x]=2 十位    sound[x]=1 个位

				cloth=(str1+str2)/2;
			printf("str1=%d-str2=%d\r\n",str1,str2);
			a=str1;
        while (i--)
        {
            if (((int)a) / ((int)pow(10, i)) % 10 > 0)   						 // (int)a)强制转换数据类型，如果这里看不懂，好好用C语言编译器练练
            {
                money[i] = i + 1;
                for (j = i; j >= 0; j--)
                {
                    sound[j] = ((int)a) / ((int)pow(10, j)) % 10;   // ((int)pow(10, j))如果j=4  10000  万位，j- 依次类推  %10这里就是取位数的数值
                }																																					//j=3  1000   千位
                break;																																		//j=2  100    百位
            }																																							//j=1  10   	十位
        }																																									//j=0  1   		万位
        num_flag = money[i]-1; 																			//判断位数，这里赋值时要-1，不然会给你多判断一个位数
				i==-1?num_flag=0:0;																					//if(i==-1)num_flag=0;  这里是三目运算与后面的语句等同，


				if(page==1)Speech_algorithm(32);
				else if(page==2)Speech_algorithm(33);
				else if(page==3)Speech_algorithm(34);
				else if(page==4)Speech_algorithm(35);
				else if(page==5)Speech_algorithm(36);
				else if(page==6)Speech_algorithm(37);
				else if(page==7)Speech_algorithm(38);


				Speech_algorithm(42);

        if (num_flag >= 1) //十
        {
            printf("十sound[1]=%d\r\n", sound[1]);
            Speech_algorithm(sound[1]);
            delay_ms(6);
            Speech_algorithm(10);
        }
        if (num_flag >= 0) //（个）元
        {
            printf("个sound[0]=%d\r\n", sound[0]);
            Speech_algorithm(sound[0]);
            delay_ms(6);
            //Speech_algorithm(15);
        }

				i = 5;
				Speech_algorithm(15);

					a=str2;
        while (i--)
        {
            if (((int)a) / ((int)pow(10, i)) % 10 > 0)   						 // (int)a)强制转换数据类型，如果这里看不懂，好好用C语言编译器练练
            {
                money[i] = i + 1;
                for (j = i; j >= 0; j--)
                {
                    sound[j] = ((int)a) / ((int)pow(10, j)) % 10;   // ((int)pow(10, j))如果j=4  10000  万位，j- 依次类推  %10这里就是取位数的数值
                }																																					//j=3  1000   千位
                break;																																		//j=2  100    百位
            }																																							//j=1  10   	十位
        }																																									//j=0  1   		万位
        num_flag = money[i]-1; 																			//判断位数，这里赋值时要-1，不然会给你多判断一个位数
				i==-1?num_flag=0:0;																					//if(i==-1)num_flag=0;  这里是三目运算与后面的语句等同，


        if (num_flag >= 1) //十
        {
            printf("十sound[1]=%d\r\n", sound[1]);
            Speech_algorithm(sound[1]);
            delay_ms(6);
            Speech_algorithm(10);
        }
        if (num_flag >= 0) //（个）元
        {
            printf("个sound[0]=%d\r\n", sound[0]);
            Speech_algorithm(sound[0]);
            delay_ms(6);
            //Speech_algorithm(15);
        }
        i = 5;
				Speech_algorithm(14);	//摄氏度




//				ff=1;
//		if(ff==1)
//		{
//		cloth_remind();
//			ff=0;
//		}
}



void data_judge_hum(u8 str1,u8 str2)
{

    float a, c;							//a 接收数据 , c 取数据的小数部分，精确到两位
    int i = 5, j, num_flag; // i最多只运算到万，所以i给5，   numflag 一个接收的标志位
    int money[5]={0} 		//判断是总共多少位数：
												//money[4]=5 万位    money[3]=4 千位    money[2]=3 百位    money[1]=2 十位    money[0]=1 个位
				, sound[5]={0};		//位数上的数值大小，范围x：0~9，
																//sound[x]=5 万位    sound[x]=4 千位    sound[x]=3 百位    sound[x]=2 十位    sound[x]=1 个位

				//cloth=(str1+str2)/2;
			printf("str1=%d-str2=%d\r\n",str1,str2);
			a=str1;
        while (i--)
        {
            if (((int)a) / ((int)pow(10, i)) % 10 > 0)   						 // (int)a)强制转换数据类型，如果这里看不懂，好好用C语言编译器练练
            {
                money[i] = i + 1;
                for (j = i; j >= 0; j--)
                {
                    sound[j] = ((int)a) / ((int)pow(10, j)) % 10;   // ((int)pow(10, j))如果j=4  10000  万位，j- 依次类推  %10这里就是取位数的数值
                }																																					//j=3  1000   千位
                break;																																		//j=2  100    百位
            }																																							//j=1  10   	十位
        }																																									//j=0  1   		万位
        num_flag = money[i]-1; 																			//判断位数，这里赋值时要-1，不然会给你多判断一个位数
				i==-1?num_flag=0:0;																					//if(i==-1)num_flag=0;  这里是三目运算与后面的语句等同，

//				if(page==0)Speech_algorithm(13);
//				else if(page==1)Speech_algorithm(32);
//				else if(page==2)Speech_algorithm(33);
//				else if(page==3)Speech_algorithm(34);
//				else if(page==4)Speech_algorithm(35);
//				else if(page==5)Speech_algorithm(36);
//				else if(page==6)Speech_algorithm(37);
//				else if(page==7)Speech_algorithm(38);


				Speech_algorithm(40);

        if (num_flag >= 1) //十
        {
            printf("十sound[1]=%d\r\n", sound[1]);
            Speech_algorithm(sound[1]);
            delay_ms(6);
            Speech_algorithm(10);
        }
        if (num_flag >= 0) //（个）元
        {
            printf("个sound[0]=%d\r\n", sound[0]);
            Speech_algorithm(sound[0]);
            delay_ms(6);
            //Speech_algorithm(15);
        }

				i = 5;
				Speech_algorithm(15);

					a=str2;
        while (i--)
        {
            if (((int)a) / ((int)pow(10, i)) % 10 > 0)   						 // (int)a)强制转换数据类型，如果这里看不懂，好好用C语言编译器练练
            {
                money[i] = i + 1;
                for (j = i; j >= 0; j--)
                {
                    sound[j] = ((int)a) / ((int)pow(10, j)) % 10;   // ((int)pow(10, j))如果j=4  10000  万位，j- 依次类推  %10这里就是取位数的数值
                }																																					//j=3  1000   千位
                break;																																		//j=2  100    百位
            }																																							//j=1  10   	十位
        }																																									//j=0  1   		万位
        num_flag = money[i]-1; 																			//判断位数，这里赋值时要-1，不然会给你多判断一个位数
				i==-1?num_flag=0:0;																					//if(i==-1)num_flag=0;  这里是三目运算与后面的语句等同，


        if (num_flag >= 1) //十
        {
            printf("十sound[1]=%d\r\n", sound[1]);
            Speech_algorithm(sound[1]);
            delay_ms(6);
            Speech_algorithm(10);
        }
        if (num_flag >= 0) //（个）元
        {
            printf("个sound[0]=%d\r\n", sound[0]);
            Speech_algorithm(sound[0]);
            delay_ms(6);
            //Speech_algorithm(15);
        }
        i = 5;
				Speech_algorithm(41);	//摄氏度




//				ff=1;
//		if(ff==1)
//		{
//
//			ff=0;
//		}
}
