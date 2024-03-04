#include "telecom.h"

#define BYTE0(dwTemp) (*(char *)(&dwTemp))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))
	
void RC_Send_Info(void)
{
	if(DMA_GetCurrDataCounter(DMA1_Channel7)){
		return;
	}
	uint16_t len=0;
	canfly.txlen=0;
	
	
	DMA_Cmd(DMA1_Channel7,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel7, canfly.txlen);
	DMA_Cmd(DMA1_Channel7,ENABLE);
}
void RC_Rec_Info(void);


