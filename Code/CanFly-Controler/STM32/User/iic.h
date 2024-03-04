#ifndef _IIC_H
#define _IIC_H
#include "stm32f10x.h"
#include "gpio.h"
#define IIC_SDA  GPIOA
#define IIC_SCL  GPIOA

#define IIC_SDA_PIN GPIO_Pin_12
#define IIC_SCL_PIN GPIO_Pin_11

#define IIC_SDA_H 	PAout(12)=1
#define IIC_SDA_L 	PAout(12)=0

#define IIC_SCL_H 	PAout(11)=1
#define IIC_SCL_L 	PAout(11)=0

#define SDA_READ 	PAin(12)
#define SCL_READ 	PAin(11)

#define IIC_SDA_OUT()	 {GPIOA->CRH&=~(0X0F<<16);GPIOA->CRH|=(0X03<<16);}
#define IIC_SDA_IN()	 {GPIOA->CRH&=~(0X0F<<16);GPIOA->CRH|=(0X08<<16);}

void IIC_Config(void);
void IIC_Set_Delay_Us(uint8_t us);
void IIC_START(void);
void IIC_STOP(void);
void IIC_ACK(void);
void IIC_NACK(void);
unsigned char IIC_WAIT_ACK(void);
void IIC_SEND_BYTE(unsigned char byte);
unsigned char IIC_RECEIVE_BYTE(unsigned char ack);

uint8_t IIC_READ_BYTE(uint8_t addr,uint8_t reg);
uint8_t IIC_WRITE_BYTE(uint8_t addr,uint8_t reg,uint8_t data);
uint8_t IIC_READ(uint8_t addr,uint8_t reg,uint8_t len,uint8_t* data);
uint8_t IIC_WRITE(uint8_t addr,uint8_t reg,uint8_t len,uint8_t* data);
#endif
