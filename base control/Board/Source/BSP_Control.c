#include "BSP_Control.h"
long int Target_A,Target_B,Target_C,Target_D;     //电机目标值
long int Position_A,Position_B,Position_C,Position_D,Rate_A,Rate_B,Rate_C,Rate_D; //PID控制相关变量
float linear_x,linear_y,angular_z,position_x,position_y,position_z;
//float Velocity_KP=20,Velocity_KI=10;	          //速度控制PID参数
float Velocity_KP=50,Velocity_KI=50;	          //速度控制PID参数
float	Position_KP=50,Position_KI=0,Position_KD=10;  //位置控制PID参数
#define a_PARAMETER          (0.125f)               
#define b_PARAMETER          (0.153f)
#define WHEEL_DIAMETER  			3.1415926*0.080 //wheel's diameter in meters
/**************************************************************************
函数功能：小车运动数学模型
入口参数：X Y Z 三轴速度或者位置
返回  值：无
**************************************************************************/
void Kinematic_Analysis(float Vx,float Vy,float Vz)
{
        Target_A   = -Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
        Target_B   = +Vx+Vy+Vz*(a_PARAMETER+b_PARAMETER);
	      Target_C   = -Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
				Target_D   = +Vx+Vy-Vz*(a_PARAMETER+b_PARAMETER);
}
/**************************************************************************
函数功能：获取位置控制过程速度值
入口参数：X Y Z 三轴位置变化量
返回  值：无
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
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>10000)Pwm=10000;
	 if(Pwm<-10000)Pwm=-10000;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>10000)Pwm=10000;
	 if(Pwm<-10000)Pwm=-10000;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_C (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>10000)Pwm=10000;
	 if(Pwm<-10000)Pwm=-10000;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_D (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>10000)Pwm=10000;
	 if(Pwm<-10000)Pwm=-10000;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
int Position_PID_A (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
int Position_PID_B (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
int Position_PID_C (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
int Position_PID_D (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
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
