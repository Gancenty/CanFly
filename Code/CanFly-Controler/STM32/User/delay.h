#ifndef _DELAY_H
#define _DELAY_H
#include "stm32f10x.h"
void DELAY_Config(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif
