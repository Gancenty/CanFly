#include "mpu6050.h"

FLOAT_XYZ 	Gyr_R,Gyr_D;	                //�������ǵĸ�ͨ�����������ݣ�ת���ɻ����� �� ��
FLOAT_XYZ   Acc_M;													//			��/ƽ����
FLOAT_ANGLE Angle;
INT16_XYZ   MPU6050_ACC_RAW,MPU6050_GYRO_RAW;	  //MPU����һ��ԭʼ����
INT16_XYZ	  GYRO_OFFSET_RAW,ACC_OFFSET_RAW;			//MPU��Ưֵ
FLOAT_XYZ 	Acc_filt,Gyr_filt;
uint8_t MPU6050_Buffer[14];

uint8_t MPU_Config(void)
{ 
	uint8_t ret;
	ret=mpu_read_byte(MPU_DEVICE_ID_REG); 
	while(ret!=0X68)//����ID��ȷ
	{
		ret=mpu_read_byte(MPU_DEVICE_ID_REG);
		delay_ms(100);
 	}
	mpu_write_byte(MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
  delay_ms(100);
	mpu_write_byte(MPU_PWR_MGMT1_REG,0X01);	//����MPU6050 
	mpu_write_byte(MPU_PWR_MGMT2_REG,0X00);
	mpu_write_byte(MPU_INT_EN_REG,0X00);
	mpu_write_byte(MPU_USER_CTRL_REG,0X00);
	mpu_write_byte(MPU_FIFO_EN_REG,0X00);
	
	mpu_write_byte(MPU_SAMPLE_RATE_REG,0X00);//1000hz/(1+0)
	mpu_write_byte(MPU_CFG_REG,0X04);
	mpu_write_byte(MPU_GYRO_CFG_REG,0X03<<3);//0-250 1-500 2-1000 3-2000 (��ͷֱ��� = 2^15/2000 = 16.4LSB )
	mpu_write_byte(MPU_ACCE_CFG_REG,0X01<<3);//0-2G 1-4G 2-8G 3-16G 		 (��ͷֱ��� = 2^15/4 = 8192LSB/g )
	delay_ms(10);
	mpu6050_offset();
	return 0;
}

uint8_t mpu6050_offset(void)
{
	int32_t temp[6];
	uint16_t cnt=0;
	if(cnt==0){
		temp[0]=0;
		temp[1]=0;
		temp[2]=0;	
		temp[3]=0;
		temp[4]=0;
		temp[5]=0;
		cnt=1;
	}
	delay_ms(1000);
	while(cnt<=200){
		mpu_get_sensor_data();
		temp[0]+=MPU6050_ACC_RAW.X;
		temp[1]+=MPU6050_ACC_RAW.Y;
		temp[2]+=MPU6050_ACC_RAW.Z-8192;
		
		temp[3]+=MPU6050_GYRO_RAW.X;
		temp[4]+=MPU6050_GYRO_RAW.Y;
		temp[5]+=MPU6050_GYRO_RAW.Z;
		cnt++;
		delay_ms(5);
	}
	ACC_OFFSET_RAW.X=temp[0]/200;
	ACC_OFFSET_RAW.Y=temp[1]/200;
	ACC_OFFSET_RAW.Z=temp[2]/200;
	
	GYRO_OFFSET_RAW.X=temp[3]/200;
	GYRO_OFFSET_RAW.Y=temp[4]/200;
	GYRO_OFFSET_RAW.Z=temp[5]/200;
	return 1;
}
uint8_t mpu_get_temp(float *mpu_temp)
{
	uint8_t buf[2],ret; 
	short raw;
	ret=mpu_read(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
	if(ret==0)
	{
		raw=((uint16_t)buf[0]<<8)|buf[1];  
		*mpu_temp=36.53+((float)raw)/340;  
	}
	return ret;
}

uint8_t mpu_get_gyr(short *gx,short *gy,short *gz)
{
  uint8_t buf[6],res;  
	res=mpu_read(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((uint16_t)buf[0]<<8)|buf[1];  
		*gy=((uint16_t)buf[2]<<8)|buf[3];  
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	} 	
  return res;
}
uint8_t mpu_get_acc(short *ax,short *ay,short *az)
{
  uint8_t buf[6],res;  
	res=mpu_read(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];  
		*ay=((uint16_t)buf[2]<<8)|buf[3];  
		*az=((uint16_t)buf[4]<<8)|buf[5];
	} 	
  return res;;
}
void mpu_get_sensor_data(void)
{
	uint8_t res;  
	short raw;
	res=mpu_read(MPU_ADDR,MPU_ACCEL_XOUTH_REG,14,MPU6050_Buffer);
	if(res==0)
	{
		MPU6050_ACC_RAW.X=((int16_t)MPU6050_Buffer[0]<<8)|MPU6050_Buffer[1];  
		MPU6050_ACC_RAW.Y=((int16_t)MPU6050_Buffer[2]<<8)|MPU6050_Buffer[3];  
		MPU6050_ACC_RAW.Z=((int16_t)MPU6050_Buffer[4]<<8)|MPU6050_Buffer[5];
		raw=((uint16_t)MPU6050_Buffer[6]<<8)|MPU6050_Buffer[7];  
		canfly.mpu_temp=36.53+((float)raw)/340;  
		MPU6050_GYRO_RAW.X=((int16_t)MPU6050_Buffer[8]<<8)|MPU6050_Buffer[9];  
		MPU6050_GYRO_RAW.Y=((int16_t)MPU6050_Buffer[10]<<8)|MPU6050_Buffer[11];  
		MPU6050_GYRO_RAW.Z=((int16_t)MPU6050_Buffer[12]<<8)|MPU6050_Buffer[13];
	}
	
}
uint8_t mpu_write(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	return IIC_WRITE(MPU_ADDR,reg,len,buf);	
} 
uint8_t mpu_read(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
 	return IIC_READ(MPU_ADDR,reg,len,buf);
}
uint8_t mpu_write_byte(uint8_t reg,uint8_t data) 				 
{ 
   return IIC_WRITE_BYTE(MPU_ADDR,reg,data);
}
uint8_t mpu_read_byte(uint8_t reg)
{
	return IIC_READ_BYTE(MPU_ADDR,reg);
}


#define N 20
float FindPos(float*a,int low,int high)
{
    float val = a[low];                      //ѡ��һ��Ҫȷ��ֵvalȷ��λ��
    while(low<high)
    {
        while(low<high && a[high]>=val)
             high--;                       //����ұߵ�������VAL�±���ǰ��
             a[low] = a[high];             //���ұߵ�ֵС��VAL��ֵ��A[low]

        while(low<high && a[low]<=val)
             low++;                        //�����ߵ���С��VAL�±�������
             a[high] = a[low];             //����ߵ�ֵ����VAL��ֵ���ұ�a[high]
    }
    a[low] = val;//
    return low;
}

 void QuiteSort(float* a,int low,int high)
 {
     int pos;
     if(low<high)
     {
         pos = FindPos(a,low,high); //����һ��λ��
         QuiteSort(a,low,pos-1);    //�ݹ����
         QuiteSort(a,pos+1,high);
     }
 }
void  SortAver_FilterXYZ(INT16_XYZ *acc,FLOAT_XYZ *Acc_filt,uint8_t n)
{
	static float bufx[N],bufy[N],bufz[N];
	static uint8_t cnt =0,flag = 1;
	float temp1=0,temp2=0,temp3=0;
	uint8_t i;
	bufx[cnt] = acc->X;
	bufy[cnt] = acc->Y;
	bufz[cnt] = acc->Z;
	cnt++;      //�����λ�ñ����ڸ�ֵ���󣬷���bufx[0]���ᱻ��ֵ
	if(cnt<n && flag) 
		return;   //�������������
	else
		flag = 0;
	
  QuiteSort(bufx,0,n-1);
	QuiteSort(bufy,0,n-1);
	QuiteSort(bufz,0,n-1);
	for(i=1;i<n-1;i++)
	 {
		temp1 += bufx[i];
		temp2 += bufy[i];
		temp3 += bufz[i];
	 }

	 if(cnt>=n) cnt = 0;
	 Acc_filt->X  = temp1/(n-2);
	 Acc_filt->Y  = temp2/(n-2);
	 Acc_filt->Z  = temp3/(n-2);
}
#define AcctoG  	  0.0011953f				//���ٶȱ��G (��ʼ�����ٶ�������-+4g LSBa = 2*4/65536.0*G)
#define GyrotoD 	  0.0610351f				//���ٶȱ�ɶ� (��ʼ��������������+-2000 LSBg = 2*2000/65536.0)
#define GyrotoR	    0.0010652f			  //���ٶȱ�ɻ���(3.1415/180 * GyrotoD)     

#define IIR_MODE    1
#define Kp_New 0.9
#define Kp_Old 0.1
void CanFly_Calcu_Position(void)
{
	static float acc_old[3];
	canfly.sensor_data_ready=0;
	mpu_get_sensor_data();
	
	MPU6050_ACC_RAW.X = (MPU6050_ACC_RAW.X-ACC_OFFSET_RAW.X);
	MPU6050_ACC_RAW.Y = (MPU6050_ACC_RAW.Y-ACC_OFFSET_RAW.Y);
	MPU6050_ACC_RAW.Z = (MPU6050_ACC_RAW.Z-ACC_OFFSET_RAW.Z);
    
	MPU6050_GYRO_RAW.X = (MPU6050_GYRO_RAW.X-GYRO_OFFSET_RAW.X);  
	MPU6050_GYRO_RAW.Y = (MPU6050_GYRO_RAW.Y-GYRO_OFFSET_RAW.Y);
	MPU6050_GYRO_RAW.Z = (MPU6050_GYRO_RAW.Z-GYRO_OFFSET_RAW.Z);
	canfly.sensor_data_ready=1;
	//SortAver_FilterXYZ(&MPU6050_ACC_RAW,&Acc_filt,5);
	
	Acc_M.X = (float)(MPU6050_ACC_RAW.X) * AcctoG;
	Acc_M.Y = (float)(MPU6050_ACC_RAW.Y) * AcctoG;
	Acc_M.Z = (float)(MPU6050_ACC_RAW.Z) * AcctoG;

	//������ADֵ ת���� ����/��    
	Gyr_R.X = (float) (MPU6050_GYRO_RAW.X) * GyrotoR;  
	Gyr_R.Y = (float) (MPU6050_GYRO_RAW.Y) * GyrotoR;
	Gyr_R.Z = (float) (MPU6050_GYRO_RAW.Z) * GyrotoR;
	
	Gyr_D.X = (float) (MPU6050_GYRO_RAW.X) * GyrotoD;  
	Gyr_D.Y = (float) (MPU6050_GYRO_RAW.Y) * GyrotoD;
	Gyr_D.Z = (float) (MPU6050_GYRO_RAW.Z) * GyrotoD;
	//printf("gx=%0.2f,gy=%0.2f,gz=%0.2f\r\n",canfly.f_gyr_x,canfly.f_gyr_y,canfly.f_gyr_z);
	if(IIR_MODE)
	{
		Acc_M.X = Acc_M.X * Kp_New + acc_old[0] * Kp_Old;
		Acc_M.Y = Acc_M.Y * Kp_New + acc_old[1] * Kp_Old;
		Acc_M.Z = Acc_M.Z * Kp_New + acc_old[2] * Kp_Old;
		
		acc_old[0] =  Acc_M.X;
		acc_old[1] =  Acc_M.Y;
		acc_old[2] =  Acc_M.Z;
	}
	mpu_get_euler_angle();
}
/**************************ʵ�ֺ���*********************************************************************
��  ����static float invSqrt(float x) 
������: ���ټ��� 1/Sqrt(x) 	
��  ����Ҫ�����ֵ
����ֵ�����
��  ע������ͨSqrt()����Ҫ���ı�See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
*********************************************************************************************************/
static float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}
/*********************************************************************************************************
*��  ����void IMUupdate(FLOAT_XYZ *Gyr_rad,FLOAT_XYZ *Acc_filt,FLOAT_ANGLE *Att_Angle)
*�����ܣ���ȡ��̬��
*��  ����Gyr_rad ָ����ٶȵ�ָ�루ע�ⵥλ�����ǻ��ȣ�
*        Acc_filt ָ����ٶȵ�ָ��
*        Att_Angle ָ����̬�ǵ�ָ��
*����ֵ����
*��  ע�������Ԫ����ŷ���Ƕ��ڴ˺��������
**********************************************************************************************************/	
//kp=ki=0 ������ȫ����������
#define Kp 2.1                         // proportional gain governs rate of convergence to accelerometer/magnetometer
                                         //����������Ƽ��ٶȼƣ������Ƶ���������
