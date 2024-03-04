#include "timer.h"
void TIMER_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM2);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=49;
	TIM_TimeBaseInitStruct.TIM_Prescaler=7199;
	
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	canfly.tim_ms=0;
}
void TIM2_IRQHandler(void){             
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)//溢出中断 
	{		
		canfly.tim_ms+=5;
		if(IS_200HZ()){//是否5ms时间到
			CanFly_Calcu_Position();//计算四轴飞行器的位置
  		CanFly_Control();	//进行PID控制
			CanFly_Upload();	//将飞控数据通过串口发送到ESP8266			
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}	

