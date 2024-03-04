#include "control.h"
void PWM_Limit(int32_t *pwm,uint16_t min,uint16_t max)
{
	if(*pwm<min)*pwm=min;
	if(*pwm>max)*pwm=max;
}
void CanFly_Control(void)
{
	if((canfly.status&CONNECT_MARK)==NO_CONNECT){
		canfly.pwm1=canfly.pwm2=canfly.pwm3=canfly.pwm4=0;
		Motor_Pwm_Set(canfly.pwm1,canfly.pwm2,canfly.pwm3,canfly.pwm4);
		return;
	}
	if(canfly.sflag==0){
		canfly.status&=~FLY_MARK;
		canfly.pwm1=canfly.pwm2=canfly.pwm3=canfly.pwm4=0;
		Motor_Pwm_Set(canfly.pwm1,canfly.pwm2,canfly.pwm3,canfly.pwm4);
	}else{
		canfly.status|=IN_FLY_MODE;
		CanFly_PID_Call();
		Motor_Pwm_Set(canfly.pwm1,canfly.pwm2,canfly.pwm3,canfly.pwm4);
	}
}
void CanFly_PID_Call(void)
{
	//�ǶȻ�
	PID_Postion_Cal(&PID_ROL_Angle,(float)canfly.c_rol,Angle.rol);//ROLL�ǶȻ�PID ������Ƕ� ������ٶȣ�
	PID_Postion_Cal(&PID_PIT_Angle,(float)canfly.c_pit,Angle.pit);//PITH�ǶȻ�PID ������Ƕ� ������ٶȣ�
	//PID_Postion_Cal(&PID_YAW_Angle,(float)0,Angle.yaw);//YAW�ǶȻ�PID ������Ƕ� ������ٶȣ�
	
  //���ٶȻ�
	PID_Postion_Cal(&PID_ROL_Rate,PID_ROL_Angle.OutPut,Gyr_D.Y); //ROLL���ٶȻ�PID ������ǶȻ����������������������
	PID_Postion_Cal(&PID_PIT_Rate,PID_PIT_Angle.OutPut,Gyr_D.X); //PITH���ٶȻ�PID ������ǶȻ����������������������
	PID_Postion_Cal(&PID_YAW_Rate,PID_YAW_Angle.OutPut,Gyr_D.Z); //YAW���ٶȻ�PID ������Ƕȣ���������������

	if(canfly.c_thr>250){
		canfly.pwm1 = canfly.c_thr + PID_ROL_Rate.OutPut + PID_PIT_Rate.OutPut + PID_YAW_Rate.OutPut;   
		canfly.pwm2 = canfly.c_thr - PID_ROL_Rate.OutPut - PID_PIT_Rate.OutPut + PID_YAW_Rate.OutPut;   
		canfly.pwm3 = canfly.c_thr - PID_ROL_Rate.OutPut + PID_PIT_Rate.OutPut - PID_YAW_Rate.OutPut;   
		canfly.pwm4 = canfly.c_thr + PID_ROL_Rate.OutPut - PID_PIT_Rate.OutPut - PID_YAW_Rate.OutPut; 
	}else{
		canfly.pwm1 = 0;   
		canfly.pwm2 = 0;   
		canfly.pwm3 = 0;   
		canfly.pwm4 = 0; 	
	}		
	
	PWM_Limit(&canfly.pwm1,0,PWM_MAX);
	PWM_Limit(&canfly.pwm2,0,PWM_MAX);
	PWM_Limit(&canfly.pwm3,0,PWM_MAX);
	PWM_Limit(&canfly.pwm4,0,PWM_MAX);
}


