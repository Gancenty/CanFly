#include "iic.h"

static uint8_t IIC_DELAY_US=1;
void IIC_Config(void)
{
	static uint8_t isInit=0;
	if(isInit==1)return;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;//SDA
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(IIC_SDA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=IIC_SCL_PIN;//SCL
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(IIC_SCL,&GPIO_InitStructure);
	IIC_SCL_H;
	IIC_SDA_H;
	IIC_DELAY_US=2;
	isInit=1;
}
//void IIC_SDA_OUT(void)//SDA是输出方向
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_Init(IIC_SDA,&GPIO_InitStructure);
//}
//void IIC_SDA_IN(void)//SDA是输入方向
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
//	GPIO_Init(IIC_SDA,&GPIO_InitStructure);
//}
void IIC_DELAY(void)
{
	uint32_t cnt=2;
	while(cnt--);
}
void IIC_HALF_DELAY(void)
{
	uint32_t cnt=1;
	while(cnt--);
}
void IIC_Set_Delay_Us(uint8_t us)
{
	IIC_DELAY_US=us;
}
void IIC_START()//IIC_SCL高电平时候 IIC_SDA拉低
{
	IIC_SDA_OUT();
	IIC_SCL_L;IIC_HALF_DELAY();IIC_SDA_H;IIC_HALF_DELAY();

	IIC_SCL_H;IIC_HALF_DELAY();IIC_SDA_L;IIC_HALF_DELAY();
	IIC_SCL_L;	
}
void IIC_STOP()//IIC_SCL高电平时候 IIC_SDA升高
{
	IIC_SDA_OUT();
	IIC_SCL_L;IIC_HALF_DELAY();IIC_SDA_L;IIC_HALF_DELAY();
	
	IIC_SCL_H;IIC_HALF_DELAY();IIC_SDA_H;IIC_HALF_DELAY();
	IIC_SCL_L;
}
void IIC_ACK()//SCL为高电平时候 IIC_SDA为低电平
{
	IIC_SDA_OUT();
	IIC_SCL_L;IIC_HALF_DELAY();IIC_SDA_L;IIC_HALF_DELAY();

	IIC_SCL_H;IIC_DELAY();
	IIC_SCL_L;
}
void IIC_NACK()
{
	IIC_SDA_OUT();
	IIC_SCL_L;IIC_HALF_DELAY();IIC_SDA_H;IIC_HALF_DELAY();

	IIC_SCL_H;IIC_DELAY();
	IIC_SCL_L;
}
uint8_t IIC_WAIT_ACK(void)
{
	uint8_t rpt=0;
	IIC_SDA_IN();
	IIC_SCL_L;IIC_HALF_DELAY();IIC_SDA_H;IIC_HALF_DELAY();

	IIC_SCL_H;IIC_HALF_DELAY();
	while(SDA_READ==Bit_SET)   //等待应答信号
	{
		rpt++;
		if(rpt>200){
			IIC_STOP();
			return 1;
		}
	}
	IIC_HALF_DELAY();
	IIC_SCL_L;
	return 0;
}
void IIC_SEND_BYTE(uint8_t byte)
{
	IIC_SDA_OUT();
	uint8_t i=0;
	for(i=0;i<8;i++){
		uint8_t bit=(byte>>(7-i)&0x01);
		IIC_SCL_L;IIC_HALF_DELAY();
		if(bit){
			IIC_SDA_H;
		}else{
			IIC_SDA_L;
		}
		IIC_HALF_DELAY();		
		IIC_SCL_H;IIC_DELAY();
	}
	IIC_SCL_L;
}
uint8_t IIC_RECEIVE_BYTE(uint8_t ack)
{
	uint8_t temp,i;
	IIC_SDA_IN();
	for(i=0;i<8;i++)
	{
		IIC_SCL_L;IIC_DELAY();
		
		IIC_SCL_H;IIC_HALF_DELAY();
		temp<<=1;
		if(SDA_READ==Bit_SET)temp|=0x01;
		IIC_HALF_DELAY();
	}
	IIC_SCL_L;	
	if(!ack)
		 IIC_NACK();        //发送NACK
	else
		 IIC_ACK();         //发送ACK   
	return temp;
}
uint8_t IIC_READ_BYTE(uint8_t addr,uint8_t reg)
{
	uint8_t ret=0;
	IIC_START(); 
	IIC_SEND_BYTE((addr<<1)|0);
	if(IIC_WAIT_ACK()==1){
		return 1;
	}
	
  IIC_SEND_BYTE(reg);
  if(IIC_WAIT_ACK()==1){
		return 1;
	}
	
  IIC_START();
	IIC_SEND_BYTE((addr<<1)|1);
  if(IIC_WAIT_ACK()==1){
		return 1;
	}
	ret=IIC_RECEIVE_BYTE(0);
  IIC_STOP();
	return ret;
	
}
uint8_t IIC_WRITE_BYTE(uint8_t addr,uint8_t reg,uint8_t data)
{
	uint8_t ret=0;
	IIC_START(); 
	IIC_SEND_BYTE((addr<<1)|0);
	if(IIC_WAIT_ACK()==1){
		return 1;
	}
	
	IIC_SEND_BYTE(reg);         //写寄存器地址
  if(IIC_WAIT_ACK()==1){
		return 1;
	}
  IIC_SEND_BYTE(data);
  if(IIC_WAIT_ACK()==1){
		return 1;
	}
  IIC_STOP();
	return 0;
}
//R/W#   1/0
uint8_t IIC_READ(uint8_t addr,uint8_t reg,uint8_t len,uint8_t* data)
{
	IIC_START();
	IIC_SEND_BYTE((addr<<1)|0);
	if(IIC_WAIT_ACK()==1){
		return 1;
	}
	
	IIC_SEND_BYTE(reg);
	if(IIC_WAIT_ACK()==1){
		return 1;
	}
	
	IIC_START();
	IIC_SEND_BYTE((addr<<1)|1);
	if(IIC_WAIT_ACK()==1){
		return 1;
	}
	while(len){
		if(len==1)*data=IIC_RECEIVE_BYTE(0);
		else *data=IIC_RECEIVE_BYTE(1);
		len--;
		data++;	
	}
	IIC_STOP();
	return 0;
}
uint8_t IIC_WRITE(uint8_t addr,uint8_t reg,uint8_t len,uint8_t* data)
{
	IIC_START();
	IIC_SEND_BYTE((addr<<1)|0);
	if(IIC_WAIT_ACK()==1){
		return 1;
	}
	IIC_SEND_BYTE(reg);
	if(IIC_WAIT_ACK()==1){
		return 1;
	}
	
	while(len){
		IIC_SEND_BYTE(*data);
		IIC_WAIT_ACK();	
		len--;
		data++;
	}
	IIC_STOP();
	return 0;
}

