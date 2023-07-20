/*
***************************************************************************
*    ģ�飺BSP_ADConverter 
*    �������弶 ADConverter ����ģ������
		   ADC1CH11-PC1 ADC1CH13-PC3
*    ���ߣ�Huo
*    ʱ�䣺2017.09.26
*    �汾��UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_ADConverter.h"

/* �ڲ��������� */
static void BSP_ADConverter_GPIO_Init(void);
static void BSP_ADConverter_ADC_Init(void);

/*
***************************************************************************
*	�� �� ��: BSP_ADConverter_Init
*	����˵��: �弶 ADConverter ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_ADConverter_Init(void)
{
	BSP_ADConverter_GPIO_Init();
	BSP_ADConverter_ADC_Init();
}

/*
***************************************************************************
*	�� �� ��: BSP_ADConverter_GPIO_Init
*	����˵��: ����ADConverter GPIO��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_ADConverter_GPIO_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );//ʹ��ADC1ͨ��ʱ��

	//PC1 PC3��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

/*
***************************************************************************
*	�� �� ��: BSP_ADConverter_ADC_Init
*	����˵��: ����ADConverter ADC��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_ADConverter_ADC_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );//ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//72M/6=12,ADC���ʱ�䲻�ܳ���14M

	ADC_DeInit(ADC1);  //��λADC1 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC����ģʽ:ADC1�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}				  

/*
***************************************************************************
*	�� �� ��: BSP_ADConverter_Get
*	����˵��: ����ADConverter Get���ܺ���
*	��    ��: ADConverterChannel : ָʾADConverterChannel���
*	�� �� ֵ: ADConverterChannel ת�����
***************************************************************************
*/
uint16_t BSP_ADConverter_Get(uint8_t ADConverterChannel)
{
 	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ADConverterChannel, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}
