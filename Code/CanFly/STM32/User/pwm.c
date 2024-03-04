#include "pwm.h"
uint16_t PWM_MAX=1000;
void PWM_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	//PWM1                                                                     	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PWM2 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PWM3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PWM4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = PWM_ARR; 
	TIM_TimeBaseStructure.TIM_Prescaler = PWM_PSC; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);
}
void Motor_Pwm_Set(uint16_t MOTO1_PWM,uint16_t MOTO2_PWM,uint16_t MOTO3_PWM,uint16_t MOTO4_PWM)
{		
	if(MOTO1_PWM>PWM_MAX)	MOTO1_PWM = PWM_MAX;
	if(MOTO2_PWM>PWM_MAX)	MOTO2_PWM = PWM_MAX;
	if(MOTO3_PWM>PWM_MAX)	MOTO3_PWM = PWM_MAX;
	if(MOTO4_PWM>PWM_MAX)	MOTO4_PWM = PWM_MAX;
	if(MOTO1_PWM<0)	MOTO1_PWM = 0;
	if(MOTO2_PWM<0)	MOTO2_PWM = 0;
	if(MOTO3_PWM<0)	MOTO3_PWM = 0;
	if(MOTO4_PWM<0)	MOTO4_PWM = 0;
	
	TIM4->CCR1 = MOTO1_PWM;
	TIM4->CCR2 = MOTO2_PWM;
	TIM4->CCR3 = MOTO3_PWM;
	TIM4->CCR4 = MOTO4_PWM;
}
void Motor_Max_Set(uint16_t max)
{
	
	PWM_MAX=max;
}
