/*
***************************************************************************
*    模块：BSP_Delay 
*    描述：板级 延时功能模块驱动
*    作者：Huo
*    时间：2017.09.01
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/

#include "BSP_Delay.h"

static uint8_t  fac_us=0;		//us延时倍乘数			   
static uint16_t fac_ms=0;		//ms延时倍乘数

/*
***************************************************************************
*	函 数 名: BSP_Delay_Init
*	功能说明: 板载 Delay 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
 void BSP_Delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//选择时钟 HCLK/8
	fac_us = SystemCoreClock/8000000;	//为系统时钟的1/8  
	fac_ms = (uint16_t)fac_us*1000;		//代表每个ms需要的systick时钟数   
}								    
/*
***************************************************************************
*	函 数 名: BSP_Delay_us
*	功能说明: 板载 Delay 微秒功能函数
*	形    参: nus 要延时的us数
*	返 回 值: 无
***************************************************************************
*/
void BSP_Delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					//清空计数器	 
}

/*
***************************************************************************
*	函 数 名: BSP_Delay_ms
*	功能说明: 板载 Delay 毫秒功能函数
*	形    参: nms 要延时的ms数，nms<=1864
*	返 回 值: 无
***************************************************************************
*/

void BSP_Delay_ms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)nms*fac_ms;		//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;							//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;       					//清空计数器	  	    
} 
