/*
***************************************************************************
*    ģ�飺BSP_DCMotor 
*    �������弶 DCMotor ����ģ������
*		   TIM1_CH2N-PB14  TIM8_CH3N-PB1  TIM1_CH1-PA8  TIM3_CH3-PB0
*		   TIM8_CH1-PC6   TIM1_CH3N-PB15  TIM2_CH4-PB11  TIM2_CH3-PB10
*    ���ߣ�Huo
*    ʱ�䣺2017.09.06
*    �汾��UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_DCMotor.h"

/* �ڲ��������� */
static void BSP_DCMotor_GPIO_Init(void);
static void BSP_DCMotor_TIM1_Init(uint16_t Period,uint16_t Prescaler);
static void BSP_DCMotor_TIM8_Init(uint16_t Period,uint16_t Prescaler);


/*
***************************************************************************
*	�� �� ��: BSP_DCMotor_Init
*	����˵��: ����DCMotor��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_DCMotor_Init(uint16_t Period,uint16_t Prescaler)
{
	BSP_DCMotor_GPIO_Init();
	BSP_DCMotor_TIM1_Init(Period, Prescaler);
	BSP_DCMotor_TIM8_Init(Period, Prescaler);

	BSP_DCMotor1_Brake();
	BSP_DCMotor2_Brake();
	BSP_DCMotor3_Brake();
	BSP_DCMotor4_Brake();

}

/*
***************************************************************************
*	�� �� ��: BSP_DCMotor_GPIO_Init
*	����˵��: ����DCMotor GPIO��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_DCMotor_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | \
	                  RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	//���ø�����Ϊ�����������,���TIM1 CH2N ��PWM���岨��	GPIOB.14
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //TIM1_CH2N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIO
	
	//���ø�����Ϊ�����������,���TIM8 CH3N ��PWM���岨��	GPIOB.1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM8_CH3N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIO
	
	//���ø�����Ϊ�����������,���TIM1 CH1 ��PWM���岨��	GPIOA.8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIO
	
	//���ø�����Ϊ�����������,���TIM3 CH3 ��PWM���岨��	GPIOB.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM3_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIO
	
	//���ø�����Ϊ�����������,���TIM8 CH1 ��PWM���岨��	GPIOC.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM8_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��GPIO
	
	//���ø�����Ϊ�����������,���TIM1 CH3N ��PWM���岨��	GPIOB.15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //TIM1_CH3N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIO

//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
	//���ø�����Ϊ�����������,���TIM2 CH4 ��PWM���岨��	GPIOB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM2_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIO
	
	//���ø�����Ϊ�����������,���TIM2 CH3 ��PWM���岨��	GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TIM2_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM1_CH3N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIO
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}

/*
***************************************************************************
*	�� �� ��: BSP_DCMotor_TIM1_Init
*	����˵��: ����DCMotor TIM1��ʼ������
*	��    ��: Period : �Զ���װֵ��Prescaler : ʱ��Ԥ��Ƶ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_DCMotor_TIM1_Init(uint16_t Period,uint16_t Prescaler)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʹ�ܶ�ʱ��1ʱ��
	
	TIM_TimeBaseStructure.TIM_Period = Period; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =Prescaler; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //���� ���� ����ֵ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	/* Channel 3 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ������Ƚ�״̬
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //ʹ�� ���� ���״̬
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ ռ��ʱ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //���� ������Ը�
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //MOE=0 ���� TIM1����ȽϿ���״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; //MOE=0 ���� TIM1����ȽϿ���״̬

	TIM_OC3Init(TIM1, &TIM_OCInitStructure); //�趨�õĲ��� ��ʼ��TIM
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); //�趨�õĲ��� ��ʼ��TIM
	
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE); 
	
	TIM_Cmd(TIM1, ENABLE);
}
/*
***************************************************************************
*	�� �� ��: BSP_DCMotor_TIM8_Init
*	����˵��: ����DCMotor TIM8��ʼ������
*	��    ��: Period : �Զ���װֵ��Prescaler : ʱ��Ԥ��Ƶ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_DCMotor_TIM8_Init(uint16_t Period,uint16_t Prescaler)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //ʹ�ܶ�ʱ��8ʱ��
	
	TIM_TimeBaseStructure.TIM_Period = Period; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =Prescaler; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //���� ���� ����ֵ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	/* Channel 1 2 3 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ������Ƚ�״̬
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //ʹ�� ���� ���״̬
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ ռ��ʱ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //���� ������Ը�
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //MOE=0 ���� TIM8����ȽϿ���״̬
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; //MOE=0 ���� TIM8����ȽϿ���״̬
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); //�趨�õĲ��� ��ʼ��TIM
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); //�趨�õĲ��� ��ʼ��TIM
	TIM_OC3Init(TIM8, &TIM_OCInitStructure); //�趨�õĲ��� ��ʼ��TIM

	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM8, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE); 
	
	TIM_Cmd(TIM8, ENABLE);
}
/*

***************************************************************************
*	�� �� ��: BSP_DCMotor_Go
*	����˵��: ����DCMotorת�ٹ��ܺ���
*	��    ��: DCMotorValue : ת��ֵ��0-1000
*	�� �� ֵ: ��
***************************************************************************
*/

