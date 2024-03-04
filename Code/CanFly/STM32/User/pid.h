#ifndef _PID_H_
#define _PID_H_
#include "stdint.h"
#include "string.h"
#include "config.h"
//PID�㷨�����ݽṹ 
typedef struct PID
{
  float P;//�������������
  float I;//�������������
  float D;//΢�����������
  float Error;     //������
  float Integral;  //������
  float Differ;    //΢����
  float PreError;  //ǰһ�����
  float Ilimit;    //���ֿ�ʼ��Χ
  float Irang;	   //���������ֵ
  uint8_t Ilimit_flag;    //���ַ����־
  float Pout;//���������
  float Iout;//���������
  float Dout;//΢�������
  float OutPut;//PID���      
}PID_TYPE; 

//�ǶȻ�PID
extern PID_TYPE PID_ROL_Angle;
extern PID_TYPE PID_PIT_Angle;
extern PID_TYPE PID_YAW_Angle;
//���ٶȻ�PID
extern PID_TYPE PID_ROL_Rate;
extern PID_TYPE PID_PIT_Rate;
extern PID_TYPE PID_YAW_Rate;
//�߶�PID
extern PID_TYPE PID_ALT_Rate;
extern PID_TYPE PID_ALT;

void PID_Postion_Cal(PID_TYPE*PID,float target,float measure);
void PID_Parm_Config(void);

#endif
