#ifndef _ADC_H
#define _ADC_H
#include "stm32f10x.h"
#include "config.h"
void ADC_Config(void);
void CanFly_Bat(void);
unsigned short ADC_get(uint8_t ch);
unsigned short ADC_get_ave(uint8_t ch);
#endif
