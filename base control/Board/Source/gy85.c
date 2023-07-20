
/*******************************************************************************
*  函数功能：串口发送数据  HMC5883L	+ ADXL345 + BMP085+L3G4200D
*  芯片：STM32F103XXXX
*  波特率：115200
*  编译环境：Keil uVision4
*  编写时间：
*  作    者：
*******************************************************************************/
#include "misc.h"

#include "gy85.h"

typedef int bool;
#define TRUE 1
#define FALSE 0
#include  <math.h>    //Keil library  

GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;


#define   uchar unsigned char
#define   uint unsigned int	
#define   OSS 0	 //	BMP085使用

//L3G4200D内部寄存器
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

//定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
#define	HMC5883L_Addr   0x3C	//磁场传感器器件地址
#define	ADXL345_Addr    0xA6	//加速度传感器器件地址
#define	BMP085_Addr     0xee	//气压传感器器件地址
#define	L3G4200_Addr    0xD2	//陀螺仪传感器器件地址


#define WHO	    0x00
#define	SMPL	0x15
#define DLPF	0x16
#define INT_C	0x17
#define INT_S	0x1A
#define	TMP_H	0x1B
#define	TMP_L	0x1C
#define	GX_H	0x1D
#define	GX_L	0x1E
#define	GY_H	0x1F
#define	GY_L	0x20
#define GZ_H	0x21
#define GZ_L	0x22
#define PWR_M	0x3E
//****************************

#define	ITG3205_Addr   0xD0	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改

unsigned char BUF[8];                         //接收数据缓存区
char  test=0; 
int   hx,hy;
float angle;
uchar ge,shi,bai,qian,wan,shiwan;           //显示变量
int A_X,A_Y,A_Z;
short T_X,T_Y,T_Z,T_T;

unsigned char TX_DATA[4];  	 //显示据缓存区

short data_xyz[3];
float Roll,Pitch,Q,T,K;


//***BMP085使用
short ac1;
short ac2; 
short ac3; 
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1; 
short b2;
short mb;
short mc;
short md;

long  temperature;
long  pressure;
//********
 //************************************++++++++++++++++++++++++++++++++
/*模拟IIC端口输出输入定义*/
#define SCL_H         GPIOA->BSRR = GPIO_Pin_5
#define SCL_L         GPIOA->BRR  = GPIO_Pin_5 
   
#define SDA_H         GPIOA->BSRR = GPIO_Pin_2
#define SDA_L         GPIOA->BRR  = GPIO_Pin_2

#define SCL_read      GPIOA->IDR  & GPIO_Pin_5
#define SDA_read      GPIOA->IDR  & GPIO_Pin_2

/* 函数申明 -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void USART1_Configuration(void);
void USART2_Configuration(void);
void USART3_Configuration(void);
void USART4_Configuration(void);
void USART5_Configuration(void);
void WWDG_Configuration(void);
void Delay(u32 nTime);
void Delayms(vu32 m);  
void conversion(long temp_data);
void adxl345_angle(void);
/* 变量定义 ----------------------------------------------*/

//***************************************************
void conversion(long temp_data)  
{     
    shiwan=temp_data/100000+0x30 ;
    temp_data=temp_data%100000;   //取余运算 
    wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //取余运算
	qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //取余运算
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //取余运算
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //取余运算
    ge=temp_data+0x30; 	
}
/*******************************************************************************
* Function Name  : I2C_GPIO_Config
* Description    : Configration Simulation IIC GPIO
* Input          : None 
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : I2C_delay
* Description    : Simulation IIC Timing series delay
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_delay(void)
{
		
   u8 i=30; //这里可以优化速度	，经测试最低到5还能写入
   while(i) 
   { 
     i--; 
   }  
}

void delay5ms(void)
{
		
   int i=5000;  
   while(i) 
   { 
     i--; 
   }  
}
/*******************************************************************************
* Function Name  : I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
bool I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return FALSE;	//SDA线为低电平则总线忙,退出
	SDA_L;
	I2C_delay();
	if(SDA_read) return FALSE;	//SDA线为高电平则总线出错,退出
	SDA_L;
	I2C_delay();
	return TRUE;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
} 
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}   
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
bool I2C_WaitAck(void) 	 //返回为:=1有ACK,=0无ACK
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
	  I2C_delay();
      return FALSE;
	}
	SCL_L;
	I2C_delay();
	return TRUE;
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_SendByte(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
		SCL_H;
        I2C_delay();
    }
    SCL_L;
}  
/*******************************************************************************
* Function Name  : I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
unsigned char I2C_RadeByte(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
} 
//ZRX          
//单字节写入*******************************************

bool Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address );   //设置低起始地址      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    delay5ms();
    return TRUE;
}

//单字节读取*****************************************
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   unsigned char REG_data;     	
	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;}
    I2C_SendByte((u8) REG_Address);   //设置低起始地址      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

	REG_data= I2C_RadeByte();
    I2C_NoAck();
    I2C_Stop();
    //return TRUE;
	return REG_data;

}						      

/*
********************************************************************************
** 函数名称 ： RCC_Configuration(void)
** 函数功能 ： 时钟初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void RCC_Configuration(void)
{   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  } 
   /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO  , ENABLE);  
}

/*
********************************************************************************
** 函数名称 ： GPIO_Configuration(void)
** 函数功能 ： 端口初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE  );
   /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //	选中管脚9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 最高输出速率50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);				 // 选择A端口
    
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			  //选中管脚10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);				  //选择A端口

  /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //	选中管脚2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 最高输出速率50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);				 // 选择A端口
    
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			  //选中管脚3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);				  //选择A端口


  /* Configure USART3 Tx (PB.10) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //	选中管脚10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 最高输出速率50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);				 // 选择B端口
    
  /* Configure USART3 Rx (PB.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			  //选中管脚11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);				  //选择B端口

   /* Configure USART4 Tx (PC.10) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //	选中管脚10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 最高输出速率50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);				 // 选择C端口
    
  /* Configure USART4 Rx (PC.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			  //选中管脚11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);				  //选择C端口

   /* Configure USART5 Tx (PC.12) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //	选中管脚12
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 最高输出速率50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);				 // 选择C端口
    
  /* Configure USART5 Rx (PD.2) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			  //选中管脚2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //浮空输入
  GPIO_Init(GPIOD, &GPIO_InitStructure);				  //选择D端口


}

/*
********************************************************************************
** 函数名称 ： USART1_Configuration(void)
** 函数功能 ： 串口1初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void USART1_Configuration(void)
{

USART_InitTypeDef USART_InitStructure;
USART_ClockInitTypeDef  USART_ClockInitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 |RCC_APB2Periph_USART1, ENABLE  );

USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;			// 时钟低电平活动
USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;				// 时钟低电平
USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;				// 时钟第二个边沿进行数据捕获
USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;		// 最后一位数据的时钟脉冲不从SCLK输出
/* Configure the USART1 synchronous paramters */
USART_ClockInit(USART1, &USART_ClockInitStructure);					// 时钟参数初始化设置
																	 
USART_InitStructure.USART_BaudRate = 115200;						  // 波特率为：115200
USART_InitStructure.USART_WordLength = USART_WordLength_8b;			  // 8位数据
USART_InitStructure.USART_StopBits = USART_StopBits_1;				  // 在帧结尾传输1个停止位
USART_InitStructure.USART_Parity = USART_Parity_No ;				  // 奇偶失能
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件流控制失能

USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // 发送使能+接收使能
/* Configure USART1 basic and asynchronous paramters */
USART_Init(USART1, &USART_InitStructure);
    
  /* Enable USART1 */
USART_ClearFlag(USART1, USART_IT_RXNE); 			//清中断，以免一启用中断后立即产生中断
USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);		//使能USART1中断源
USART_Cmd(USART1, ENABLE);							//USART1总开关：开启 
}


