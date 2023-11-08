#ifndef _OLED_H
#define _OLED_H
#include "stm32f10x.h"

//������Ļ��С
#define OLED_WIGH 128
#define OLED_PAGE 8

#define OLED_CS(x)	(x)?(GPIOB->ODR |= (1<<7)):(GPIOB->ODR &= ~ (1<<7))//ƬѡCS  PB7��ͨ���
#define OLED_RES(x)	 (x)?(GPIOB->ODR |= (1<<6)):(GPIOB->ODR &= ~ (1<<6))//��λRES  PA4��ͨ���
#define OLED_DC(x)	(x)?(GPIOA->ODR |= (1<<15)):(GPIOA->ODR &= ~ (1<<15))//���ݿ���ѡ��DC  PB14��ͨ���

typedef enum
{
	OLED_CMD=0,	//д����
	OLED_DATA,	//д����
}OLED_MODE;

extern const unsigned char ascii_8_16[94][16];
extern const unsigned char ziku[][96]; 
void Delay_ms(u32 t);
void OLED_Init (void);
u8 OLED_WR_Byte(uint8_t dat,OLED_MODE cmd);
void OLED_Clear(void);
void OLED_ShowPhoto(uint16_t x, uint16_t y, uint16_t width, uint16_t high, const unsigned char * photo);
void OLED_ShowChar(uint16_t x, uint16_t y, char c);
void OLED_ShowStr(uint16_t x, uint16_t y, char * str);
void OLED_Showchinaese(u8 x,u8 y,u8 no);
void OLED_ShowCH (uint16_t x, uint16_t y,u8 *ch);
void Draw_Point(void);
void OLED_ShowChineseHz(uint16_t x, uint16_t y,u8 *ch);
#endif
