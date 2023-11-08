#include "bsp_usart.h"


u8 wifi_uart_buff[1024];


//配置串口2内核中断 抢占2，子优先级0
void uart2_nvic_irq_init(void)
{
    uint32_t priority;
    //配置内核中断分组
    NVIC_SetPriorityGrouping(5);
    //配置内核中断的优先级
    priority = NVIC_EncodePriority(5,2,0);
    NVIC_SetPriority(USART2_IRQn,priority);
    //使能内核中断
    NVIC_EnableIRQ(USART2_IRQn);
}

void uart2_irq_init(void)
{
    //USART2->CR1 |= (1<<5);//开启接收完成中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
}

void uart2_init(u32 bound)
{
//    /* 配置IO口 */
//    RCC->APB2ENR |= (1<<2);
//    GPIOA->CRL &= 0XFFFF00FF;
//    GPIOA->CRL |= 0X00008B00;
//    GPIOA->ODR |= (1<<3);//PA3上拉
//
//    RCC->APB1ENR |= (1<<17);//开启串口1时钟
//
//    USART2->BRR=(36*1000000)/(bound);// 波特率设置
//		USART2->CR1|=0X200C;  	//1位停止,无校验位.
//    /* 配置内核中断 */
//    uart2_nvic_irq_init();
//    /* 配置外设中断 */
//    uart2_irq_init();
//    /* 使能串口 */
//    USART2->CR1 |= (1<<3);//发送使能
//    USART2->CR1 |= (1<<2);//接收使能
//    USART2->CR1 |= (1<<13);//总使能

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    //USART_DeInit(USART2);  //复位串口2
    //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2

    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3
    GPIO_SetBits(GPIOA, GPIO_Pin_3);

    USART_InitStructure.USART_BaudRate = bound;//;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口	2

    /* 配置内核中断 */
    //uart2_nvic_irq_init();
    /* 配置外设中断 */
    //uart2_irq_init();

    USART_Cmd(USART2,ENABLE);								/*使能串口2*/
}

void uart2_send_data(u8 data)
{
    //等待数据发送完成
    while(!(USART2->SR&(1<<6)));
    USART2->DR = data;
}
//发送字符串函数
void usart2_sendstring(char *str)
{
    while(*str != '\0')
    {
        uart2_send_data(*str);
        str++;
    }
    uart2_send_data('\r');
    uart2_send_data('\n');
}

u16 usart2_res_len=0;
void USART2_IRQHandler(void)
{
//    u32 sr;
//    sr = USART2->SR;
//    if(sr&(1<<5))//接收完成中断
    u8 res;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
    {
        if(usart2_res_len==0) //收到第一个字符
        {
            //time3_start(); //开启定时器7
        }
        else //收到第二个及之后的字符
        {
            //time3_reset(); //复位定时器7的CNT
        }
        res = USART_ReceiveData(USART2);
        wifi_uart_buff[usart2_res_len++] = res;//存放收到的数据。
    }
}


//实现fputc函数
void wifi_fputc(char str)
{
    uart2_send_data(str);
}

//实现printf函数
void wifi_printf(char *str,...)
{
    int int_d;
    double float_d;
    const char *string_s;
    char buff[20];
    int i;

    va_list ap;//定义变参的起始指针
    va_start(ap, str);//起始指针指向str

    while(*str!='\0')
    {
        if(*str=='\\')
        {
            str++;
            switch(*str)
            {
            case 'r':
                wifi_fputc('\r');
                str++;
                break;
            case 'n':
                wifi_fputc('\n');
                str++;
                break;
            default:
                str++;
                break;
            }
        }
        else if(*str=='%')
        {
            str++;
            switch(*str)
            {
            case 'd':
                int_d = va_arg(ap, int);
                sprintf(buff,"%d",int_d);
                for(i=0; i<(strlen(buff)); i++)
                    wifi_fputc(buff[i]);
                str++;
                break;
            case 'f':
                float_d = va_arg(ap, double);
                sprintf(buff,"%f",float_d);
                for(i=0; i<(strlen(buff)); i++)
                    wifi_fputc(buff[i]);
                str++;
                break;
            case 's':
                string_s = va_arg(ap, const char *);
                while(*string_s)
                {
                    wifi_fputc(*string_s++);
                }
                str++;
                break;
            default:
                str++;
                break;
            }
        }
        else
        {
            wifi_fputc(*str++);
        }
    }
}

