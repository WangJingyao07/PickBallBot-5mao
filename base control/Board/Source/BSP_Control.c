#include "BSP_Control.h"
long int Target_A,Target_B,Target_C,Target_D;     //���Ŀ��ֵ
long int Position_A,Position_B,Position_C,Position_D,Rate_A,Rate_B,Rate_C,Rate_D; //PID������ر���
float linear_x,linear_y,angular_z,position_x,position_y,position_z;
//float Velocity_KP=20,Velocity_KI=10;	          //�ٶȿ���PID����
float Velocity_KP=50,Velocity_KI=50;	          //�ٶȿ���PID����
float	Position_KP=50,Position_KI=0,Position_KD=10;  //λ�ÿ���PID����
#define a_PARAMETER          (0.125f)               
#define b_PARAMETER          (0.153f)
#define WHEEL_DIAMETER  			3.1415926*0.080 //wheel's diameter in meters
/**************************************************************************
�������ܣ�С���˶���ѧģ��
��ڲ�����X Y Z �����ٶȻ���λ��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis(float Vx,float Vy,float Vz)
{
        Target_A   = -Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
        Target_B   = +Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
	      Target_C   = -Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
				Target_D   = +Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
}
/**************************************************************************
�������ܣ���ȡλ�ÿ��ƹ����ٶ�ֵ
��ڲ�����X Y Z ����λ�ñ仯��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis2(float Vx,float Vy,float Vz)
{
	      Rate_A   = -Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
        Rate_B   = +Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
	      Rate_C   = -Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
				Rate_D   = +Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
}

void getVelocities(int rpm1, int rpm2, int rpm3, int rpm4)
{
    float average_rps_x;
    float average_rps_y;
    float average_rps_a;

    //convert average revolutions per minute to revolutions per second
    average_rps_x = ((float)(rpm1 + rpm2 + rpm3 + rpm4) / 4) / 22;  
    linear_x = average_rps_x * WHEEL_DIAMETER;  
		position_x+=linear_x;
    //convert average revolutions per minute in y axis to revolutions per second
    average_rps_y = ((float)(-rpm1 + rpm2 - rpm3 + rpm4) / 4) / 22;  
    linear_y = average_rps_y * WHEEL_DIAMETER;  
		position_y+=linear_y;
    //convert average revolutions per minute to revolutions per second
    average_rps_a = ((float)(-rpm1 - rpm2 + rpm3 + rpm4) / 4) / 22;
    angular_z =  (average_rps_a * WHEEL_DIAMETER) / (a_PARAMETER  + b_PARAMETER ); 
		position_z+=angular_z;
}
/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>10000)Pwm=10000;
	 if(Pwm<-10000)Pwm=-10000;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>10000)Pwm=10000;
	 if(Pwm<-10000)Pwm=-10000;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_C (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>10000)Pwm=10000;
	 if(Pwm<-10000)Pwm=-10000;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_D (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>10000)Pwm=10000;
	 if(Pwm<-10000)Pwm=-10000;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
int Position_PID_A (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
int Position_PID_B (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
int Position_PID_C (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
int Position_PID_D (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int motor_c,int motor_d)
{
	    int siqu=0;
    	if(motor_a>0)			PWMA=motor_a+siqu, INA=0;
			else  	          PWMA=10000+motor_a-siqu,INA=1;
	
			if(motor_b>0)			PWMB=motor_b+siqu, INB=0;
			else 	          	PWMB=10000+motor_b-siqu,INB=1;
	
//	    if(motor_c>0)			PWMC=motor_c+siqu,INC=0;
//			else 	            PWMC=1000+motor_c-siqu,INC=1;
	
	   if(motor_c>0)			PWMC=10000-motor_c-siqu,INC=1;
			else 	            PWMC=-motor_c+siqu,INC=0;
	
			if(motor_d>0)			PWMD=motor_d+siqu,IND=0;
			else 	            PWMD=10000+motor_d-siqu,IND=1;
}
