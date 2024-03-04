#ifndef _CONFIG_H
#define _CONFIG_H
#include "stdint.h"
#define CanFly_Controler_Version

#define MAX_RX_TEMP     1024
#define MAX_TX_TEMP     1024
#define STAY_BAT 3.5 
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

#define RC_UI 			0X00
#define CANFLY_UI 	0X01
#define MOTOR_UI 		0X02
#define MAX_UI_CNT 	3
typedef struct CanFly_Control{	
	uint8_t rxtemp[MAX_RX_TEMP];//���ڽ��ܻ���
	uint8_t txtemp[MAX_TX_TEMP];//���ڷ��ͻ���
	volatile uint16_t rxlen;//�������ݵĳ���
	volatile uint16_t txlen;//�������ݵĳ���
	
	float bat_vol;//ң������ص�ѹ
	float ano_bat;//��������ص�ѹ
	volatile int16_t acc_x,acc_y,acc_z,gyr_x,gyr_y,gyr_z;//������������ٶ��Լ�������ٶ�
	float rol,pit,yaw;//��������̬������ŷ����

	int16_t t_rol,t_pit,t_yaw;//������Ŀ����̬��
	volatile int16_t c_rol,c_pit,c_yaw,c_thr;//ң����������̬��
	volatile uint16_t pwm1,pwm2,pwm3,pwm4;//������������PWM�����
	volatile uint8_t r,g,b,bright;//������LED��Ϣ
	volatile uint8_t mode,sflag,cid,cmd0,cmd1;//sflag -0���� -1����
	
	volatile uint32_t tim_ms;
	volatile uint32_t last_rec;
	volatile uint8_t status;
	
	uint16_t adc_normal[4];//ҡ�˳�ʼ״̬ADCֵ
	uint32_t beep_time;//��������-��λ10ms
	
	int8_t ltd;
	int8_t llr;
	
	int8_t rtd;
	int8_t rlr;
	
	uint8_t ui_mode;
}CANFLY_CONTROLER;

extern CANFLY_CONTROLER canfly_ctr;



#endif