void BSP_DCMotor1_Go(uint16_t DCMotorValue)
{
	DCMotorValue += DCMOTOR_DEAD_VAL;
	if(DCMotorValue > DCMOTOR_MAX_VAL)
		DCMotorValue = DCMOTOR_MAX_VAL;
//	TIM_SetCompare3(TIM8,DCMotorValue);		
	TIM_SetCompare3(TIM8,DCMOTOR_MAX_VAL-DCMotorValue);		
}

void BSP_DCMotor2_Go(uint16_t DCMotorValue)
{
	DCMotorValue += DCMOTOR_DEAD_VAL;
	if(DCMotorValue > DCMOTOR_MAX_VAL)
		DCMotorValue = DCMOTOR_MAX_VAL;
//	TIM_SetCompare3(TIM3,DCMotorValue);		
	TIM_SetCompare2(TIM8,DCMOTOR_MAX_VAL-DCMotorValue);		
}
void BSP_DCMotor3_Go(uint16_t DCMotorValue)
{
	DCMotorValue += DCMOTOR_DEAD_VAL;
	if(DCMotorValue > DCMOTOR_MAX_VAL)
		DCMotorValue = DCMOTOR_MAX_VAL;
//	TIM_SetCompare1(TIM8,DCMotorValue);		
	TIM_SetCompare1(TIM8,DCMOTOR_MAX_VAL-DCMotorValue);		
}
void BSP_DCMotor4_Go(uint16_t DCMotorValue)
{
	DCMotorValue += DCMOTOR_DEAD_VAL;
	if(DCMotorValue > DCMOTOR_MAX_VAL)
		DCMotorValue = DCMOTOR_MAX_VAL;
//	TIM_SetCompare3(TIM1,DCMotorValue);		
	TIM_SetCompare3(TIM1,DCMOTOR_MAX_VAL-DCMotorValue);		
}


/*
***************************************************************************
*	�� �� ��: BSP_DCMotor_Brake
*	����˵��: ����DCMotorɲ�����ܺ���
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/

void BSP_DCMotor1_Brake(void)
{
//	TIM_SetCompare3(TIM8,0);
	TIM_SetCompare3(TIM8,DCMOTOR_MAX_VAL);
}

void BSP_DCMotor2_Brake(void)
{
//	TIM_SetCompare3(TIM3,0);
	TIM_SetCompare2(TIM8,DCMOTOR_MAX_VAL);
}
void BSP_DCMotor3_Brake(void)
{
//	TIM_SetCompare1(TIM8,0);
	TIM_SetCompare1(TIM8,DCMOTOR_MAX_VAL);
}
void BSP_DCMotor4_Brake(void)
{
//	TIM_SetCompare3(TIM1,0);
	TIM_SetCompare3(TIM1,DCMOTOR_MAX_VAL);
}

