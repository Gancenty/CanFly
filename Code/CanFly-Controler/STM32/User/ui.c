#include "ui.h"
static char temp[512];
uint8_t line[128];
#define AcctoG  	  0.0011953f				//加速度变成G (初始化加速度满量程-+4g LSBa = 2*4/65536.0*G)
#define GyrotoD 	  0.0610351f				//角速度变成度 (初始化陀螺仪满量程+-2000 LSBg = 2*2000/65536.0)
#define GyrotoR	    0.0010652f			  //角速度变成弧度(3.1415/180 * GyrotoD)     
void CanFly_UI(void)
{
	OLED_GReset();
	sprintf(temp,"CanFly    Bat:%.1fV",canfly_ctr.ano_bat);
	OLED_GDrawString(0,0,temp,WIDTH,HIGHT);
	
	sprintf(temp,"Rol:%.1f x:%.1f",canfly_ctr.rol,(float)canfly_ctr.gyr_x*GyrotoD);
	OLED_GDrawString(0,16,temp,WIDTH,HIGHT);
	
	sprintf(temp,"Pit:%.1f y:%.1f",canfly_ctr.pit,(float)canfly_ctr.gyr_y*GyrotoD);
	OLED_GDrawString(0,32,temp,WIDTH,HIGHT);
	
	sprintf(temp,"Yaw:%.1f z:%.1f",canfly_ctr.yaw,(float)canfly_ctr.gyr_z*GyrotoD);
	OLED_GDrawString(0,48,temp,WIDTH,HIGHT);
	
	OLED_Display();
}
void Motor_UI(void)
{
	OLED_GReset();
	sprintf(temp,"Motor     Bat:%.1fV",canfly_ctr.ano_bat);
	OLED_GDrawString(0,0,temp,WIDTH,HIGHT);
	
	sprintf(temp,"Pwm1:%03d Pwm3:%03d",canfly_ctr.pwm1,canfly_ctr.pwm3);
	OLED_GDrawString(0,16,temp,WIDTH,HIGHT);
	
	sprintf(temp,"Pwm4:%03d Pwm2:%03d",canfly_ctr.pwm4,canfly_ctr.pwm2);
	OLED_GDrawString(0,32,temp,WIDTH,HIGHT);
	
	sprintf(temp,"Thro:%03d",canfly_ctr.c_thr);
	OLED_GDrawString(0,48,temp,WIDTH,HIGHT);
	
	OLED_Display();
}
void Rc_UI(void)
{
	OLED_GReset();
	
	sprintf(temp,"RC-Info");
	OLED_GDrawString(0,0,temp,WIDTH,HIGHT);
	
	sprintf(temp,"Rol:%02d Bat:%.1fV",canfly_ctr.c_rol,canfly_ctr.bat_vol);
	OLED_GDrawString(0,16,temp,WIDTH,HIGHT);;
	
	sprintf(temp,"Pit:%02d Bri:%d",canfly_ctr.c_pit,canfly_ctr.bright);
	OLED_GDrawString(0,32,temp,WIDTH,HIGHT);
	
	sprintf(temp,"Yaw:%02d",canfly_ctr.c_yaw);
	OLED_GDrawString(0,48,temp,WIDTH,HIGHT);
	OLED_Display();	
}
void UI_Refresh(void)
{
	switch(canfly_ctr.ui_mode){
		case RC_UI:Rc_UI();break;
		case CANFLY_UI:CanFly_UI();break;
		case MOTOR_UI:Motor_UI();break;
	}
}
