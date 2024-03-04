#ifndef _ADC_H
#define _ADC_H
#include "config.h"
#include "stm32f10x.h"
#include "delay.h"

void ADC_Config(void);
void CanFly_Bat(void);
void CanFly_Move(void);
unsigned short ADC_get(uint8_t ch);
unsigned short ADC_get_ave(uint8_t ch);
#endif
