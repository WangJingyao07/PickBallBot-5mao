/*
***************************************************************************
*    ģ�飺BSP_DebugUART 
*    �������弶 DebugUART ����ģ������
          USART1:TX1-PA9 RX1-PA10 ֧��printf
*    ���ߣ�Huo
*    ʱ�䣺2017.09.01
*    �汾��UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_DebugUART.h"

/* �ڲ��������� */
static void BSP_DebugUART_GPIO_Init(void);
static void BSP_DebugUART_USART_Init(uint32_t BaudRate);
static void BSP_DebugUART_NVIC_Init(void);

typedef union
{
float floatData;
char byteData[4];  //���ǰ�����ĳ�ʲô��
}FLOAT_BYTE;

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting) //���ð�����ģʽ        
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc������ʹ��USART1��USART1�����ʼ���ſ��ã� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR & USART_SR_TC)==0) {};//����Ƿ������
	USART1->DR = (ch & (uint16_t)0x01FF);	//��������
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	

	return ch;
}
int GetKey (void)  
{ 
	while (!(USART1->SR & USART_FLAG_RXNE));

	return ((int)(USART1->DR & 0x1FF));
}
*/

/*
***************************************************************************
*	�� �� ��: BSP_DebugUART_Init
*	����˵��: ���� DebugUART ��ʼ������
*	��    ��: BaudRate ������
*	�� �� ֵ: ��
***************************************************************************
*/
void BSP_DebugUART_Init(uint32_t BaudRate)
{
	BSP_DebugUART_GPIO_Init();
	BSP_DebugUART_NVIC_Init();
	BSP_DebugUART_USART_Init(BaudRate);
}

/*
***************************************************************************
*	�� �� ��: BSP_DebugUART_GPIO_Init
*	����˵��: ���� DebugUART GPIO ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_DebugUART_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 		//PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��GPIOA.10  
}

/*
***************************************************************************
*	�� �� ��: BSP_DebugUART_USART_Init
*	����˵��: ���� DebugUART USART ��ʼ������
*	��    ��: BaudRate ������
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_DebugUART_USART_Init(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;	//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = \
	                    USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = \
	                    USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); 	//��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

/*
***************************************************************************
*	�� �� ��: BSP_DebugUART_NVIC_Init
*	����˵��: ���� DebugUART NVIC ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
***************************************************************************
*/
static void BSP_DebugUART_NVIC_Init(void)
{
 	NVIC_InitTypeDef NVIC_InitStructure;

	//USART1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	/* ��ռ���ȼ����ã����ȼ�����Ϊ 4 ������£���ռ���ȼ������÷�Χ 0-15 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
	/* �����ȼ����ã����ȼ�����Ϊ 4 ������£������ȼ���Ч��ȡ��ֵ 0 ���� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

/*
***************************************************************************
*	�� �� ��: ISRprintf
*	����˵��: ���� �ж�printf ���ܺ���
*	��    ��: string �ַ�����num �ַ���������Ϊ�ַ���ʱΪ 0��
*	�� �� ֵ: ��
***************************************************************************
*/
//void ISRprintf(char* string,uint8_t num)
//{
//	uint8_t count = 0;
//	
//	if(num)
//		while(count < num)
//		{
//			while((USART1->SR & USART_FLAG_TXE)==0) {};	//����Ƿ������
//			USART_ClearFlag(USART1, USART_FLAG_TXE);
////			while((USART1->SR & USART_SR_TC)==0) {};	//����Ƿ������   
//			USART1->DR = (*string++ & (uint16_t)0x01FF);//��������
////			USART_SendData(USART1,(uint16_t)*string++);
//			count++;
//		}
//	else
//		while(*string)
//		{
//			while((USART1->SR & USART_FLAG_TXE)==0) {};	//����Ƿ������
//			USART_ClearFlag(USART1, USART_FLAG_TXE);
////			while((USART1->SR & USART_SR_TC)==0) {};	//����Ƿ������   
//			USART1->DR = (*string++ & (uint16_t)0x01FF);//��������
////			USART_SendData(USART1,(uint16_t)*string++);
//		}
//}
float bit_to_float(char *bit,int n)
{
	uint16_t i;
	FLOAT_BYTE testData;
	for(i=0;i<4;i++)
		testData.byteData[i]=bit[n+i];
	return testData.floatData;
}

//float Usart_Text_Data(char Data[], int N)
//{
//	float x,y,z;
//	if(Data[0]=='<' && Data[N] == '>')
//	{
//		//���յ������ݽ��м���
//		x = bit_to_float(Data,1);
//		y = bit_to_float(Data,5);	
//		z = bit_to_float(Data,9);
//	}

//}
      	  
//void Send_data(USART_TypeDef * USARTx,u8 *s)
//{
//	while(*s!='\0')
//	{ 
//		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC )==RESET);	
//		USART_SendData(USARTx,*s);
//		s++;
//	}
//}
