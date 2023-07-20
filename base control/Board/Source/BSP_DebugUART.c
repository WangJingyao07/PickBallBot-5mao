/*
***************************************************************************
*    模块：BSP_DebugUART 
*    描述：板级 DebugUART 功能模块驱动
          USART1:TX1-PA9 RX1-PA10 支持printf
*    作者：Huo
*    时间：2017.09.01
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
#include "BSP_DebugUART.h"

/* 内部函数声明 */
static void BSP_DebugUART_GPIO_Init(void);
static void BSP_DebugUART_USART_Init(uint32_t BaudRate);
static void BSP_DebugUART_NVIC_Init(void);

typedef union
{
float floatData;
char byteData[4];  //就是把这个改成什么样
}FLOAT_BYTE;

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting) //禁用半主机模式        
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数，使用USART1，USART1必须初始化才可用！ 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR & USART_SR_TC)==0) {};//检测是否发送完毕
	USART1->DR = (ch & (uint16_t)0x01FF);	//发送数据
	return ch;
}
#endif 

/*使用microLib的方法*/
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
*	函 数 名: BSP_DebugUART_Init
*	功能说明: 板载 DebugUART 初始化函数
*	形    参: BaudRate 波特率
*	返 回 值: 无
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
*	函 数 名: BSP_DebugUART_GPIO_Init
*	功能说明: 板载 DebugUART GPIO 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_DebugUART_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
	
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 		//PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIOA.9

	//USART1_RX	  GPIOA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIOA.10  
}

/*
***************************************************************************
*	函 数 名: BSP_DebugUART_USART_Init
*	功能说明: 板载 DebugUART USART 初始化函数
*	形    参: BaudRate 波特率
*	返 回 值: 无
***************************************************************************
*/
static void BSP_DebugUART_USART_Init(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;	//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = \
	                    USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = \
	                    USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); 	//初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

/*
***************************************************************************
*	函 数 名: BSP_DebugUART_NVIC_Init
*	功能说明: 板载 DebugUART NVIC 初始化函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
static void BSP_DebugUART_NVIC_Init(void)
{
 	NVIC_InitTypeDef NVIC_InitStructure;

	//USART1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	/* 抢占优先级设置，优先级分组为 4 的情况下，抢占优先级可设置范围 0-15 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
	/* 子优先级设置，优先级分组为 4 的情况下，子优先级无效，取数值 0 即可 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

/*
***************************************************************************
*	函 数 名: ISRprintf
*	功能说明: 板载 中断printf 功能函数
*	形    参: string 字符串，num 字符个数（当为字符串时为 0）
*	返 回 值: 无
***************************************************************************
*/
//void ISRprintf(char* string,uint8_t num)
//{
//	uint8_t count = 0;
//	
//	if(num)
//		while(count < num)
//		{
//			while((USART1->SR & USART_FLAG_TXE)==0) {};	//检测是否发送完毕
//			USART_ClearFlag(USART1, USART_FLAG_TXE);
////			while((USART1->SR & USART_SR_TC)==0) {};	//检测是否发送完毕   
//			USART1->DR = (*string++ & (uint16_t)0x01FF);//发送数据
////			USART_SendData(USART1,(uint16_t)*string++);
//			count++;
//		}
//	else
//		while(*string)
//		{
//			while((USART1->SR & USART_FLAG_TXE)==0) {};	//检测是否发送完毕
//			USART_ClearFlag(USART1, USART_FLAG_TXE);
////			while((USART1->SR & USART_SR_TC)==0) {};	//检测是否发送完毕   
//			USART1->DR = (*string++ & (uint16_t)0x01FF);//发送数据
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
//		//对收到的数据进行计算
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
