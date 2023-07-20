#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f10x.h"
#include "BSP_Port.h"

#define PWMD   TIM1->CCR3  //M6
#define PWMC   TIM8->CCR1  //M5
#define PWMB   TIM8->CCR2 //M4
#define PWMA   TIM8->CCR3 //M2
#define IND   PBout(10)  //M8
#define INC   PBout(11)  //M7
#define INB   PAout(8)  //M3
#define INA   PBout(14)  //M1

extern float Velocity_KP,Velocity_KI;	          //速度控制PID参数
extern float	Position_KP,Position_KI,Position_KD;  //位置控制PID参数
void Kinematic_Analysis(float Vx,float Vy,float Vz);

void Kinematic_Analysis2(float Vx,float Vy,float Vz);

int Incremental_PI_A (int Encoder,int Target);

int Incremental_PI_B (int Encoder,int Target);

int Incremental_PI_C (int Encoder,int Target);

int Incremental_PI_D (int Encoder,int Target);

int Position_PID_A (int Encoder,int Target);

int Position_PID_B (int Encoder,int Target);

int Position_PID_C (int Encoder,int Target);

int Position_PID_D (int Encoder,int Target);

void getVelocities(int rpm1, int rpm2, int rpm3, int rpm4);

void Set_Pwm(int motor_a,int motor_b,int motor_c,int motor_d);


#endif


