/*
***************************************************************************
*    模块：Test_DCMotor 
*    描述：DCMotor 应用测试
*    作者：Huo
*    时间：2017.09.07
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "stm32f10x.h"
#include "BSP_DebugUART.h"
#include "DataScope_DP.h"
#include "BSP_Timer.h"
#include "BSP_BlueTooth.h"
#include "BSP_DCMotor.h"
#include "BSP_Control.h"
#include "encoder.h"
#include "BSP_Delay.h"
#include "DMA.h"
extern uint16_t delay_1ms; //dalay count
extern unsigned int Flag_Direction;  //uart flag Contains accepted data
extern long int Target_A,Target_B,Target_C,Target_D;     //Motor target value
extern rcv_data	com_x_rcv_data;//Data acceptance

void JTAG_Set(uint8_t mode)
	
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;      
	AFIO->MAPR&=0XF8FFFFFF; 
	AFIO->MAPR|=temp;       
} 

int main(void)
{
	u8 Flag_Move=1; //control flag 1:auto 0:Manual
	float x,y,z;
	static int Encoder_A,Encoder_B,Encoder_C,Encoder_D;
	static int Motor_A,Motor_B,Motor_C,Motor_D;
	static int Move_X,Move_Y,Move_Z;
	float step=50;
	int RC_Velocity=700;
	
	JTAG_Set(1);
	NVIC_PriorityGroupConfig(2);
	BSP_BlueTooth_Init(9600);
//BSP_DebugUART_Init(128000);
	com_x_usart_Init();
	BSP_Timer_Init(1*1000);
	BSP_Encoder_Init();
	BSP_DCMotor_Init(10000-1,72*2-1);//72 PWM=72000000/（72*2*10000） =50hz
	BSP_Delay_Init();
	Set_Pwm(0,0,0,0);
	/*Electric regulation initialization*/
	BSP_Delay_ms(1000);  
	TIM_SetCompare4(TIM1,1100);   //high level 2s 
	BSP_Delay_ms(1000);
	BSP_Delay_ms(1000);
	TIM_SetCompare4(TIM1,500);  // low level 1s 
	BSP_Delay_ms(1000);
	
	while(1)
	{
		if(delay_1ms%50==0)
		{
			TIM_SetCompare4(TIM1,540);  
			//printf("%d\r\n",Flag_Direction);
			
		 switch(Flag_Direction)   //flag control
			 {
			 case 1:  Move_X=0;           Move_Y+=step;  Move_Z=0;			          		break;
			 case 2:  Move_X=0;       		Move_Y=0;  		 Move_Z+=step;	             	break;
			 case 3:  Move_X+=step;       Move_Y=0;      Move_Z=0;			              break;
			 case 4:  Move_X+=step;       Move_Y-=step;  Move_Z=0;			              break;
			 case 5:  Move_X=0;           Move_Y-=step;  Move_Z=0;			              break;
			 case 6:  Move_X-=step;       Move_Y-=step;  Move_Z=0;			              break;
			 case 7:  Move_X-=step;       Move_Y=0;      Move_Z=0;			              break;
			 case 8:  Move_X=0;     			Move_Y=0;  		 Move_Z-=step;                break; 
			 case 10: Flag_Move=1; break;
			 case 11: Flag_Move=0; break; 
			 default:    		  Move_X=0;      Move_Y=0;    	Move_Z=0;								  break;	 
		 }
			 
			if(Move_X<-RC_Velocity) Move_X=-RC_Velocity;	   //Speed control limiting
			if(Move_X>RC_Velocity)  Move_X=RC_Velocity;	     
			if(Move_Y<-RC_Velocity) Move_Y=-RC_Velocity;	
			if(Move_Y>RC_Velocity)  Move_Y=RC_Velocity;	 
			if(Move_Z<-RC_Velocity*3.59) Move_Z=-RC_Velocity*3.59;	
			if(Move_Z>RC_Velocity*3.59)  Move_Z=RC_Velocity*3.59;
		 
			Kinematic_Analysis(Move_X,Move_Y,Move_Z);
				
		 	x=75*com_x_rcv_data.linear_vx.fv;
			y=75*com_x_rcv_data.linear_vy.fv;
			z=75*com_x_rcv_data.angular_v.fv;
			if(Flag_Move==0)	
			Kinematic_Analysis(Move_X,Move_Y,-Move_Z);
			else if(Flag_Move==1)	
			Kinematic_Analysis(y,x,z);
			data_pack();
			getVelocities(Encoder_A,Encoder_B,Encoder_C,Encoder_D);
			
			Encoder_A=Read_Encoder(2);
			Encoder_B=Read_Encoder(3);
			Encoder_C=Read_Encoder(4);
			Encoder_D=Read_Encoder(5);

			Motor_A = Incremental_PI_A(Encoder_A,Target_A);
			Motor_B = Incremental_PI_B(Encoder_B,Target_B);
			Motor_C = Incremental_PI_C(Encoder_C,Target_C);
			Motor_D = Incremental_PI_D(Encoder_D,Target_D);
			Set_Pwm(Motor_A,Motor_B,Motor_C,Motor_D);
			//DataScope(Encoder_A,Encoder_B,Encoder_C,Encoder_D,Target_B,0,0,0);
					
		}
	}
}
