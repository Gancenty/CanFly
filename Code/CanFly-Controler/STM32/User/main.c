#include "includes.h"
CANFLY_CONTROLER canfly_ctr;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART1_Config(921600);
	USART2_Config(921600);
	DELAY_Config();
	IO_Config();
	WS2812B_Config(WS2812B_ARR);
	LED_Config();
	BEEP_Config();
	ADC_Config();
	IIC_Config();
	OLED_Config();
	TIMER_Config();
	while(1){
		
		if(IS_10HZ()){
			WS2812B_Blink();
			UI_Refresh();
		}
		if(IS_2HZ()){
			LED_Blink();
		}
	}
}
