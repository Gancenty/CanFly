#include "led.h"

uint16_t LED_Buffer[DATA_SIZE*LED_NUM+1];

void WS2812B_Config(uint16_t arr)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); 

	GPIO_InitStructure.GPIO_Pin = WS2812B_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             
	GPIO_Init(WS2812B_PORT,&GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler=0;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=arr;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, ENABLE);		
	TIM_DMACmd(TIM3, TIM_DMA_CC4, ENABLE);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM3->CCR4);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)LED_Buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;											
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel3, DISABLE);
	canfly_ctr.bright=50;
}
void WS2812B_Reset(void)
{
	delay_us(50);	//此处延时时间最小值大于50us即可
}
uint16_t WS2812B_Write_24Bits(uint16_t num,uint32_t GRB_Data)
{
  uint16_t i=0;
	uint8_t color[3];
	color[0]=(GRB_Data>>16)&0XFF;
	color[1]=(GRB_Data>>8)&0XFF;
	color[2]=(GRB_Data>>0)&0XFF;
	for(i=0;i<3;i++){
		color[i]=(float)color[i]*((float)1/40);
	}
	GRB_Data=color[0]<<16|color[1]<<8|color[2];
	
	for(i = 0; i < DATA_SIZE; i++)
	{
		LED_Buffer[num + i] = ((GRB_Data << i) & 0x800000) ? T1H : T0H;
	}
	return num+DATA_SIZE;
}
//点亮灯珠
void WS2812B_Show(uint16_t num)
{
	uint8_t need_reset=0;
	while(DMA_GetCurrDataCounter(DMA1_Channel3)!=0){
		need_reset=1;
	}
	if(need_reset){
		WS2812B_Reset();
	}
	LED_Buffer[num*DATA_SIZE]=0;
	DMA_Cmd(DMA1_Channel3, DISABLE);
	TIM_Cmd(TIM3,DISABLE);
	TIM_SetCompare4(TIM3,0);
	TIM_SetCounter(TIM3,0);
	DMA_SetCurrDataCounter(DMA1_Channel3, num*DATA_SIZE+1);
	TIM_Cmd(TIM3,ENABLE);
	DMA_Cmd(DMA1_Channel3, ENABLE);
}

void WS2812B_Red(uint16_t num)
{
	uint16_t next=0;
	for(uint16_t i=0;i<num;i++){
		next=WS2812B_Write_24Bits(next,0x00ff00);
	}
	WS2812B_Show(num);
}
void WS2812B_Green(uint16_t num)
{
	uint16_t next=0;
	for(uint16_t i=0;i<num;i++){
		next=WS2812B_Write_24Bits(next,0xff0000);
	}
	
	WS2812B_Show(num);
}
void WS2812B_Blue(uint16_t num)
{
	uint16_t next=0;
	for(uint16_t i=0;i<num;i++){
		next=WS2812B_Write_24Bits(next,0x0000ff);
	}
	WS2812B_Show(num);
}
void WS2812B_White(uint16_t num)
{
	uint16_t next=0;
	for(uint16_t i=0;i<num;i++){
		next=WS2812B_Write_24Bits(next,0xffffff);
	}
	WS2812B_Show(num);
}
void WS2812B_Dark(uint16_t num)
{
	uint16_t next=0;
	for(uint16_t i=0;i<num;i++){
		next=WS2812B_Write_24Bits(next,0x000000);
	}
	WS2812B_Show(num);
}
void WS2812B_Yellow(uint16_t num)
{
	uint16_t next=0;
	for(uint16_t i=0;i<num;i++){
		next=WS2812B_Write_24Bits(next,0Xb1ff00);
	}
	
	WS2812B_Show(num);
}
void WS2812B_Rand(uint16_t num)
{
	uint32_t color;
	uint8_t green,red,blue;
	green=rand()%256;
	red=rand()%256;
	blue=rand()%256;
	color=green<<16|red<<8|blue;
	
	uint16_t next=0;
	for(uint16_t i=0;i<num;i++){
		next=WS2812B_Write_24Bits(next,color);
	}
	
	WS2812B_Show(num);
}
void WS2812B_Color(uint32_t color[],uint16_t num)
{
	uint16_t next=0;
	for(uint16_t i=0;i<num;i++){
		next=WS2812B_Write_24Bits(next,color[i]);
	}
	WS2812B_Show(num);
}
void WS2812B_Blink(void)
{
	uint32_t color[][1]={0x8bf300,\
											 0x5e89f9,\
											 0xd5186b,\
											 0x7945d4,\
											};
	if(canfly_ctr.tim_ms-canfly_ctr.last_rec>200){
		canfly_ctr.status&=~CONNECT_MARK;
	}else{
		canfly_ctr.status|=CONNECTED;
	}
	static uint8_t change=0;
	switch(canfly_ctr.status&POWER_MARK){
		case FULL_POWER:		
			switch(canfly_ctr.status&CONNECT_MARK){			
				case CONNECTED:				
					switch(canfly_ctr.status&FLY_MARK){
						case IN_FLY_MODE:
							if(change){
								WS2812B_Blue(LED_NUM);
							}else{
								WS2812B_Dark(LED_NUM);
							}
							change=!change;
							break;
						case STAY_MODE:
							if(change){
								WS2812B_Green(LED_NUM);
							}else{
								WS2812B_Dark(LED_NUM);
							}
							change=!change;
							break;
						default:break;
					}
					break;				
				case NO_CONNECT:			
					if(change){
						WS2812B_Red(LED_NUM);
					}else{
						WS2812B_Dark(LED_NUM);
					}
					change=!change;
					break;
				
				default:break;
			}
			break;		
		case LOW_POWER:
			if(change){
				WS2812B_Yellow(LED_NUM);
			}else{
				WS2812B_Dark(LED_NUM);
			}
			change=!change;
			break;
		default:break;		
	}
}
void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}
void LED_Blink(void)
{
	static uint8_t flag=0;
	if(flag){
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}
	flag=!flag;
}