#define Ki 0.01f                        // integral gain governs rate of convergence of gyroscope biases  
                                         //���������������ƫ��������ٶ�
#define halfT 0.005f                     // half the sample period �������ڵ�һ��

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;     // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error

void mpu_get_euler_angle(void)
{
	uint8_t i;
	float matrix[9] = {1.f,  0.0f,  0.0f, 0.0f,  1.f,  0.0f, 0.0f,  0.0f,  1.f };//��ʼ������
  float ax = Acc_M.X,ay = Acc_M.Y,az = Acc_M.Z;
  float gx = Gyr_R.X,gy = Gyr_R.Y,gz = Gyr_R.Z;
  float vx, vy, vz;
  float ex, ey, ez;
	float norm;

  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  float q0q3 = q0*q3;
  float q1q1 = q1*q1;
  float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
	
	if(ax*ay*az==0)
 		return;
	
  //���ٶȼƲ�������������(��������ϵ) 
	norm = invSqrt(ax*ax + ay*ay + az*az); 
  ax = ax * norm;
  ay = ay * norm;
  az = az * norm;
//	printf("ax=%0.2f ay=%0.2f az=%0.2f\r\n",ax,ay,az);
 
	//�����ǻ��ֹ�����������(��������ϵ) 
  vx = 2*(q1q3 - q0q2);												
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;
 // printf("vx=%0.2f vy=%0.2f vz=%0.2f\r\n",vx,vy,vz); 
	
	//�������������������������������������������� 
  ex = (ay*vz - az*vy); //+ (my*wz - mz*wy);                     
  ey = (az*vx - ax*vz); //+ (mz*wx - mx*wz);
  ez = (ax*vy - ay*vx); //+ (mx*wy - my*wx);

  //��������������л���
  exInt = exInt + ex * Ki;								 
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;

  //�����PI�󲹳���������
  gx = gx + Kp*ex + exInt;					   		  	
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;//�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�

  //��Ԫ�ص�΢�ַ���
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  //��λ����Ԫ�� 
  norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 * norm;
  q1 = q1 * norm;
  q2 = q2 * norm;  
  q3 = q3 * norm;
	
	//����R ����������ϵ(n)ת������������ϵ(b) 
	matrix[0] = q0q0 + q1q1 - q2q2 - q3q3;// 11(ǰ�к���)
	matrix[1] = 2.f * (q1q2 + q0q3);	    // 12
	matrix[2] = 2.f * (q1q3 - q0q2);	    // 13
	matrix[3] = 2.f * (q1q2 - q0q3);	    // 21
	matrix[4] = q0q0 - q1q1 + q2q2 - q3q3;// 22
	matrix[5] = 2.f * (q2q3 + q0q1);	    // 23
	matrix[6] = 2.f * (q1q3 + q0q2);	    // 31
	matrix[7] = 2.f * (q2q3 - q0q1);	    // 32
	matrix[8] = q0q0 - q1q1 - q2q2 + q3q3;// 33
	 
  //��Ԫ��ת����ŷ����(Z->Y->X) 
  Angle.yaw -= Gyr_D.Z *0.005f;
	
	//Angle.yaw = atan2(2.f * (q1q2 - q0q3), q0q0 - q1q1 + q2q2 - q3q3)* RadtoDeg; // yaw
  Angle.pit = asin(2.f * (q2q3 + q0q1))* RadtoDeg;                                 // pitch(����Ҫע��) 
  Angle.rol = -atan2(2.f * (q1q3 - q0q2), q0q0 - q1q1 - q2q2 + q3q3)* RadtoDeg ; // roll
//  for(i=0;i<9;i++)
//  {
//    *(&(DCMgb[0][0])+i) = matrix[i];
//  }
	
	//ʧ�ر��� (����ʱ��ע�͵�)
//	Safety_Check(); 
}