/*
********************************************************************************
** 函数名称 ： NVIC_Configuration(void)
** 函数功能 ： 中断初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void NVIC_Configuration(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
 
  NVIC_InitStructure.NVIC_IRQChannel = 0;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_Init(&NVIC_InitStructure);

}

 /*
********************************************************************************
** 函数名称 ： WWDG_Configuration(void)
** 函数功能 ： 看门狗初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void WWDG_Configuration(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	
  WWDG_SetPrescaler(WWDG_Prescaler_8);	              //  WWDG clock counter = (PCLK1/4096)/8 = 244 Hz (~4 ms)  
  WWDG_SetWindowValue(0x41);		                 // Set Window value to 0x41
  WWDG_Enable(0x50);		       // Enable WWDG and set counter value to 0x7F, WWDG timeout = ~4 ms * 64 = 262 ms 
  WWDG_ClearFlag();			       // Clear EWI flag
  WWDG_EnableIT();			       // Enable EW interrupt
}

/*
********************************************************************************
** 函数名称 ： Delay(vu32 nCount)
** 函数功能 ： 延时函数
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
 void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

/*
********************************************************************************
** 函数名称 ： void Delayms(vu32 m)
** 函数功能 ： 长延时函数	 m=1,延时1ms
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
 void Delayms(vu32 m)
{
  u32 i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}



/*
********************************************************************************
** 函数名称 ： WWDG_IRQHandler(void)
** 函数功能 ： 窗口提前唤醒中断
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/ 

void WWDG_IRQHandler(void)
{
  /* Update WWDG counter */
  WWDG_SetCounter(0x50);
	
  /* Clear EWI flag */
  WWDG_ClearFlag();
  
}
 //************************************************
void  USART1_SendData(uchar SendData)
{
USART_SendData(USART1, SendData);
Delayms(1);
}
	

//********************************************************************
long bmp085ReadTemp(void)
{   short  temp_ut;
	Single_Write(BMP085_Addr,0xF4,0x2E);
	Delayms(5);	// max time is 4.5ms
	temp_ut = Single_Read(BMP085_Addr,0xF6);
	temp_ut = (temp_ut<<8)| Single_Read(BMP085_Addr,0xF7);		
	return (long) temp_ut ;
}
//*************************************************************

long bmp085ReadPressure(void)
{
	long pressure = 0;
	Single_Write(BMP085_Addr,0xF4,0x34);
	Delayms(5);	// max time is 4.5ms
	pressure = Single_Read(BMP085_Addr,0xF6);
	pressure = (pressure<<8)| Single_Read(BMP085_Addr,0xF7);		
	pressure &= 0x0000FFFF;	
	return pressure;	
}

 //******************
 void Send_ADXL345_data(int dis_data)
  { float temp ;
	if(dis_data>0x7fff)dis_data-=0xffff;
 	if(dis_data<0){
  	     dis_data=-dis_data;
	     USART_SendData(USART1,'-');
		 Delayms(2);
	}
	else
	  { 
	   USART_SendData(USART1,'+');
	   Delayms(2);
	   }
        temp=(float)dis_data*3.9;  //计算数据和显示,查考ADXL345快速入门第4页
        conversion(temp);          //转换出显示需要的数据	 
	    USART1_SendData(qian);
		USART1_SendData('.');
	    USART1_SendData(bai); 
	    USART1_SendData(shi); 
	    USART1_SendData(ge); 
		USART1_SendData('g'); 
}
//***************************************
 //******************
 void Send_L3G420D_data(short dis_data)
  { float temp ;
 	if(dis_data<0){
  	     dis_data=-dis_data;
	     USART_SendData(USART1,'-');
		 Delayms(2);
	}
	else
	  { 
	   USART_SendData(USART1,'+');
	   Delayms(2);
	   }
        temp=(float)dis_data*0.07;  //计算数据和显示,查考ADXL345快速入门第4页
        conversion(temp);           //转换出显示需要的数据	 
	    USART1_SendData(bai); 
	    USART1_SendData(shi); 
	    USART1_SendData(ge); 
 
}
//***************************************


void Send_HMC5883L()
{
USART1_SendData('H');
USART1_SendData('M');
USART1_SendData('C');
USART1_SendData('5');
USART1_SendData('8');
USART1_SendData('8');
USART1_SendData('3');
USART1_SendData('L');
USART1_SendData(':');
conversion(angle);
USART1_SendData(bai); 
USART1_SendData(shi); 
USART1_SendData(ge); 
USART1_SendData('`');  
USART1_SendData(0x0d);
USART1_SendData(0x0a);
}
//*************************************************
void  Send_BMP085()
{
USART1_SendData('B');
USART1_SendData('M');
USART1_SendData('P');
USART1_SendData('0');
USART1_SendData('8');
USART1_SendData('5');
USART1_SendData(':');

USART1_SendData('t');
USART1_SendData('=');
conversion(temperature);
USART1_SendData(bai); 
USART1_SendData(shi); 
USART1_SendData('.');
USART1_SendData(ge);
USART1_SendData('`'); 
USART1_SendData('C'); 

USART1_SendData(' ');
USART1_SendData('p');
USART1_SendData('=');
conversion(pressure);

USART1_SendData(shiwan);
USART1_SendData(wan);
USART1_SendData(qian);
USART1_SendData('.');
USART1_SendData(bai); 
USART1_SendData(shi); 
USART1_SendData(ge); 
USART1_SendData('K'); 
USART1_SendData('p'); 
USART1_SendData('a');  				
USART1_SendData(0x0d);
USART1_SendData(0x0a);

}
//*****************************************************

