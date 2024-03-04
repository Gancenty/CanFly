#include "pid.h"

//角度环PID 
PID_TYPE PID_ROL_Angle;
PID_TYPE PID_PIT_Angle;
PID_TYPE PID_YAW_Angle;
//角速度环PID 
PID_TYPE PID_ROL_Rate;
PID_TYPE PID_PIT_Rate;
PID_TYPE PID_YAW_Rate;
//高度环PID 
PID_TYPE PID_ALT_Rate;
PID_TYPE PID_ALT;

void PID_Postion_Cal(PID_TYPE*PID,float target,float measure)
{
	PID->Error  = target - measure;              //误差
	PID->Differ = PID->Error - PID->PreError;    //微分量
	
	PID->Pout = PID->P * PID->Error;                        //比例控制
	PID->Iout = PID->Ilimit_flag * PID->I * PID->Integral;  //积分控制
	PID->Dout = PID->D * PID->Differ;                       //微分控制
	
	if(((canfly.status&FLY_MARK)==IN_FLY_MODE)&&(canfly.c_thr >= 300)) //飞机解锁之后再加入积分,防止积分过调
	{
		if(measure > (PID->Ilimit)||measure < -PID->Ilimit){   //积分分离
			PID->Ilimit_flag = 0;
			//PID->Integral = 0;
		}else{
			PID->Ilimit_flag = 1;                               //加入积分
			PID->Integral += PID->Error;                        //对误差进行积分
			if(PID->Integral > PID->Irang)                      //积分限幅
          PID->Integral = PID->Irang;
		  if(PID->Integral < -PID->Irang)                     //积分限幅
			    PID->Integral = -PID->Irang;                    
		}
	}else{
		PID->Integral = 0;
	}

	PID->OutPut =  PID->Pout + PID->Iout + PID->Dout;       //比例 + 积分 + 微分总控制
	
	PID->PreError = PID->Error ;                            //前一个误差值
}




void PID_Parm_Config(void)
{
	memset(&PID_ROL_Angle,0,sizeof(PID_TYPE));
	memset(&PID_PIT_Angle,0,sizeof(PID_TYPE));
	memset(&PID_YAW_Angle,0,sizeof(PID_TYPE));
	
	memset(&PID_ROL_Rate,0,sizeof(PID_TYPE));
	memset(&PID_PIT_Rate,0,sizeof(PID_TYPE));
	memset(&PID_YAW_Rate,0,sizeof(PID_TYPE));
	
	//角度环数据PID参数
	PID_ROL_Angle.P = 1.6;
	PID_ROL_Angle.I = 0;
	PID_ROL_Angle.D = 0;
	PID_PIT_Angle.P =	1.6;
	PID_PIT_Angle.I = 0;
	PID_PIT_Angle.D = 0;
	PID_YAW_Angle.P = 1.6;
	PID_YAW_Angle.I = 0;
	PID_YAW_Angle.D = 0;
	
	//角速度环PID参数
	PID_ROL_Rate.P = 1.4;
	PID_ROL_Rate.I = 0.0652;
	PID_ROL_Rate.D = 0.7;
	PID_PIT_Rate.P = 1.4;
	PID_PIT_Rate.I = 0.0652;
	PID_PIT_Rate.D = 0.7;
	PID_YAW_Rate.P = 0.97;
	PID_YAW_Rate.I = 0;
	PID_YAW_Rate.D = 0;
	
	//高度环PID
	PID_ALT.P = 0;
	PID_ALT.I = 0;
	PID_ALT.D = 0;
	PID_ALT_Rate.P = 0;
	PID_ALT_Rate.I = 0;
	PID_ALT_Rate.D = 0;
	
	//ROLL轴
  PID_ROL_Rate.Ilimit_flag = 0; //Roll轴角速度积分的分离标志
	PID_ROL_Rate.Ilimit = 120;    //Roll轴角速度积分范围
	PID_ROL_Rate.Irang = 500;    //Roll轴角速度积分限幅度（由于电机输出有限，所以积分输出也是有限的）
  PID_ROL_Angle.Ilimit_flag = 0;//Roll轴角度积分的分离标志
	PID_ROL_Angle.Ilimit = 35;    //Roll轴角度积分范围
	PID_ROL_Angle.Irang = 1000;    //Roll轴角度积分限幅度（由于电机输出有限，所以积分输出也是有限的）

	//PITCH轴
	PID_PIT_Rate.Ilimit_flag = 0; //Pitch轴角速度积分的分离标志
	PID_PIT_Rate.Ilimit = 120;    //Pitch轴角速度积分范围
	PID_PIT_Rate.Irang = 500;    //Pitch轴角速度积分限幅度（由于电机输出有限，所以积分输出也是有限的）
  PID_PIT_Angle.Ilimit_flag = 0;//Roll轴角度积分的分离标志
	PID_PIT_Angle.Ilimit = 35;    //Roll轴角度积分范围
	PID_PIT_Angle.Irang = 1000;    //Roll轴角度积分限幅度（由于电机输出有限，所以积分输出也是有限的）
	
	//YAW轴
	PID_YAW_Rate.Ilimit_flag = 0; //Yaw轴角速度积分的分离标志
	PID_YAW_Rate.Ilimit = 150;    //Yaw轴角速度积分范围
	PID_YAW_Rate.Irang = 1200;    //Yaw轴角速度积分限幅度（由于电机输出有限，所以积分输出也是有限的）
	PID_YAW_Angle.Ilimit_flag = 0;//Yaw轴角度积分的分离标志
	PID_YAW_Angle.Ilimit = 35;    //Yaw轴角度积分范围
	PID_YAW_Angle.Irang = 200;    //Yaw轴角度积分限幅度（由于电机输出有限，所以积分输出也是有限的）
  

	//高度环
	PID_ALT_Rate.Ilimit_flag = 0;
	PID_ALT_Rate.Ilimit = 0;
	PID_ALT_Rate.Irang = 0;
	PID_ALT.Ilimit_flag = 0;
	PID_ALT.Ilimit = 100;
	PID_ALT.Irang = 200;
}
