#include "protocol.h"

#define BYTE0(dwTemp) (*(char *)(&dwTemp))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))

void ano_data_make_head(uint8_t *data, uint16_t *len, uint16_t max_len)
{
    *len = 0;
    data[HEAD_LOC] = ANO_HEAD;
    data[ADDR_LOC] = ANO_ADDR;
}
char ano_data_check(uint8_t *data, uint16_t len)
{
    uint8_t sumcheck = 0, addcheck = 0;
    for (short i = 0; i < len - 2; i++)
    {
        sumcheck += data[i];
        addcheck += sumcheck;
    }
    if (sumcheck == data[len - 2] && addcheck == data[len - 1])
        return 1;
    else
        return 0;
}
void ano_data_check_fill(uint8_t *data, uint16_t len)
{
    uint8_t sumcheck = 0, addcheck = 0;
    for (short i = 0; i < len - 2; i++)
    {
        sumcheck += data[i];
        addcheck += sumcheck;
    }
    data[len - 2] = sumcheck;
    data[len - 1] = addcheck;
}
uint8_t ano_data_fill_u8bit(uint8_t *data, uint8_t _d, uint8_t index)
{
    data[index] = BYTE0(_d);
    return index + 1;
}
uint8_t ano_data_fill_u16bit(uint8_t *data, uint16_t _d, uint8_t index)
{
    data[index] = BYTE0(_d);
    data[index + 1] = BYTE1(_d);
    return index + 2;
}
uint8_t ano_data_fill_u32bit(uint8_t *data, uint32_t _d, uint8_t index)
{
    data[index] = BYTE0(_d);
    data[index + 1] = BYTE1(_d);
    data[index + 2] = BYTE2(_d);
    data[index + 3] = BYTE3(_d);
    return index + 4;
}
uint8_t ano_data_fill_s8bit(uint8_t *data, int8_t _d, uint8_t index)
{
    data[index] = BYTE0(_d);
    return index + 1;
}
uint8_t ano_data_fill_s16bit(uint8_t *data, int16_t _d, uint8_t index)
{
    data[index] = BYTE0(_d);
    data[index + 1] = BYTE1(_d);
    return index + 2;
}
uint8_t ano_data_fill_s32bit(uint8_t *data, int32_t _d, uint8_t index)
{
    data[index] = BYTE0(_d);
    data[index + 1] = BYTE1(_d);
    data[index + 2] = BYTE2(_d);
    data[index + 3] = BYTE3(_d);
    return index + 4;
}
uint8_t ano_data_get_u8bit(uint8_t *data, uint8_t index, uint8_t *ret)
{
    *ret = 0;
    *ret |= (uint8_t)(data[index]);
    return index + 1;
}
uint8_t ano_data_get_u16bit(uint8_t *data, uint8_t index, uint16_t *ret)
{
    *ret = 0;
    *ret |= (uint16_t)(data[index]);
    *ret |= (uint16_t)(data[index + 1] << 8);
    return index + 2;
}
uint8_t ano_data_get_u32bit(uint8_t *data, uint8_t index, uint32_t *ret)
{
    *ret = 0;
    *ret |= (uint32_t)(data[index]);
    *ret |= (uint32_t)(data[index + 1] << 8);
    *ret |= (uint32_t)(data[index + 2] << 16);
    *ret |= (uint32_t)(data[index + 3] << 24);
    return index + 4;
}
uint8_t ano_data_get_s8bit(uint8_t *data, uint8_t index, int8_t *ret)
{
    *ret = 0;
    *ret |= (uint8_t)(data[index]);
    return index + 1;
}
uint8_t ano_data_get_s16bit(uint8_t *data, uint8_t index, int16_t *ret)
{
    *ret = 0;
    *ret |= (uint16_t)(data[index]);
    *ret |= (uint16_t)(data[index + 1] << 8);
    return index + 2;
}
uint8_t ano_data_get_s32bit(uint8_t *data, uint8_t index, int32_t *ret)
{
    *ret = 0;
    *ret |= (uint32_t)(data[index]);
    *ret |= (uint32_t)(data[index + 1] << 8);
    *ret |= (uint32_t)(data[index + 2] << 16);
    *ret |= (uint32_t)(data[index + 3] << 24);
    return index + 4;
}
void ano_data_make_isensor(uint8_t *data, uint16_t *len)
{
    uint8_t shock_sta = 1;

    uint8_t next = 0;
    uint8_t DATA_LEN = 13;
    ano_data_make_head(data, len, MAX_TX_TEMP);
    *len = DATA_LEN + ADDI_BYTE_NUM;
    data[FUN_LOC] = INERTIAL_SENSOR;
    data[LEN_LOC] = DATA_LEN;
    next = ano_data_fill_s16bit(data, MPU6050_ACC_RAW.X, 4);
    next = ano_data_fill_s16bit(data, MPU6050_ACC_RAW.Y, next);
    next = ano_data_fill_s16bit(data, MPU6050_ACC_RAW.Z, next);
    next = ano_data_fill_s16bit(data, MPU6050_GYRO_RAW.X, next);
    next = ano_data_fill_s16bit(data, MPU6050_GYRO_RAW.Y, next);
    next = ano_data_fill_s16bit(data, MPU6050_GYRO_RAW.Z, next);
    next = ano_data_fill_u8bit(data, shock_sta, next);
    ano_data_check_fill(data, *len);
}
void ano_data_make_euler_arc(uint8_t *data, uint16_t *len)
{
    uint8_t fusion_sta = 1;

    uint8_t next = 0;
    uint8_t DATA_LEN = 7;
    ano_data_make_head(data, len, MAX_TX_TEMP);
    *len = DATA_LEN + ADDI_BYTE_NUM;
    data[FUN_LOC] = EULER_ARC;
    data[LEN_LOC] = DATA_LEN;
    next = ano_data_fill_s16bit(data, Angle.rol * 100, 4);
    next = ano_data_fill_s16bit(data, Angle.pit * 100, next);
    next = ano_data_fill_s16bit(data, Angle.yaw * 100, next);
    next = ano_data_fill_u8bit(data, fusion_sta, next);
    ano_data_check_fill(data, *len);
}
void ano_data_make_target_euler_arc(uint8_t *data, uint16_t *len)
{
    int16_t rol = 1, pit = 1, yaw = 1;

    uint8_t next = 0;
    uint8_t DATA_LEN = 6;
    ano_data_make_head(data, len, MAX_TX_TEMP);
    *len = DATA_LEN + ADDI_BYTE_NUM;
    data[FUN_LOC] = TARGET_ARC;
    data[LEN_LOC] = DATA_LEN;
    next = ano_data_fill_s16bit(data, rol, 4);
    next = ano_data_fill_s16bit(data, pit, next);
    next = ano_data_fill_s16bit(data, yaw, next);
    ano_data_check_fill(data, *len);
}
void ano_data_make_ctrl_euler_arc(uint8_t *data, uint16_t *len)
{
    uint8_t next = 0;
    uint8_t DATA_LEN = 8;
    ano_data_make_head(data, len, MAX_TX_TEMP);
    *len = DATA_LEN + ADDI_BYTE_NUM;
    data[FUN_LOC] = CONTROL_INFO;
    data[LEN_LOC] = DATA_LEN;
    next = ano_data_fill_s16bit(data, canfly.c_rol, 4);
    next = ano_data_fill_s16bit(data, canfly.c_pit, next);
    next = ano_data_fill_s16bit(data, canfly.c_thr, next);
    next = ano_data_fill_u16bit(data, canfly.c_yaw, next);
    ano_data_check_fill(data, *len);
}
void ano_data_make_rgb(uint8_t *data, uint16_t *len)
{
    uint8_t next = 0;
    uint8_t DATA_LEN = 4;
    ano_data_make_head(data, len, MAX_TX_TEMP);
    *len = DATA_LEN + ADDI_BYTE_NUM;
    data[FUN_LOC] = RGB_INFO;
    data[LEN_LOC] = DATA_LEN;
    next = ano_data_fill_u8bit(data, canfly.r, 4);
    next = ano_data_fill_u8bit(data, canfly.g, next);
    next = ano_data_fill_u8bit(data, canfly.b, next);
    next = ano_data_fill_u8bit(data, canfly.bright, next);
    ano_data_check_fill(data, *len);
}
void ano_data_make_pwm(uint8_t *data, uint16_t *len)
{
    uint8_t next = 0;
    uint8_t DATA_LEN = 8;
    ano_data_make_head(data, len, MAX_TX_TEMP);
    *len = DATA_LEN + ADDI_BYTE_NUM;
    data[FUN_LOC] = PWM_INFO;
    data[LEN_LOC] = DATA_LEN;
    next = ano_data_fill_u16bit(data, canfly.pwm1, 4);
    next = ano_data_fill_u16bit(data, canfly.pwm2, next);
    next = ano_data_fill_u16bit(data, canfly.pwm3, next);
    next = ano_data_fill_u16bit(data, canfly.pwm4, next);
    ano_data_check_fill(data, *len);
}
void ano_data_make_atti(uint8_t *data, uint16_t *len)
{
    int32_t alt_fu = 1, alt_add = 1;
    uint8_t alt_sta = 1;

    uint8_t next = 0;
    uint8_t DATA_LEN = 9;
    ano_data_make_head(data, len, MAX_TX_TEMP);
    *len = DATA_LEN + ADDI_BYTE_NUM;
    data[FUN_LOC] = ATTITUDE;
    data[LEN_LOC] = DATA_LEN;
    next = ano_data_fill_u32bit(data, alt_fu * 100, 4);
    next = ano_data_fill_u32bit(data, alt_add * 100, next);
    next = ano_data_fill_u8bit(data, alt_sta, next);
    ano_data_check_fill(data, *len);
}
void ano_data_make_flymode(uint8_t *data, uint16_t *len)
{
    uint8_t next = 0;
    uint8_t DATA_LEN = 5;
    ano_data_make_head(data, len, MAX_TX_TEMP);
    *len = DATA_LEN + ADDI_BYTE_NUM;
    data[FUN_LOC] = FLY_MODE;
    data[LEN_LOC] = DATA_LEN;
    next = ano_data_fill_u8bit(data, canfly.mode, 4);
    next = ano_data_fill_u8bit(data, canfly.sflag, next);
    next = ano_data_fill_u8bit(data, canfly.cid, next);
    next = ano_data_fill_u8bit(data, canfly.cmd0, next);
    next = ano_data_fill_u8bit(data, canfly.cmd1, next);
    ano_data_check_fill(data, *len);
}
void ano_data_make_bat(uint8_t *data, uint16_t *len)
{
    float bat = canfly.bat_vol;
    uint8_t next = 0;
    uint8_t DATA_LEN = 2;
    ano_data_make_head(data, len, MAX_TX_TEMP);
    *len = DATA_LEN + ADDI_BYTE_NUM;
    data[FUN_LOC] = BAT_INFO;
    data[LEN_LOC] = DATA_LEN;
    next = ano_data_fill_u16bit(data, bat * 100, 4);
    ano_data_check_fill(data, *len);
}
void ano_data_make_valid(uint8_t *data, uint16_t *len, char *rec, uint16_t rec_len)
{
    uint8_t DATA_LEN = 3;
    ano_data_make_head(data, len, MAX_TX_TEMP);
    *len = DATA_LEN + ADDI_BYTE_NUM;
    data[FUN_LOC] = VALID_SET;
    data[LEN_LOC] = DATA_LEN;
    data[LEN_LOC + 1] = rec[FUN_LOC];
    data[LEN_LOC + 2] = rec[rec_len - 2];
    data[LEN_LOC + 3] = rec[rec_len - 1];
    ano_data_check_fill(data, *len);
}
void ano_data_decode(uint8_t *data,uint16_t *len)
{
		uint16_t total=0;
		uint16_t next=0;
	  *len=0;
		if(data[0]!=ANO_HEAD)return;
		if(data[1]!=ANO_ADDR)return;
		total+=ADDI_BYTE_NUM;
		total+=data[3];
		if(ano_data_check(data,total)==0){
			return;
		}
		*len=total;
		switch(data[2]){
			case CONTROL_INFO:
				next=ano_data_get_s16bit(data,4,&canfly.c_rol);
				next=ano_data_get_s16bit(data,next,&canfly.c_pit);
				next=ano_data_get_s16bit(data,next,&canfly.c_thr);
				next=ano_data_get_s16bit(data,next,&canfly.c_yaw);
				break;
			case FLY_MODE:
				next=ano_data_get_u8bit(data,4,&canfly.mode);
				next=ano_data_get_u8bit(data,next,&canfly.sflag);
				next=ano_data_get_u8bit(data,next,&canfly.cid);
				next=ano_data_get_u8bit(data,next,&canfly.cmd0);
				next=ano_data_get_u8bit(data,next,&canfly.cmd1);
				break;
			case RGB_INFO:
				next=ano_data_get_u8bit(data,4,&canfly.r);
				next=ano_data_get_u8bit(data,next,&canfly.g);
				next=ano_data_get_u8bit(data,next,&canfly.b);
				next=ano_data_get_u8bit(data,next,&canfly.bright);
			default:break;
		}
		
}