void  Send_ADXL345()
{
USART1_SendData('A');
USART1_SendData('D');
USART1_SendData('X');
USART1_SendData('L');
USART1_SendData('3');
USART1_SendData('4');
USART1_SendData('5');
USART1_SendData(':');
	   USART1_SendData('X');
	   USART1_SendData('=');
	   Send_ADXL345_data(A_X);

	   USART1_SendData('Y');
	   USART1_SendData('=');
	   Send_ADXL345_data(A_Y);

	   USART1_SendData('Z');
	   USART1_SendData('=');
	   Send_ADXL345_data(A_Z);

	   adxl345_angle();


USART1_SendData(0x0d);
USART1_SendData(0x0a);
}
 //*****************************************************
 void Send_L3G4200D()
 {
   	USART1_SendData('L');
	USART1_SendData('3');
	USART1_SendData('G');
	USART1_SendData('4');
	USART1_SendData('2');
	USART1_SendData('0');
	USART1_SendData('0');
	USART1_SendData('D');
	USART1_SendData(':');

	   USART1_SendData('X');
	   USART1_SendData('=');
	   Send_L3G420D_data(T_X);

	   USART1_SendData('Y');
	   USART1_SendData('=');
	   Send_L3G420D_data(T_Y);

	   USART1_SendData('Z');
	   USART1_SendData('=');
	   Send_L3G420D_data(T_Z);

	  USART1_SendData(0x0d);
     USART1_SendData(0x0a);
 }
