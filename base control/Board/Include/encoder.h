#ifndef __ENCODER_H
#define __ENCODER_H
#include "stm32f10x.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define ENCODER_TIM_PERIOD (uint16_t)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�


void BSP_Encoder_Init(void);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
void Encoder_Init_TIM5(void);
int Read_Encoder(uint8_t TIMX);

void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM5_IRQHandler(void);
#endif
