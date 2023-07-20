/*
***************************************************************************
*    ģ�飺BSP_Key 
*    �������弶 Key ����ģ������
		   Key1-PG13 Key2-PF9 Key3-PF11 Key4-PG15 ���·��ص͵�ƽ
*    ���ߣ�Huo
*    ʱ�䣺2017.09.01
*    �汾��UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_Key.h"

/* �ڲ��������� */
static void BSP_Key_GPIO_Init(void);

/*
***************************************************************************
*	�� �� ��: BSP_Key_Init
*	����˵��: �弶 Key ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_Key_Init(void)
{
	BSP_Key_GPIO_Init();
}

/*
***************************************************************************
*	�� �� ��: BSP_Key_GPIO_Init
*	����˵��: �弶 Key GPIO �ڲ���ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_Key_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_Key1 | RCC_Key2 | \
                   RCC_Key3 | RCC_Key4, ENABLE);	//ʹ�ܶ�Ӧʱ��
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//��������

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
*	�� �� ��: BSP_Key_Scan
*	����˵��: ����Keyɨ�蹦�ܺ���
*	��    ��: KeyCount : �������
*	�� �� ֵ: KeyState : ����״̬
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
