/*
***************************************************************************
*    模块：BSP_Key 
*    描述：板级 Key 功能模块驱动
		   Key1-PG13 Key2-PF9 Key3-PF11 Key4-PG15 按下返回低电平
*    作者：Huo
*    时间：2017.09.01
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_Key.h"

/* 内部函数声明 */
static void BSP_Key_GPIO_Init(void);

/*
***************************************************************************
*	函 数 名: BSP_Key_Init
*	功能说明: 板级 Key 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_Key_Init(void)
{
	BSP_Key_GPIO_Init();
}

/*
***************************************************************************
*	函 数 名: BSP_Key_GPIO_Init
*	功能说明: 板级 Key GPIO 内部初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_Key_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_Key1 | RCC_Key2 | \
                   RCC_Key3 | RCC_Key4, ENABLE);	//使能对应时钟
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_Key1;
	GPIO_Init(GPIO_PORT_Key1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_Key2;
	GPIO_Init(GPIO_PORT_Key2, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_Key3;
	GPIO_Init(GPIO_PORT_Key3, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_Key4;
	GPIO_Init(GPIO_PORT_Key4, &GPIO_InitStructure);
}

/*
***************************************************************************
*	函 数 名: BSP_Key_Scan
*	功能说明: 板载Key扫描功能函数
*	形    参: KeyCount : 按键序号
*	返 回 值: KeyState : 按键状态
***************************************************************************
*/
uint8_t BSP_Key_Scan(uint8_t KeyCount)
{
	uint8_t KeyState=0x00;
	
	if(KeyCount & Key1)
		if(!GPIO_ReadInputDataBit(GPIO_PORT_Key1,GPIO_PIN_Key1))
			KeyState |= Key1;
	if(KeyCount & Key2)
		if(!GPIO_ReadInputDataBit(GPIO_PORT_Key2,GPIO_PIN_Key2))
			KeyState |= Key2;
	if(KeyCount & Key3)
		if(!GPIO_ReadInputDataBit(GPIO_PORT_Key3,GPIO_PIN_Key3))
			KeyState |= Key3;
	if(KeyCount & Key4)
		if(!GPIO_ReadInputDataBit(GPIO_PORT_Key4,GPIO_PIN_Key4))
			KeyState |= Key4;
	
	return KeyState;
}