//******************************************************
void read_hmc5883l(void)
{
       Single_Write(HMC5883L_Addr,0x00,0x14);   //
       Single_Write(HMC5883L_Addr,0x02,0x00);   //
  	   Delayms(10);

       BUF[1]=Single_Read(HMC5883L_Addr,0x03);//OUT_X_L_A
       BUF[2]=Single_Read(HMC5883L_Addr,0x04);//OUT_X_H_A

			BUF[3]=Single_Read(HMC5883L_Addr,0x07);//OUT_Y_L_A
       BUF[4]=Single_Read(HMC5883L_Addr,0x08);//OUT_Y_H_A

       hx=(BUF[1] << 8) | BUF[2]; //Combine MSB and LSB of X Data output register
       hy=(BUF[3] << 8) | BUF[4]; //Combine MSB and LSB of Z Data output register

       if(hx>0x7fff)hx-=0xffff;	  
       if(hy>0x7fff)hy-=0xffff;	  
       angle= atan2(hy,hx) * (180 / 3.14159265) + 180; // angle in degrees
}
//****************************************
void read_ADXL345(void)
{
       BUF[0]=Single_Read(ADXL345_Addr,0x32);//OUT_X_L_A
       BUF[1]=Single_Read(ADXL345_Addr,0x33);//OUT_X_H_A

	   BUF[2]=Single_Read(ADXL345_Addr,0x34);//OUT_Y_L_A
       BUF[3]=Single_Read(ADXL345_Addr,0x35);//OUT_Y_H_A

	   BUF[4]=Single_Read(ADXL345_Addr,0x36);//OUT_Z_L_A
       BUF[5]=Single_Read(ADXL345_Addr,0x37);//OUT_Z_H_A

	   A_X=(BUF[1]<<8)+BUF[0];  //合成数据  
	   A_Y=(BUF[3]<<8)+BUF[2];  //合成数据
	   A_Z=(BUF[5]<<8)+BUF[4];  //合成数据
}
//*****************************************
void read_BMP085(void)
{

   	long ut;
	long up;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;

	ut = bmp085ReadTemp();	   // 读取温度
//	ut = bmp085ReadTemp();	   // 读取温度
	up = bmp085ReadPressure();  // 读取压强
//	up = bmp085ReadPressure();  // 读取压强
	x1 = ((long)ut - ac6) * ac5 >> 15;
	x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	temperature = (b5 + 8) >> 4;
	
	 //****************

	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;
	b3 = (((long)ac1 * 4 + x3) + 2)/4;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) up - b3) * (50000 >> OSS);
	if( b7 < 0x80000000)
	     p = (b7 * 2) / b4 ;
           else  
		    p = (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	 pressure = p + ((x1 + x2 + 3791) >> 4);
}
//***************************
void  Init_ADXL345(void)
{
   Single_Write(ADXL345_Addr,0x31,0x0B);   //测量范围,正负16g，13位模式
  // Single_Write(ADXL345_Addr,0x2C,0x0e);   //速率设定为100hz 参考pdf13页
   Single_Write(ADXL345_Addr,0x2D,0x08);   //选择电源模式   参考pdf24页
   Single_Write(ADXL345_Addr,0x2E,0x80);   //使能 DATA_READY 中断
  // Single_Write(ADXL345_Addr,0x1E,0x00);   //X 偏移量 根据测试传感器的状态写入pdf29页
  // Single_Write(ADXL345_Addr,0x1F,0x00);   //Y 偏移量 根据测试传感器的状态写入pdf29页
  // Single_Write(ADXL345_Addr,0x20,0x05);   //Z 偏移量 根据测试传感器的状态写入pdf29页
}
//****************************
 void  Init_BMP085(void)
{
	ac1 = Single_Read(BMP085_Addr,0xAA);
	ac1 = (ac1<<8)|Single_Read(BMP085_Addr,0xAB);

    ac2 = Single_Read(BMP085_Addr,0xAC);
	ac2 = (ac2<<8)| Single_Read(BMP085_Addr,0xAD);

	ac3 = Single_Read(BMP085_Addr,0xAE);
	ac3 = (ac3<<8)| Single_Read(BMP085_Addr,0xAF);

	ac4 = Single_Read(BMP085_Addr,0xB0);
	ac4 = (ac4<<8)| Single_Read(BMP085_Addr,0xB1);

	ac5 = Single_Read(BMP085_Addr,0xB2);
	ac5 = (ac5<<8)| Single_Read(BMP085_Addr,0xB3);

	ac6 = Single_Read(BMP085_Addr,0xB4);
	ac6 = (ac6<<8)| Single_Read(BMP085_Addr,0xB5);

	b1 = Single_Read(BMP085_Addr,0xB6);
	b1 = (b1<<8)| Single_Read(BMP085_Addr,0xB7);

	b2 = Single_Read(BMP085_Addr,0xB8);
	b2 = (b2<<8)| Single_Read(BMP085_Addr,0xB9);

	mb = Single_Read(BMP085_Addr,0xBA);
	mb = (mb<<8)| Single_Read(BMP085_Addr,0xBB);

	mc = Single_Read(BMP085_Addr,0xBC);
	mc = (mc<<8)| Single_Read(BMP085_Addr,0xBD);

	md = Single_Read(BMP085_Addr,0xBE);
	md = (md<<8)| Single_Read(BMP085_Addr,0xBF);

}
//****************************
 void  Init_HMC5883L()
{
   Single_Write(HMC5883L_Addr,0x00,0x14);   //
   Single_Write(HMC5883L_Addr,0x02,0x00);   //
}
//*****************************************

 //************初始化L3G4200D*********************************
void Init_L3G4200D(void)
{
	Single_Write(L3G4200_Addr,CTRL_REG1, 0x0f);
	Single_Write(L3G4200_Addr,CTRL_REG2, 0x00);
	Single_Write(L3G4200_Addr,CTRL_REG3, 0x08);
	Single_Write(L3G4200_Addr,CTRL_REG4, 0x30);	//+-2000dps
	Single_Write(L3G4200_Addr,CTRL_REG5, 0x00);
}	
//******读取L3G4200D数据****************************************
void read_L3G4200D(void)
{
   BUF[0]=Single_Read(L3G4200_Addr,OUT_X_L);
   BUF[1]=Single_Read(L3G4200_Addr,OUT_X_H);
   T_X=	(BUF[1]<<8)|BUF[0];
 


   BUF[2]=Single_Read(L3G4200_Addr,OUT_Y_L);
   BUF[3]=Single_Read(L3G4200_Addr,OUT_Y_H);
   T_Y=	(BUF[3]<<8)|BUF[2];
  

   BUF[4]=Single_Read(L3G4200_Addr,OUT_Z_L);
   BUF[5]=Single_Read(L3G4200_Addr,OUT_Z_H);
   T_Z=	(BUF[5]<<8)|BUF[4];

}

 //******************ADXL345计算倾斜角度************
