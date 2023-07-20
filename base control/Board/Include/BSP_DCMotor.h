#ifndef __BSP_DCMOTOR_H
#define __BSP_DCMOTOR_H

#include "stm32f10x.h"

//#define DCMOTOR_DEAD_VAL		200		//电机死区
#define DCMOTOR_DEAD_VAL		0		//电机死区
#define DCMOTOR_MAX_VAL			1000	//电机PWM最大值

void BSP_DCMotor_Init(uint16_t Period,uint16_t Prescaler);

void BSP_DCMotor1_Go(uint16_t parameter);
void BSP_DCMotor2_Go(uint16_t parameter);
void BSP_DCMotor3_Go(uint16_t parameter);
void BSP_DCMotor4_Go(uint16_t parameter);
void BSP_DCMotor5_Go(uint16_t parameter);
void BSP_DCMotor6_Go(uint16_t parameter);
void BSP_DCMotor7_Go(uint16_t parameter);
void BSP_DCMotor8_Go(uint16_t parameter);
void BSP_DCMotor1_Brake(void);
void BSP_DCMotor2_Brake(void);
void BSP_DCMotor3_Brake(void);
void BSP_DCMotor4_Brake(void);
void BSP_DCMotor5_Brake(void);
void BSP_DCMotor6_Brake(void);
void BSP_DCMotor7_Brake(void);
void BSP_DCMotor8_Brake(void);

#endif
