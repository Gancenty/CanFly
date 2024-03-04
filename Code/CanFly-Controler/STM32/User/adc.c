#include "adc.h"
volatile uint16_t ADC_Value[5]={0};
void ADC_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);	
 
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 5;
	ADC_Init(ADC1, &ADC_InitStructure);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 4, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 5, ADC_SampleTime_239Cycles5);
	
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
 
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET);
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)&(ADC1->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 5; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular  ;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High ;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	delay_ms(5);
	for(uint8_t i=0;i<4;i++){
		canfly_ctr.adc_normal[i]=ADC_Value[i+1];
	}
}

unsigned short ADC_get(uint8_t ch){
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);//配置ADC1通道11为55.5个采样周期 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);
}
unsigned short ADC_get_ave(uint8_t ch){
	uint8_t i=0,times=3;
	uint8_t temp=0;
	for(i=0;i<times;i++){
		temp+=ADC_get(ch);
	}
	return temp/times;
}

void CanFly_Bat(void)
{
	static uint8_t cnt0=0;
	static uint8_t index=0;
	static uint16_t adc_temp[6];
	adc_temp[index%5]=ADC_Value[0];
	index++;
	adc_temp[5]=0;
	for(uint8_t i=0;i<5;i++){
		adc_temp[5]+=adc_temp[i];
	}
	adc_temp[5]/=5;
	canfly_ctr.bat_vol=adc_temp[5]*2*3.3f/4096.0f;
	if(canfly_ctr.bat_vol<STAY_BAT){
		cnt0++;
		if(cnt0>10){
			canfly_ctr.status&=~POWER_MARK;
		}
	}else{
		cnt0=0;
		canfly_ctr.status|=FULL_POWER;
	}
}
void CanFly_Move(void)
{
	canfly_ctr.llr=((float)ADC_Value[1]-canfly_ctr.adc_normal[0])/2048*20;
	canfly_ctr.ltd=(-(float)ADC_Value[2]+canfly_ctr.adc_normal[1])/2048*20;
	canfly_ctr.rlr=((float)ADC_Value[3]-canfly_ctr.adc_normal[2])/2048*20;
	canfly_ctr.rtd=((float)ADC_Value[4]-canfly_ctr.adc_normal[3])/2048*20;
		
	if((canfly_ctr.status&CONNECT_MARK)==NO_CONNECT){
		canfly_ctr.sflag=0;
		canfly_ctr.c_thr=0;
		canfly_ctr.c_rol=0;
		canfly_ctr.c_pit=0;
		return;
	}
	canfly_ctr.c_rol=canfly_ctr.rlr;
	canfly_ctr.c_pit=canfly_ctr.rtd;
	
	if(canfly_ctr.llr<-15){
		if(canfly_ctr.sflag==1){
			canfly_ctr.beep_time=10;
		}
		canfly_ctr.sflag=0;
	}
	if(canfly_ctr.llr>15){
		if(canfly_ctr.sflag==0){
			canfly_ctr.beep_time=10;
		}
		canfly_ctr.sflag=1;
	}
	
	if(canfly_ctr.sflag==0){
		canfly_ctr.status&=~FLY_MARK;
	}else{
		canfly_ctr.status|=IN_FLY_MODE;
	}
	
	if(canfly_ctr.sflag==1){
		if(canfly_ctr.llr>-10&&canfly_ctr.llr<10){
			canfly_ctr.c_thr+=canfly_ctr.ltd;
			if(canfly_ctr.c_thr>700){
				canfly_ctr.c_thr=700;
			}
			if(canfly_ctr.c_thr<0){
				canfly_ctr.c_thr=0;
			}			
		}	
	}else{
		canfly_ctr.c_thr=0;
		if(canfly_ctr.ltd>0){
			if(canfly_ctr.bright<255){
				canfly_ctr.bright++;
			}
		}
		if(canfly_ctr.ltd<0){
			if(canfly_ctr.bright>0){
				canfly_ctr.bright--;
			}
		}
	}
}
