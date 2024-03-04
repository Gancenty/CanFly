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
    next = ano_data_fill_s16bit(data, canfly.acc_x, 4);
    next = ano_data_fill_s16bit(data, canfly.acc_y, next);
    next = ano_data_fill_s16bit(data, canfly.acc_z, next);
    next = ano_data_fill_s16bit(data, canfly.gyr_x, next);
    next = ano_data_fill_s16bit(data, canfly.gyr_y, next);
    next = ano_data_fill_s16bit(data, canfly.gyr_z, next);
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
    next = ano_data_fill_s16bit(data, canfly.rol * 100, 4);
    next = ano_data_fill_s16bit(data, canfly.pit * 100, next);
    next = ano_data_fill_s16bit(data, canfly.yaw * 100, next);
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
    uint8_t r = 1, g = 1, b = 1, bright = 1;

    uint8_t next = 0;
    uint8_t DATA_LEN = 4;
    ano_data_make_head(data, len, MAX_TX_TEMP);
    *len = DATA_LEN + ADDI_BYTE_NUM;
    data[FUN_LOC] = RGB_INFO;
    data[LEN_LOC] = DATA_LEN;
    next = ano_data_fill_u8bit(data, r, 4);
    next = ano_data_fill_u8bit(data, g, next);
    next = ano_data_fill_u8bit(data, b, next);
    next = ano_data_fill_u8bit(data, bright, next);
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
bool CanFly_Msg_Valid(void)
{
    int len = queue_insert(canfly.rxtemp,canfly.rxlen);
    while(queue_size()>ADDI_BYTE_NUM){
        uint16_t data_len=0;
        uint8_t fun_code=0;
        while (queue_peek(1)!=ANO_HEAD)
        {
            queue_pop();
            if(queue_size()==0)return false;
        }
        if(queue_size()<6)return false;
        if (queue_peek(2)!=ANO_ADDR)
        {
            queue_pop();
            continue;
        }
        fun_code=queue_peek(3);
        data_len=queue_peek(4);
        if(queue_size()<data_len+ADDI_BYTE_NUM)return false;
        for(int i=0;i<data_len+ADDI_BYTE_NUM;i++){
            canfly.wtxtemp[i+canfly.wtxlen]=queue_peek(1);
            queue_pop();
        }
        if(ano_data_check(canfly.wtxtemp+canfly.wtxlen,data_len+ADDI_BYTE_NUM)==true){
            canfly.wtxlen+=data_len+ADDI_BYTE_NUM;
        }
    }
    if(canfly.wtxlen==0)return false;
    else return true;
}