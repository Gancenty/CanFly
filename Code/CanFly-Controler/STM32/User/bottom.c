#include "bottom.h"

void BOTTOM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
}
void BOTTOM_Scan(void)
{
	static uint32_t High_Cnt[5]={0};
	if(PBin(6)==1){
		High_Cnt[0]++;
		if(High_Cnt[0]>25){
		}
	}else{
		if(High_Cnt[0]>3&&High_Cnt[0]<25){
			canfly_ctr.ui_mode=(canfly_ctr.ui_mode+1)%MAX_UI_CNT;
		}
		High_Cnt[0]=0;
	}
	if(PBin(7)==1){
		High_Cnt[1]++;
		if(High_Cnt[1]>25){
		}
	}else{
		if(High_Cnt[1]>3&&High_Cnt[1]<25){
		}
		High_Cnt[1]=0;
	}
}
