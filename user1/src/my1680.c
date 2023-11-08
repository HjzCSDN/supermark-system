#include "my1680.h"
#include <stdio.h>
#include "usart1.h"
#include "delay.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*******************************************************************************************************
**** �������ƣ�
**** �����:      Mr'K    ���ڣ�2022�� 1�� 21��
**** �޸���:      Mr'K    ���ڣ�2022�� 1�� 21��
**** ��  �ܣ�			�������ź���
**** �汾��Ϣ:   void Speech_algorithm(u8 i) ָ��������У����Ҫ������Σ�У���벻��ָ���������ܷ���
**** ˵  ����    ָ������У������һ��Ҫע�Ⱑ��ģ���ֲ�֪ʶ���赭д�ĸ�����û�к���ȷ��˵��������Ҳ���ҵĵ�ʦ�����Ҳ�֪���ģ�֮ǰһֱŪ������
****���˼·��   ��ǰ�ź�������Ȼ����������
********************************************************************************************************/
/*
�������ܣ�ָ���
����ֵ����
������u8 i
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
�������ܣ�ָ���
����ֵ����
������u8 i
*/
void Speech_algorithm(u8 i)
{
	u8 j;
	usart3_send_byte(0X7E);
	usart3_send_byte(0X05);
	usart3_send_byte(0x41);
	usart3_send_byte(0x00);
	usart3_send_byte(i);
	j=0x41^0X05^i;				//У�飬����ȥ��my1680�ֲᣬ����Ͳ�չ����
	usart3_send_byte(j);
	usart3_send_byte(0XEF);
}




