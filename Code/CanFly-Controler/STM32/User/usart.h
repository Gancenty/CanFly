#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
#include "config.h"
#include "protocol.h"
#include "stdio.h"
void USART1_Config(uint32_t bound);
void USART2_Config(uint32_t bound);
void USART1_SendStr(char* str);
void USART2_SendStr(char* str);
void USART1_SendHex(char *data,uint16_t len);
void USART2_SendHex(char *data,uint16_t len);


#endif


