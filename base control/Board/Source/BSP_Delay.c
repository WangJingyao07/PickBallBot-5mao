/*
***************************************************************************
*    ģ�飺BSP_Delay 
*    �������弶 ��ʱ����ģ������
*    ���ߣ�Huo
*    ʱ�䣺2017.09.01
*    �汾��UP-Magic-Version 1.0.0
***************************************************************************
*/

#include "BSP_Delay.h"

static uint8_t  fac_us=0;		//us��ʱ������			   
static uint16_t fac_ms=0;		//ms��ʱ������

/*
***************************************************************************
*	�� �� ��: BSP_Delay_Init
*	����˵��: ���� Delay ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
 void BSP_Delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//ѡ��ʱ�� HCLK/8
	fac_us = SystemCoreClock/8000000;	//Ϊϵͳʱ�ӵ�1/8  
	fac_ms = (uint16_t)fac_us*1000;		//����ÿ��ms��Ҫ��systickʱ����   
}								    
/*
***************************************************************************
*	�� �� ��: BSP_Delay_us
*	����˵��: ���� Delay ΢�빦�ܺ���
*	��    ��: nus Ҫ��ʱ��us��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_Delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					//��ռ�����	 
}

/*
***************************************************************************
*	�� �� ��: BSP_Delay_ms
*	����˵��: ���� Delay ���빦�ܺ���
*	��    ��: nms Ҫ��ʱ��ms����nms<=1864
*	�� �� ֵ: ��
***************************************************************************
*/

void BSP_Delay_ms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)nms*fac_ms;		//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;       					//��ռ�����	  	    
} 