/*
�������ܣ�����3��ʼ��
����ֵ����
������u32 bps
*/
void usart3_init(u32 bps)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO�ṹ�����
	USART_InitTypeDef USART_InitStruct;//��USART�ṹ�����
	NVIC_InitTypeDef NVIC_InitStruct;//����NVIC�ṹ�����

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);

	/* Configure USART3 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //?��
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //????��?��??�꨺?
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	USART_InitStruct.USART_BaudRate=bps;//���ò�����
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//����Ӳ����
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;//����Ϊ���պͷ���
	USART_InitStruct.USART_Parity=USART_Parity_No;//����żУ��
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//����ֹͣλ1
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//��������λ��8λ

	USART_Init(USART3, &USART_InitStruct);//��ʼ������

	//�ж�����
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//���������ж�ʹ��
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//���������ж�ʹ��
	//USART1->CR1 |= 1<<4;//���������ж�ʹ��
//	NVIC_InitStruct.NVIC_IRQChannel=USART3_IRQn;//ѡ��ͨ��
//	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;//ʹ��NVIC������
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;//����ǿ��ʽ���ȼ�
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;//������Ӧʽ���ȼ�
//	NVIC_Init(&NVIC_InitStruct);//��ʼ��NVIC�ṹ��
//	//NVIC_EnableIRQ(USART1_IRQn);//ѡ���ж�Դ

	USART_Cmd(USART3, ENABLE);//ʹ�ܴ���

}
/*
�������ܣ��жϷ�����
��������
����ֵ����
��ע���жϷ������޷���ֵ���޲���
*/
u8 buff3[100];
u8 flag3=0;
void USART3_IRQHandler(void)
{
	u8 data=0;
	static u8 cnt=0;//�����ж��ٸ��ֽ�
	if(USART_GetITStatus(USART3, USART_IT_RXNE)==SET)//�жϽ���״̬��־λ�ǲ�����1
	{
		USART_ClearFlag(USART3, USART_IT_RXNE);//���־λ
		//ʵ���ж�Ҫ�ɵ�����
		//data=USART1->DR;
		data=USART_ReceiveData(USART3);
		if(flag3)
		{
			return;
		}
		if(cnt<100)
		{
			buff3[cnt++]=data;//������һ���ֽڴ洢����������
		}
	}
	if(USART_GetITStatus(USART3, USART_IT_IDLE)==SET)//������״̬�ǲ��ǿ���
	{
		data=USART_ReceiveData(USART3);//����
		buff3[cnt]='\0';//��֤�ַ�������'\0'��β
		flag3=cnt;
		cnt=0;
	}
}
/*
�������ܣ�����һ���ַ�
����ֵ����
������u8 data
*/
void usart3_send_byte(u8 data)
{
	while(!USART_GetFlagStatus(USART3,USART_FLAG_TXE))
	{

	}
	USART_SendData(USART3, data);//��������
}
/*
�������ܣ�����һ���ַ�
����ֵ��u8
��������
*/
u8 usart3_recv_byte(void)
{
	while(!USART_GetFlagStatus(USART3,USART_FLAG_RXNE))
	{

	}
	return USART_ReceiveData(USART3);//�����յ����ݷ���
}
/*
�������ܣ������ַ���
����ֵ����
������u8 *data
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
�������ܣ������ַ���
����ֵ����
������u8 *data
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
////�������´���,֧��printf����,������Ҫѡ��use MicroLIB
//#if 1
//#pragma import(__use_no_semihosting)
////��׼����Ҫ��֧�ֺ���
//struct __FILE
//{
//	int handle;

//};

//FILE __stdout;
////����_sys_exit()�Ա���ʹ�ð�����ģʽ
//void _sys_exit(int x)
//{
//	x = x;
//}
////�ض���fputc����
//int fputc(int ch, FILE *f)
//{
//	  while((USART3->SR&0X40)==0);//ѭ������,ֱ���������
//    USART3->DR = (u8) ch;
//	  return ch;
//}
//#endif

int cloth;
u8 ff;
/*******************************************************************************************************
**** �������ƣ�
**** �����:      Mr'K    ��ʼ���ڣ�2022�� 1�� 23��
**** �޸���:      Mr'K    ĩֹ���ڣ�2022�� 1�� 23��
**** ��  �ܣ�			�㷨����
**** �汾��Ϣ:  	1.0
**** ˵  ����
**** ���˼·��		ͨ������������ݲ������ñ�־λ�ж��� ��ǧ���١�ʮ����λ�ȵ�,С�������Ǳ���c������ȷ��λС����
**** ע�����   С�������ﲻ���Ҹ���ʱ��printf,��Ȼ�ᵼ��������������
********************************************************************************************************/
void data_judge(u8 *buff)
{
 float a, c;							//a �������� , c ȡ���ݵ�С�����֣���ȷ����λ
    int i = 5, j, num_flag; // i���ֻ���㵽������i��5��   numflag һ�����յı�־λ
    int money[5]={0} 		//�ж����ܹ�����λ���� 
																//money[4]=5 ��λ    money[3]=4 ǧλ    money[2]=3 ��λ    money[1]=2 ʮλ    money[0]=1 ��λ    
				, sound[5]={0};		//λ���ϵ���ֵ��С����Χx��0~9�� 
																//sound[x]=5 ��λ    sound[x]=4 ǧλ    sound[x]=3 ��λ    sound[x]=2 ʮλ    sound[x]=1 ��λ
		
		

        a = strtod((char *)buff, NULL);       //�ַ�����ת����������
        c = (a - (int)a) * 100;                                     //ȡС��
//            printf("a=%.2f\r\n", a);
				Speech_algorithm(16);  //���ݣ���֧��
        while (i--)
        {
            if (((int)a) / ((int)pow(10, i)) % 10 > 0)   						 // (int)a)ǿ��ת���������ͣ�������￴�������ú���C���Ա���������
            {
                money[i] = i + 1;
                for (j = i; j >= 0; j--)
                {
                    sound[j] = ((int)a) / ((int)pow(10, j)) % 10;   // ((int)pow(10, j))���j=4  10000  ��λ��j- ��������  %10�������ȡλ������ֵ
                }																																					//j=3  1000   ǧλ
                break;																																		//j=2  100    ��λ
            }																																							//j=1  10   	ʮλ
        }																																									//j=0  1   		��λ
        num_flag = money[i]-1; 																			//�ж�λ�������︳ֵʱҪ-1����Ȼ�������ж�һ��λ��      		
				i==-1?num_flag=0:0;																					//if(i==-1)num_flag=0;  ��������Ŀ��������������ͬ��   
//            printf("money[%d]=%d\r\n\r\n", i, money[i]);					//											��ҿ���ѧһ����Ŀ�������õ���������
//            printf("num_flag=%d\r\n\r\n", num_flag);							//											��if�ṹ��ͬ

        if (num_flag >= 4) //��
        {
            printf("��sound[4]=%d\r\n", sound[4]);
            Speech_algorithm(sound[4]);
            delay_ms(6);
            Speech_algorithm(13);

        }
        if (num_flag >= 3) //ǧ
        {
            printf("ǧsound[3]=%d\r\n", sound[3]);
            Speech_algorithm(sound[3]);
            delay_ms(6);
            Speech_algorithm(12);
        }
        if (num_flag >= 2) //��
        {
            printf("��sound[2]=%d\r\n", sound[2]);
            Speech_algorithm(sound[2]);
            delay_ms(6);
            Speech_algorithm(11);
        }
        if (num_flag >= 1) //ʮ
        {
            printf("ʮsound[1]=%d\r\n", sound[1]);
            Speech_algorithm(sound[1]);
            delay_ms(6);
            Speech_algorithm(10);
        }
        if (num_flag >= 0) //������Ԫ
        {
            printf("��sound[0]=%d\r\n", sound[0]);
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
        i = 5;   																													//��װ��ֵ���Ա��´μ���

}






int page=1;
void data_judge_tem(u8 str1,u8 str2)
{

    float a, c;							//a �������� , c ȡ���ݵ�С�����֣���ȷ����λ
    int i = 5, j, num_flag; // i���ֻ���㵽������i��5��   numflag һ�����յı�־λ
    int money[5]={0} 		//�ж����ܹ�����λ����
												//money[4]=5 ��λ    money[3]=4 ǧλ    money[2]=3 ��λ    money[1]=2 ʮλ    money[0]=1 ��λ
				, sound[5]={0};		//λ���ϵ���ֵ��С����Χx��0~9��
																//sound[x]=5 ��λ    sound[x]=4 ǧλ    sound[x]=3 ��λ    sound[x]=2 ʮλ    sound[x]=1 ��λ

				cloth=(str1+str2)/2;
			printf("str1=%d-str2=%d\r\n",str1,str2);
			a=str1;
        while (i--)
        {
            if (((int)a) / ((int)pow(10, i)) % 10 > 0)   						 // (int)a)ǿ��ת���������ͣ�������￴�������ú���C���Ա���������
            {
                money[i] = i + 1;
                for (j = i; j >= 0; j--)
                {
                    sound[j] = ((int)a) / ((int)pow(10, j)) % 10;   // ((int)pow(10, j))���j=4  10000  ��λ��j- ��������  %10�������ȡλ������ֵ
                }																																					//j=3  1000   ǧλ
                break;																																		//j=2  100    ��λ
            }																																							//j=1  10   	ʮλ
        }																																									//j=0  1   		��λ
        num_flag = money[i]-1; 																			//�ж�λ�������︳ֵʱҪ-1����Ȼ�������ж�һ��λ��
				i==-1?num_flag=0:0;																					//if(i==-1)num_flag=0;  ��������Ŀ��������������ͬ��


				if(page==1)Speech_algorithm(32);
				else if(page==2)Speech_algorithm(33);
				else if(page==3)Speech_algorithm(34);
				else if(page==4)Speech_algorithm(35);
				else if(page==5)Speech_algorithm(36);
				else if(page==6)Speech_algorithm(37);
				else if(page==7)Speech_algorithm(38);


				Speech_algorithm(42);

        if (num_flag >= 1) //ʮ
        {
            printf("ʮsound[1]=%d\r\n", sound[1]);
            Speech_algorithm(sound[1]);
            delay_ms(6);
            Speech_algorithm(10);
        }
        if (num_flag >= 0) //������Ԫ
        {
            printf("��sound[0]=%d\r\n", sound[0]);
            Speech_algorithm(sound[0]);
            delay_ms(6);
            //Speech_algorithm(15);
        }

				i = 5;
				Speech_algorithm(15);

					a=str2;
        while (i--)
        {
            if (((int)a) / ((int)pow(10, i)) % 10 > 0)   						 // (int)a)ǿ��ת���������ͣ�������￴�������ú���C���Ա���������
            {
                money[i] = i + 1;
                for (j = i; j >= 0; j--)
                {
                    sound[j] = ((int)a) / ((int)pow(10, j)) % 10;   // ((int)pow(10, j))���j=4  10000  ��λ��j- ��������  %10�������ȡλ������ֵ
                }																																					//j=3  1000   ǧλ
                break;																																		//j=2  100    ��λ
            }																																							//j=1  10   	ʮλ
        }																																									//j=0  1   		��λ
        num_flag = money[i]-1; 																			//�ж�λ�������︳ֵʱҪ-1����Ȼ�������ж�һ��λ��
				i==-1?num_flag=0:0;																					//if(i==-1)num_flag=0;  ��������Ŀ��������������ͬ��


        if (num_flag >= 1) //ʮ
        {
            printf("ʮsound[1]=%d\r\n", sound[1]);
            Speech_algorithm(sound[1]);
            delay_ms(6);
            Speech_algorithm(10);
        }
        if (num_flag >= 0) //������Ԫ
        {
            printf("��sound[0]=%d\r\n", sound[0]);
            Speech_algorithm(sound[0]);
            delay_ms(6);
            //Speech_algorithm(15);
        }
        i = 5;
				Speech_algorithm(14);	//���϶�




//				ff=1;
//		if(ff==1)
//		{
//		cloth_remind();
//			ff=0;
//		}
}



void data_judge_hum(u8 str1,u8 str2)
{

    float a, c;							//a �������� , c ȡ���ݵ�С�����֣���ȷ����λ
    int i = 5, j, num_flag; // i���ֻ���㵽������i��5��   numflag һ�����յı�־λ
    int money[5]={0} 		//�ж����ܹ�����λ����
												//money[4]=5 ��λ    money[3]=4 ǧλ    money[2]=3 ��λ    money[1]=2 ʮλ    money[0]=1 ��λ
				, sound[5]={0};		//λ���ϵ���ֵ��С����Χx��0~9��
																//sound[x]=5 ��λ    sound[x]=4 ǧλ    sound[x]=3 ��λ    sound[x]=2 ʮλ    sound[x]=1 ��λ

				//cloth=(str1+str2)/2;
			printf("str1=%d-str2=%d\r\n",str1,str2);
			a=str1;
        while (i--)
        {
            if (((int)a) / ((int)pow(10, i)) % 10 > 0)   						 // (int)a)ǿ��ת���������ͣ�������￴�������ú���C���Ա���������
            {
                money[i] = i + 1;
                for (j = i; j >= 0; j--)
                {
                    sound[j] = ((int)a) / ((int)pow(10, j)) % 10;   // ((int)pow(10, j))���j=4  10000  ��λ��j- ��������  %10�������ȡλ������ֵ
                }																																					//j=3  1000   ǧλ
                break;																																		//j=2  100    ��λ
            }																																							//j=1  10   	ʮλ
        }																																									//j=0  1   		��λ
        num_flag = money[i]-1; 																			//�ж�λ�������︳ֵʱҪ-1����Ȼ�������ж�һ��λ��
				i==-1?num_flag=0:0;																					//if(i==-1)num_flag=0;  ��������Ŀ��������������ͬ��

//				if(page==0)Speech_algorithm(13);
//				else if(page==1)Speech_algorithm(32);
//				else if(page==2)Speech_algorithm(33);
//				else if(page==3)Speech_algorithm(34);
//				else if(page==4)Speech_algorithm(35);
//				else if(page==5)Speech_algorithm(36);
//				else if(page==6)Speech_algorithm(37);
//				else if(page==7)Speech_algorithm(38);


				Speech_algorithm(40);

        if (num_flag >= 1) //ʮ
        {
            printf("ʮsound[1]=%d\r\n", sound[1]);
            Speech_algorithm(sound[1]);
            delay_ms(6);
            Speech_algorithm(10);
        }
        if (num_flag >= 0) //������Ԫ
        {
            printf("��sound[0]=%d\r\n", sound[0]);
            Speech_algorithm(sound[0]);
            delay_ms(6);
            //Speech_algorithm(15);
        }

				i = 5;
				Speech_algorithm(15);

					a=str2;
        while (i--)
        {
            if (((int)a) / ((int)pow(10, i)) % 10 > 0)   						 // (int)a)ǿ��ת���������ͣ�������￴�������ú���C���Ա���������
            {
                money[i] = i + 1;
                for (j = i; j >= 0; j--)
                {
                    sound[j] = ((int)a) / ((int)pow(10, j)) % 10;   // ((int)pow(10, j))���j=4  10000  ��λ��j- ��������  %10�������ȡλ������ֵ
                }																																					//j=3  1000   ǧλ
                break;																																		//j=2  100    ��λ
            }																																							//j=1  10   	ʮλ
        }																																									//j=0  1   		��λ
        num_flag = money[i]-1; 																			//�ж�λ�������︳ֵʱҪ-1����Ȼ�������ж�һ��λ��
				i==-1?num_flag=0:0;																					//if(i==-1)num_flag=0;  ��������Ŀ��������������ͬ��


        if (num_flag >= 1) //ʮ
        {
            printf("ʮsound[1]=%d\r\n", sound[1]);
            Speech_algorithm(sound[1]);
            delay_ms(6);
            Speech_algorithm(10);
        }
        if (num_flag >= 0) //������Ԫ
        {
            printf("��sound[0]=%d\r\n", sound[0]);
            Speech_algorithm(sound[0]);
            delay_ms(6);
            //Speech_algorithm(15);
        }
        i = 5;
				Speech_algorithm(41);	//���϶�




//				ff=1;
//		if(ff==1)
//		{
//
//			ff=0;
//		}
}