void adxl345_angle(void)
 {

data_xyz[0]=A_X;  //合成数据   
data_xyz[1]=A_Y;  //合成数据   
data_xyz[2]=A_Z;  //合成数据   

//分别是加速度X,Y,Z的原始数据，10位的
Q=(float)data_xyz[0]*3.9;
T=(float)data_xyz[1]*3.9;
K=(float)data_xyz[2]*3.9;
Q=-Q;

  Roll=(float)(((atan2(K,Q)*180)/3.14159265)+180);    //X轴角度值
  Pitch=(float)(((atan2(K,T)*180)/3.14159265)+180);  //Y轴角度值
  //conversion(Pitch);								   //需要显示时候请更换
  conversion(Roll);
        USART1_SendData(' '); 
		USART1_SendData(' '); 
  	    USART1_SendData(bai); 
	    USART1_SendData(shi); 
	    USART1_SendData(ge);
}
void Init_ITG3205(void)
{
   Single_Write(ITG3205_Addr,PWR_M, 0x80);   //
   Single_Write(ITG3205_Addr,SMPL, 0x07);    //
   Single_Write(ITG3205_Addr,DLPF, 0x1E);    //±2000°
   Single_Write(ITG3205_Addr,INT_C, 0x00 );  //
   Single_Write(ITG3205_Addr,PWR_M, 0x00);   //
}
void READ_ITG3205(void)
{
   BUF[0]=Single_Read(ITG3205_Addr,GX_L); 
   BUF[1]=Single_Read(ITG3205_Addr,GX_H);
   T_X=	(BUF[1]<<8)|BUF[0];
   T_X/=14.375; 						   //读取计算X轴数据

   BUF[2]=Single_Read(ITG3205_Addr,GY_L);
   BUF[3]=Single_Read(ITG3205_Addr,GY_H);
   T_Y=	(BUF[3]<<8)|BUF[2];
   T_Y/=14.375; 						   //读取计算Y轴数据
   BUF[4]=Single_Read(ITG3205_Addr,GZ_L);
   BUF[5]=Single_Read(ITG3205_Addr,GZ_H);
   T_Z=	(BUF[5]<<8)|BUF[4];
   T_Z/=14.375; 					       //读取计算Z轴数据

   BUF[6]=Single_Read(ITG3205_Addr,TMP_L); 
   BUF[7]=Single_Read(ITG3205_Addr,TMP_H); 
   T_T=(BUF[7]<<8)|BUF[6];
   T_T = 35+ ((double) (T_T + 13200)) / 280;// 读取计算出温度
}	
 void Send_data(uchar axis)
 {uchar i;
  USART1_SendData(axis);
  USART1_SendData(':');
  for(i=0;i<4;i++)USART1_SendData(TX_DATA[i]);
  USART1_SendData(' ');
  USART1_SendData(' ');
 }
 void DATA_printf(uchar *s,short temp_data)
{
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //取余运算
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //取余运算
    *++s =temp_data+0x30; 	
}
void Send_ITG3205()
{
	USART1_SendData('I');
	USART1_SendData('T');
	USART1_SendData('G');
	USART1_SendData('3');
	USART1_SendData('2');
	USART1_SendData('0');
	USART1_SendData('5');
	USART1_SendData(':');
  DATA_printf(TX_DATA,T_X);//转换X轴数据到数组
	Send_data('X');			 //发送X轴数
	DATA_printf(TX_DATA,T_Y);//转换Y轴数据到数组
	Send_data('Y');			 //发送Y轴数
	DATA_printf(TX_DATA,T_Z);//转换Z轴数据到数组
	Send_data('Z');			 //发送Z轴数
	DATA_printf(TX_DATA,T_T);//转换温度数据到数组
	Send_data('T');			 //发送温度数据
	USART1_SendData(0X0D);	 //换行
	USART1_SendData(0X0A);	 //回车
}

