#include "includes.h"
CanFly canfly;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	memset(&canfly,0,sizeof(CanFly));
	USART1_Config(921600);
	USART2_Config(921600);
	PID_Parm_Config();
	DELAY_Config();
	IO_Config();
	LED_Config();
	ADC_Config();
	PWM_Config();		
	WS2812B_Config(WS2812B_ARR);
	WS2812B_Red(4);
	IIC_Config();
	MPU_Config();
	TIMER_Config();
	while(1){		
		if(IS_10HZ()){
			CanFly_Bat();
			WS2812B_Blink();
		}
		if(IS_2HZ()){
			LED_Blink();		
		}
		
	}
}
