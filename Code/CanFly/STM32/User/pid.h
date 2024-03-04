#ifndef _PID_H_
#define _PID_H_
#include "stdint.h"
#include "string.h"
#include "config.h"
//PID算法的数据结构 
typedef struct PID
{
  float P;//比例项比例参数
  float I;//积分项比例参数
  float D;//微分项比例参数
  float Error;     //比例项
  float Integral;  //积分项
  float Differ;    //微分项
  float PreError;  //前一误差项
  float Ilimit;    //积分开始范围
  float Irang;	   //积分项最大值
  uint8_t Ilimit_flag;    //积分分离标志
  float Pout;//比例项输出
  float Iout;//积分项输出
  float Dout;//微分项输出
  float OutPut;//PID输出      
}PID_TYPE; 

//角度环PID
extern PID_TYPE PID_ROL_Angle;
extern PID_TYPE PID_PIT_Angle;
extern PID_TYPE PID_YAW_Angle;
//角速度环PID
extern PID_TYPE PID_ROL_Rate;
extern PID_TYPE PID_PIT_Rate;
extern PID_TYPE PID_YAW_Rate;
//高度PID
extern PID_TYPE PID_ALT_Rate;
extern PID_TYPE PID_ALT;

void PID_Postion_Cal(PID_TYPE*PID,float target,float measure);
void PID_Parm_Config(void);

#endif
