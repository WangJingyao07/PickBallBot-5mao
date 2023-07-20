/*
***************************************************************************
*    ģ�飺BSP_Timer 
*    �������弶 Timer ����ģ������
		   ʹ�� Timer2
*    ���ߣ�Huo
*    ʱ�䣺2017.09.01
*    �汾��UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_Timer.h"

/* �ڲ��������� */
static void BSP_Timer_NVIC_Init(void);
static void BSP_Timer_TIM6_Init(uint16_t usCount);

/*
***************************************************************************
*	�� �� ��: BSP_Timer_Init
*	����˵��: ���� Timer ��ʼ������
*	��    ��: usCount ��ʱusʱ������
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_Timer_Init(uint16_t usCount)
{
	BSP_Timer_NVIC_Init();
	BSP_Timer_TIM6_Init(usCount);
}

/*
***************************************************************************
*	�� �� ��: BSP_Timer_NVIC_Init
*	����˵��: ���� Timer NVIC ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_Timer_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* �ж����ȼ�NVIC���� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	/* ��ռ���ȼ����ã����ȼ�����Ϊ 4 ������£���ռ���ȼ������÷�Χ 0-15 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* �����ȼ����ã����ȼ�����Ϊ 4 ������£������ȼ���Ч��ȡ��ֵ 0 ���� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
***************************************************************************
*	�� �� ��: BSP_Timer_TIM6_Init
*	����˵��: ���� Timer TIM6 ��ʼ������
*	��    ��: usCount ��ʱusʱ������
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_Timer_TIM6_Init(uint16_t usCount)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��
	
	/* ��ʱ��TIM6��ʼ�� */
	TIM_TimeBaseStructure.TIM_Period = (usCount-1); //�Զ���װ�ؼĴ���ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE );//ʹ��TIM2�ж�,��������ж�

	TIM_Cmd(TIM6, ENABLE);  //ʹ��TIM2
}
