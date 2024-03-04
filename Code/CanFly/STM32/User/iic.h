#ifndef _IIC_H
#define _IIC_H
#include "stm32f10x.h"
#include "gpio.h"
#define IIC_SDA  GPIOB
#define IIC_SCL  GPIOB

#define IIC_SDA_PIN GPIO_Pin_13
#define IIC_SCL_PIN GPIO_Pin_12

#define IIC_SDA_H 	PBout(13)=1
#define IIC_SDA_L 	PBout(13)=0

#define IIC_SCL_H 	PBout(12)=1
#define IIC_SCL_L 	PBout(12)=0

#define SDA_READ 	PBin(13)
#define SCL_READ 	PBin(12)


#define IIC_SDA_OUT()	 {GPIOB->CRH&=~(0X0F<<20);GPIOB->CRH|=(0X03<<20);}
#define IIC_SDA_IN()	 {GPIOB->CRH&=~(0X0F<<20);GPIOB->CRH|=(0X08<<20);}

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
