/*
***************************************************************************
*    模块：BSP_BlueTooth 
*    描述：板级 BlueTooth 功能模块驱动
          USART1:TX1-PA9 RX1-PA10 支持printf
*    作者：Huo
*    时间：2018.01.31
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_BlueTooth.h"
int Usart_Receive;
unsigned int Flag_Direction;  
/* 内部函数声明 */
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
*	函 数 名: BSP_BlueTooth_Init
*	功能说明: 板载 BlueTooth 初始化函数
*	形    参: BaudRate 波特率
*	返 回 值: 无
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
*	函 数 名: BSP_BlueTooth_GPIO_Init
*	功能说明: 板载 BlueTooth GPIO 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_BlueTooth_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
	
	
	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 		//PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIOA.9

	//USART3_RX	  GPIOB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIOA.10  
}

/*
***************************************************************************
*	函 数 名: BSP_BlueTooth_USART_Init
*	功能说明: 板载 BlueTooth USART 初始化函数
*	形    参: BaudRate 波特率
*	返 回 值: 无
***************************************************************************
*/
static void BSP_BlueTooth_USART_Init(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;	//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = \
	                    USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = \
	                    USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure); 	//初始化串口1
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART2, ENABLE);                    //使能串口1 
}

/*
***************************************************************************
*	函 数 名: BSP_BlueTooth_NVIC_Init
*	功能说明: 板载 BlueTooth NVIC 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_BlueTooth_NVIC_Init(void)
{
 	NVIC_InitTypeDef NVIC_InitStructure;

	//USART1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}
int USART2_IRQHandler(void)
{	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
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


