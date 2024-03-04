#ifndef _LED_H
#define _LED_H
#include "stm32f10x.h"
#include "config.h"
#include "delay.h"
#include "stdio.h"
#include "stdlib.h"

#define WS2812B_PIN    GPIO_Pin_1
#define WS2812B_PORT   GPIOB
#define WS2812B_HIGH   GPIO_SetBits(WS2812B_PORT,WS2812B_PIN)
#define WS2812B_LOW	   GPIO_ResetBits(WS2812B_PORT,WS2812B_PIN)
/*	����DataSheet����0/1�����Ӧ�ĸߵ�ƽ�Ƚ�ֵ����Ϊ�ߵ�ƽ��ǰ������ߵ�ƽ�ļ���
**	�ߵ͵�ƽ�ı���ԼΪ2:1
*/
#define WS2812B_ARR 90-1
#define T0H 30-1
#define T1H 60-1

/* �������ĸ��� */
#define LED_NUM  		1
#define DATA_SIZE  24

void WS2812B_Config(uint16_t arr);
void WS2812B_Reset(void);
uint16_t WS2812B_Write_24Bits(uint16_t num,uint32_t GRB_Data);
void WS2812B_Show(uint16_t num);
void WS2812B_Red(uint16_t num);
void WS2812B_Green(uint16_t num);
void WS2812B_Blue(uint16_t num);
void WS2812B_White(uint16_t num);
void WS2812B_Dark(uint16_t num);
void WS2812B_Blink(void);

void LED_Config(void);
void LED_Blink(void);

#endif

