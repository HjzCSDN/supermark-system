#include "includes.h"//??????????????

int main(void)
{
    u8 t;
    u8 readbuff[10] = {0};
    char at_buf[10] = {0};
		
    u8 dat;
    u8 data;
    int add = 0;
    u32 cnt = 0;
		char *sssss="11.78";
    SCB->AIRCR = 0X05FA0500; //??????ж???2??????????0X05FA????д???????????????ú?????ж???
    //??????????????????ж??�?????????????????????????????????κ?λ??

    USART2_Init();
    delay_init();

    KEY_Init();
    Led_Init();

    Usart1_Init(115200);
    usart3_init(9600);
    printf("hello world\r\n");



    //OLED_ShowString(0,0,(u8*)"777",16);

//      AT24C02_I2C_Configuration();
//      at=AT24C04_Read_Page(0,0,5);
//      printf("????%s??\r\n",at);
//      add=atoi((char *)at)+1;
//      sprintf(at_buf,"%d",add);
//      AT24C04_Write_Page(0,0,(u8*)at_buf,5);
//      delay_ms(10);
//
    W25Q64_Init();
//      W25Q64_Erase_Chip();
    delay_ms(10);
//      W25Q64_Write((u8 *)"4444",0,10);
    delay_ms(10);
//      W25Q64_Read(readbuff,0,10);
    delay_ms(10);
    printf("read=%s\r\n", readbuff);
//
//
    OLED_Init();
    OLED_Clear();

    delay_ms(10);

//
//      while(1);
//printf("hello world111\r\n");

//while(1)
//{
//      data=USART_RecByte(USART1);
//      W25Q64_Write_NoCheck(&data,cnt++,1);
//}

//OLED_ShowString(0,0,"666",16);
//OLED_Refresh();

//Speech_algorithm(1);

    delay_ms(10);
//    OLED_ShowStr(40, 0, "44444");
//    OLED_ShowCH(0, 4, (u8*)"??");
//		OLED_ShowChineseHz(0,2,(u8*)"?????123");
    printf("????????\r\n");

    while (1)
    {
        OLED_ShowChineseHz(15,0,(u8*)"??????????");

        if (Read.USART1_FLAG&0x80)
        {
            u8 *readbuff=NULL;
            OLED_Clear();
            if(readbuff=strstr((char *)Read.USART1_BUFF,"voice")) {
                if(readbuff=strstr((char *)Read.USART1_BUFF,":")) {
                    u8 buff[100]= {0};
                    readbuff++;
										data_judge(readbuff);
                    sprintf((char *)buff,"%s ?",readbuff);
                    OLED_ShowChineseHz(0,2,buff);
                }
            }


            //Usart1_Send_string(Read.USART1_BUFF);
            if (!strcasecmp((char *)Read.USART1_BUFF, "1\r\n"))
            {
                //printf("??1\r\n");
                Usart2_Send_string((u8 *)"AT+LED=OFF\r\n");

                LED2=0;
                delay_ms(50);
                LED2=1;
            }
            else if (!strcasecmp((char *)Read.USART1_BUFF, "2\r\n"))
            {
                //printf("??2\r\n");
                Usart2_Send_string((u8 *)"AT+LED=ON\r\n");
                LED2=0;
                delay_ms(50);
                LED2=1;
            }
            else if (!strcasecmp((char *)Read.USART1_BUFF, "3\r\n"))
            {
                //printf("??3\r\n");
                //Usart2_Send_string((u8 *)"AT+TRIG=CMD\r\n");
                Usart2_Send_string((u8 *)"AT+TRIG=KEY\r\n");
            }
            else if (!strcasecmp((char *)Read.USART1_BUFF, "4\r\n"))
            {
                //printf("??4\r\n");
                //Usart2_Send_string((u8 *)"AT+TRIG=CMD\r\n");
                Usart2_Send_string((u8 *)"AT+TRIG=AUTO\r\n");
            }
            else if (!strcasecmp((char *)Read.USART1_BUFF, "5\r\n"))
            {
                //printf("??5\r\n");
                Usart2_Send_string((u8 *)"AT+TRIG=?\r\n");
            }
            else if (!strcasecmp((char *)Read.USART1_BUFF, "6\r\n"))
            {
                //printf("??6\r\n");
                Usart2_Send_string((u8 *)"AT+MODE=SINGLE\r\n");
            }
            else if (!strcasecmp((char *)Read.USART1_BUFF, "7\r\n"))
            {
                //printf("??7\r\n");
                Usart2_Send_string((u8 *)"AT+MODE=REPEAT\r\n");
            }
            else if (!strcasecmp((char *)Read.USART1_BUFF, "8\r\n"))
            {
                //printf("??8\r\n");
                Usart2_Send_string((u8 *)"AT+MODE=?\r\n");
            }
            memset(&Read, 0, sizeof(Read));

            LED1=0;
            delay_ms(50);
            LED1=1;

        }




        if (usart2_type.usart2_readFlag & 0x80)
        {

            if (!strcasecmp((char *)usart2_type.usart2_readBuf, "115201\r\n"))
            {
                printf("%s\r\n",usart2_type.usart2_readBuf);
            }

            if (!strcasecmp((char *)usart2_type.usart2_readBuf, "115202\r\n"))
            {
                printf("%s\r\n",usart2_type.usart2_readBuf);
            }
            if (!strcasecmp((char *)usart2_type.usart2_readBuf, "115203\r\n"))
            {
                printf("%s\r\n",usart2_type.usart2_readBuf);
            }

            if (!strcasecmp((char *)usart2_type.usart2_readBuf, "115204\r\n"))
            {
                printf("%s\r\n",usart2_type.usart2_readBuf);
            }
            if (!strcasecmp((char *)usart2_type.usart2_readBuf, "115205\r\n"))
            {
                printf("%s\r\n",usart2_type.usart2_readBuf);
            }
            memset(&usart2_type, 0, sizeof(usart2_type));
        }

        if(KEY1==0)
        {
            delay_ms(100);
            printf("??1\r\n");
            LED1 = 0;
            LED2 = 0;
            LED3 = 0;
            LED4 = 0;
        }

        if (KEY2 == 0)
        {
            delay_ms(100);
            printf("??2\r\n");
            LED1 = 1;
            LED2 = 1;
            LED3 = 1;
            LED4 = 1;
        }
        if (KEY3 == 0)
        {
            delay_ms(100);
            printf("??3\r\n");
        }

//				if(Read.USART1_FLAG&(1<<(8-1))){
//
//					Usart1_Send_string(Read.USART1_BUFF);
//					memset(&Read,0,sizeof(Read));
//				}

//                  OLED_ShowCH(0,4,"??");
//              delay_ms(1000);
//              OLED_Clear();
//              OLED_ShowCH(0,4,"??");
//              delay_ms(1000);
//              OLED_Clear();
//              printf("11111\r\n");
////                dat=Usart1_Read_byte();
////                W25Q64_Write_NoCheck(&dat,cnt++,1);
    }


    while (1)
    {



//          printf("hello world\r\n");


//          if(KEY1==0)
//          {
//              delay_ms(100);
//              printf("??1\r\n");
//                        LED1 = 0;
//            LED2 = 0;
//            LED3 = 0;
//            LED4 = 0;
//          }
//                      if(KEY2==0)
//          {
//              delay_ms(100);
//              printf("??2\r\n");
//                        LED1 = 1;
//            LED2 = 1;
//            LED3 = 1;
//            LED4 = 1;
//          }
//                      if(KEY3==0)
//          {
//              delay_ms(100);
//              printf("??3\r\n");
//          }
//                      if(KEY4==0)
//          {
//              delay_ms(100);
//              printf("??4\r\n");
//          }
//                      if(KEY5==0)
//          {
//              delay_ms(100);
//              printf("??5\r\n");
//          }
//                      if(KEY6==0)
//          {
//              delay_ms(100);
//              printf("??6\r\n");
//          }

//          printf("??1\r\n");
//        if (usart1_type.usart1_readFlag&0x80)
//        {
//            if (!strcasecmp((char *)usart1_type.usart1_readBuf, "1\r\n"))
//            {
//                printf("??1\r\n");
//                USART_SendString(USART2, (u8 *)"AT+LED=OFF\r\n");
//            }
//            else if (!strcasecmp((char *)usart1_type.usart1_readBuf, "2\r\n"))
//            {
//                printf("??2\r\n");
//                USART_SendString(USART2, (u8 *)"AT+LED=ON\r\n");
//            }
//            else if (!strcasecmp((char *)usart1_type.usart1_readBuf, "3\r\n"))
//            {
//                printf("??3\r\n");
//                USART_SendString(USART2, (u8 *)"AT+BEEP=OFF\r\n");
//            }
//            else if (!strcasecmp((char *)usart1_type.usart1_readBuf, "4\r\n"))
//            {
//                printf("??4\r\n");
//                USART_SendString(USART2, (u8 *)"AT+BEEP=ON\r\n");
//            }
//                   else if (!strcasecmp((char *)usart1_type.usart1_readBuf, "5\r\n"))
//            {
//                printf("??5\r\n");
//                USART_SendString(USART2, (u8 *)"AT+SCAN=STOP\r\n");
//            }
//                      else if (!strcasecmp((char *)usart1_type.usart1_readBuf, "6\r\n"))
//            {
//                printf("??6\r\n");
//                USART_SendString(USART2, (u8 *)"AT+MODE=REPEAT\r\n");
//            }
//                      else if (!strcasecmp((char *)usart1_type.usart1_readBuf, "7\r\n"))
//            {
//                printf("??7\r\n");
//                USART_SendString(USART2, (u8 *)"AT+TRIG=CMD\r\n");
//            }
//                      else if (!strcasecmp((char *)usart1_type.usart1_readBuf, "8\r\n"))
//            {
//                printf("??8\r\n");
//                USART_SendString(USART2, (u8 *)"AT+SCAN=START\r\n");
//            }
//            memset(&usart1_type, 0, sizeof(usart1_type));
//        }


//          if (usart2_type.usart2_readFlag & 0x80)
//          {
//                      if (!strcasecmp((char *)usart2_type.usart2_readBuf, "1\r\n"))
//                  {
//                          printf("??1111\r\n");
//                          USART_SendString(USART2, (u8 *)"AT+LED=OFF\r\n");
//                  }
//                  if (!strcasecmp((char *)usart2_type.usart2_readBuf, "2\r\n"))
//                  {
//                          printf("??2222\r\n");
//                          USART_SendString(USART2, (u8 *)"AT+LED=OFF\r\n");
//                  }
//                  if (!strcasecmp((char *)usart2_type.usart2_readBuf, "3\r\n"))
//                  {
//                          printf("??3333\r\n");
//                          USART_SendString(USART2, (u8 *)"AT+LED=OFF\r\n");
//                  }

//                  if (!strcasecmp((char *)usart2_type.usart2_readBuf, "4\r\n"))
//                  {
//                          printf("??4444\r\n");
//                          USART_SendString(USART2, (u8 *)"AT+LED=OFF\r\n");
//                  }
//                  memset(&usart2_type, 0, sizeof(usart2_type));
//                  usart2_type.usart2_readFlag=0;
//          }

    }
}