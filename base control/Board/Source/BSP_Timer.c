/*
***************************************************************************
*    模块：BSP_Timer 
*    描述：板级 Timer 功能模块驱动
		   使用 Timer2
*    作者：Huo
*    时间：2017.09.01
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_Timer.h"

/* 内部函数声明 */
static void BSP_Timer_NVIC_Init(void);
static void BSP_Timer_TIM6_Init(uint16_t usCount);

/*
***************************************************************************
*	函 数 名: BSP_Timer_Init
*	功能说明: 板载 Timer 初始化函数
*	形    参: usCount 定时us时常计数
*	返 回 值: 无
***************************************************************************
*/
void BSP_Timer_Init(uint16_t usCount)
{
	BSP_Timer_NVIC_Init();
	BSP_Timer_TIM6_Init(usCount);
}

/*
***************************************************************************
*	函 数 名: BSP_Timer_NVIC_Init
*	功能说明: 板载 Timer NVIC 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_Timer_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 中断优先级NVIC设置 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	/* 抢占优先级设置，优先级分组为 4 的情况下，抢占优先级可设置范围 0-15 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* 子优先级设置，优先级分组为 4 的情况下，子优先级无效，取数值 0 即可 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************************************
*	函 数 名: BSP_Timer_TIM6_Init
*	功能说明: 板载 Timer TIM6 初始化函数
*	形    参: usCount 定时us时常计数
*	返 回 值: 无
***************************************************************************
*/
static void BSP_Timer_TIM6_Init(uint16_t usCount)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能
	
	/* 定时器TIM6初始化 */
	TIM_TimeBaseStructure.TIM_Period = (usCount-1); //自动重装载寄存器值	
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE );//使能TIM2中断,允许更新中断

	TIM_Cmd(TIM6, ENABLE);  //使能TIM2
}
