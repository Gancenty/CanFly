#ifndef _CONFIG_H
#define _CONFIG_H
#include "stdint.h"

#define MAX_RX_TEMP     1024
#define MAX_TX_TEMP     1024
#define STAY_BAT 3.7
#define FLY_BAT  2.8

#define CONNECT_MARK 0X01
#define POWER_MARK   0X02
#define FLY_MARK     0X04

#define NO_CONNECT 	 0X00
#define CONNECTED  	 0X01

#define LOW_POWER    0X00
#define FULL_POWER   0X02

#define STAY_MODE	   0X00
#define IN_FLY_MODE	 0X04

//三轴整型
typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
}INT16_XYZ;

//三轴浮点型
typedef struct
{
	float X;
	float Y;
	float Z;
}FLOAT_XYZ;

//姿态解算后的角度
typedef struct
{
	float rol;
	float pit;
	float yaw;
}FLOAT_ANGLE;


typedef struct CanFly{
	//USART
	uint8_t rxtemp[MAX_RX_TEMP];
	uint8_t txtemp[MAX_TX_TEMP];
	volatile uint16_t rxlen;
	volatile uint16_t txlen;
	
	float bat_vol;
	float mpu_temp;
	
	int16_t c_rol,c_pit,c_yaw,c_thr;
	int32_t pwm1,pwm2,pwm3,pwm4;
	uint8_t r,g,b,bright;
	uint8_t mode,sflag,cid,cmd0,cmd1;//mode -0姿态自稳 -1自稳和定高 sflag -0锁定 -1解锁
	
	volatile uint32_t tim_ms;
	volatile uint32_t last_rec;
	volatile uint8_t status;
	
	volatile uint8_t sensor_data_ready;
}CanFly;

extern FLOAT_XYZ 	 Gyr_R,Gyr_D;	                //把陀螺仪的各通道读出的数据，转换成弧度制 和 度
extern FLOAT_XYZ   Acc_M;													//			米/平方秒
extern FLOAT_ANGLE Angle;
extern INT16_XYZ   MPU6050_ACC_RAW,MPU6050_GYRO_RAW;	  //MPU最新一次原始数据
extern INT16_XYZ	 GYRO_OFFSET_RAW,ACC_OFFSET_RAW;			//MPU零漂值
extern FLOAT_XYZ 	 Acc_filt,Gyr_filt;	
extern CanFly canfly;
extern volatile uint16_t ADC_Value;


#endif