void CanFly_Upload(void)
{
		if(DMA_GetCurrDataCounter(DMA1_Channel7)){
			return;
		}
		if(canfly.sensor_data_ready==0)return;
		
		uint16_t len=0;
		canfly.txlen=0;
		ano_data_make_isensor(canfly.txtemp+canfly.txlen,&len);
		canfly.txlen+=len;
	
		ano_data_make_euler_arc(canfly.txtemp+canfly.txlen,&len);
		canfly.txlen+=len;
		
		ano_data_make_ctrl_euler_arc(canfly.txtemp+canfly.txlen,&len);
		canfly.txlen+=len;
	
		ano_data_make_rgb(canfly.txtemp+canfly.txlen,&len);
		canfly.txlen+=len;
	
		ano_data_make_pwm(canfly.txtemp+canfly.txlen,&len);
		canfly.txlen+=len;
		    
		ano_data_make_flymode(canfly.txtemp+canfly.txlen,&len);
		canfly.txlen+=len;
		
		ano_data_make_bat(canfly.txtemp+canfly.txlen,&len);
		canfly.txlen+=len;
		
		DMA_Cmd(DMA1_Channel7,DISABLE);
		DMA_SetCurrDataCounter(DMA1_Channel7, canfly.txlen);
		DMA_Cmd(DMA1_Channel7,ENABLE);
}
void CanFly_Decode(void)
{
	uint16_t len=0;
	uint16_t offset=0;
	canfly.rxlen=MAX_RX_TEMP-DMA_GetCurrDataCounter(DMA1_Channel6);
	while(canfly.rxlen){
		ano_data_decode(canfly.rxtemp+offset,&len);
		offset+=len;
		if(len==0){
			break;
		}else{
			canfly.last_rec=canfly.tim_ms;
		}
		canfly.rxlen-=len;
	}
	DMA_SetCurrDataCounter(DMA1_Channel6,MAX_RX_TEMP);
	DMA_Cmd(DMA1_Channel6, ENABLE);
}
