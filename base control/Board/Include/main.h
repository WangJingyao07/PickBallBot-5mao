#ifndef __MAIN_H
#define __MAIN_H
#include "stm32f10x.h"
#include "BSP_Delay.h"
#include "BSP_DCMotor.h"
#include "Test_DCMotor.h"
#include "BSP_DebugUART.h"
#include "encoder.h"
#include "DataScope_DP.h"
#include "BSP_Timer.h"
#include "BSP_Key.h"
#include "BSP_Control.h"
#include "BSP_BlueTooth.h"
#include "BSP_ADConverter.h"
#include "dma.h"
#include "gy85.h"

extern long int Position_A,Position_B,Position_C,Position_D,Rate_A,Rate_B,Rate_C,Rate_D; //PID控制相关变量
extern long int Motor_A,Motor_B,Motor_C,Motor_D;        //电机PWM变量





#endif 
