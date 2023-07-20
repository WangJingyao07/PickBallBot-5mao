/*
***************************************************************************
*    模块：BSP_ADConverter 
*    描述：板级 ADConverter 功能模块驱动
		   ADC1CH11-PC1 ADC1CH13-PC3
*    作者：Huo
*    时间：2017.09.26
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_ADConverter.h"

/* 内部函数声明 */
static void BSP_ADConverter_GPIO_Init(void);
static void BSP_ADConverter_ADC_Init(void);

/*
***************************************************************************
*	函 数 名: BSP_ADConverter_Init
*	功能说明: 板级 ADConverter 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void BSP_ADConverter_Init(void)
{
	BSP_ADConverter_GPIO_Init();
	BSP_ADConverter_ADC_Init();
}

/*
***************************************************************************
*	函 数 名: BSP_ADConverter_GPIO_Init
*	功能说明: 板载ADConverter GPIO初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_ADConverter_GPIO_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );//使能ADC1通道时钟

	//PC1 PC3作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

/*
***************************************************************************
*	函 数 名: BSP_ADConverter_ADC_Init
*	功能说明: 板载ADConverter ADC初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_ADConverter_ADC_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );//使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//72M/6=12,ADC最大时间不能超过14M

	ADC_DeInit(ADC1);  //复位ADC1 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC工作模式:ADC1工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	ADC_StartCalibration(ADC1);	 //开启AD校准
 	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}				  

/*
***************************************************************************
*	函 数 名: BSP_ADConverter_Get
*	功能说明: 板载ADConverter Get功能函数
*	形    参: ADConverterChannel : 指示ADConverterChannel序号
*	返 回 值: ADConverterChannel 转换结果
***************************************************************************
*/
uint16_t BSP_ADConverter_Get(uint8_t ADConverterChannel)
{
 	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADConverterChannel, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
