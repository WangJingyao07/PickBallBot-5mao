/*
***************************************************************************
*    ģ�飺BSP_BlueTooth 
*    �������弶 BlueTooth ����ģ������
          USART1:TX1-PA9 RX1-PA10 ֧��printf
*    ���ߣ�Huo
*    ʱ�䣺2018.01.31
*    �汾��UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_BlueTooth.h"
int Usart_Receive;
unsigned int Flag_Direction;  
/* �ڲ��������� */
static void BSP_BlueTooth_GPIO_Init(void);
static void BSP_BlueTooth_USART_Init(uint32_t BaudRate);
static void BSP_BlueTooth_NVIC_Init(void);


void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}

/*
***************************************************************************
*	�� �� ��: BSP_BlueTooth_Init
*	����˵��: ���� BlueTooth ��ʼ������
*	��    ��: BaudRate ������
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_BlueTooth_Init(uint32_t BaudRate)
{
	BSP_BlueTooth_GPIO_Init();
	BSP_BlueTooth_NVIC_Init();
	BSP_BlueTooth_USART_Init(BaudRate);
}

/*
***************************************************************************
*	�� �� ��: BSP_BlueTooth_GPIO_Init
*	����˵��: ���� BlueTooth GPIO ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_BlueTooth_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	
	
	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 		//PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��GPIOA.9

	//USART3_RX	  GPIOB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��GPIOA.10  
}

/*
***************************************************************************
*	�� �� ��: BSP_BlueTooth_USART_Init
*	����˵��: ���� BlueTooth USART ��ʼ������
*	��    ��: BaudRate ������
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_BlueTooth_USART_Init(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART1ʱ��

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;	//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = \
	                    USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = \
	                    USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); 	//��ʼ������1
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1 
}

/*
***************************************************************************
*	�� �� ��: BSP_BlueTooth_NVIC_Init
*	����˵��: ���� BlueTooth NVIC ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_BlueTooth_NVIC_Init(void)
{
 	NVIC_InitTypeDef NVIC_InitStructure;

	//USART1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}
int USART2_IRQHandler(void)
{	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
	{	  	      
		Usart_Receive=USART_ReceiveData(USART2);
  		if(Usart_Receive>=0x41&&Usart_Receive<=0x4B)  
		{	
			Flag_Direction=Usart_Receive-0x40;
		}
		else	if(Usart_Receive<=8)   
		{			
			Flag_Direction=Usart_Receive;
		}	
		else  Flag_Direction=0;
  }
return 0;	
}


