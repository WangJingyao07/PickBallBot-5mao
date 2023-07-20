/*
***************************************************************************
*    模块：BSP_DCMotor 
*    描述：板级 DCMotor 功能模块驱动
*		   TIM1_CH2N-PB14  TIM8_CH3N-PB1  TIM1_CH1-PA8  TIM3_CH3-PB0
*		   TIM8_CH1-PC6   TIM1_CH3N-PB15  TIM2_CH4-PB11  TIM2_CH3-PB10
*    作者：Huo
*    时间：2017.09.06
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_DCMotor.h"

/* 内部函数声明 */
static void BSP_DCMotor_GPIO_Init(void);
static void BSP_DCMotor_TIM1_Init(uint16_t Period,uint16_t Prescaler);
static void BSP_DCMotor_TIM8_Init(uint16_t Period,uint16_t Prescaler);


/*
***************************************************************************
*	函 数 名: BSP_DCMotor_Init
*	功能说明: 板载DCMotor初始化函数
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: BSP_DCMotor_GPIO_Init
*	功能说明: 板载DCMotor GPIO初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_DCMotor_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | \
	                  RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	//设置该引脚为复用输出功能,输出TIM1 CH2N 的PWM脉冲波形	GPIOB.14
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //TIM1_CH2N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIO
	
	//设置该引脚为复用输出功能,输出TIM8 CH3N 的PWM脉冲波形	GPIOB.1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM8_CH3N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIO
	
	//设置该引脚为复用输出功能,输出TIM1 CH1 的PWM脉冲波形	GPIOA.8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIO
	
	//设置该引脚为复用输出功能,输出TIM3 CH3 的PWM脉冲波形	GPIOB.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM3_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIO
	
	//设置该引脚为复用输出功能,输出TIM8 CH1 的PWM脉冲波形	GPIOC.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM8_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化GPIO
	
	//设置该引脚为复用输出功能,输出TIM1 CH3N 的PWM脉冲波形	GPIOB.15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //TIM1_CH3N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIO

//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
	//设置该引脚为复用输出功能,输出TIM2 CH4 的PWM脉冲波形	GPIOB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM2_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIO
	
	//设置该引脚为复用输出功能,输出TIM2 CH3 的PWM脉冲波形	GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TIM2_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM1_CH3N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIO
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}

/*
***************************************************************************
*	函 数 名: BSP_DCMotor_TIM1_Init
*	功能说明: 板载DCMotor TIM1初始化函数
*	形    参: Period : 自动重装值，Prescaler : 时钟预分频数
*	返 回 值: 无
***************************************************************************
*/
static void BSP_DCMotor_TIM1_Init(uint16_t Period,uint16_t Prescaler)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //使能定时器1时钟
	
	TIM_TimeBaseStructure.TIM_Period = Period; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =Prescaler; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //设置 周期 计数值
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	/* Channel 3 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能输出比较状态
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //使能 互补 输出状态
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值 占空时间
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //互补 输出极性高
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //MOE=0 设置 TIM1输出比较空闲状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; //MOE=0 重置 TIM1输出比较空闲状态

	TIM_OC3Init(TIM1, &TIM_OCInitStructure); //设定好的参数 初始化TIM
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); //设定好的参数 初始化TIM
	
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE); 
	
	TIM_Cmd(TIM1, ENABLE);
}
/*
***************************************************************************
*	函 数 名: BSP_DCMotor_TIM8_Init
*	功能说明: 板载DCMotor TIM8初始化函数
*	形    参: Period : 自动重装值，Prescaler : 时钟预分频数
*	返 回 值: 无
***************************************************************************
*/
static void BSP_DCMotor_TIM8_Init(uint16_t Period,uint16_t Prescaler)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //使能定时器8时钟
	
	TIM_TimeBaseStructure.TIM_Period = Period; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =Prescaler; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //设置 周期 计数值
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	/* Channel 1 2 3 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能输出比较状态
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //使能 互补 输出状态
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值 占空时间
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //互补 输出极性高
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //MOE=0 设置 TIM8输出比较空闲状态
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; //MOE=0 重置 TIM8输出比较空闲状态
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); //设定好的参数 初始化TIM
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); //设定好的参数 初始化TIM
	TIM_OC3Init(TIM8, &TIM_OCInitStructure); //设定好的参数 初始化TIM

	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM8, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE); 
	
	TIM_Cmd(TIM8, ENABLE);
}
/*

***************************************************************************
*	函 数 名: BSP_DCMotor_Go
*	功能说明: 板载DCMotor转速功能函数
*	形    参: DCMotorValue : 转速值，0-1000
*	返 回 值: 无
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
*	函 数 名: BSP_DCMotor_Brake
*	功能说明: 板载DCMotor刹车功能函数
*	形    参: 无
*	返 回 值: 无
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

