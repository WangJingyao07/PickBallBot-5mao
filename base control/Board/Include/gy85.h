#ifndef _gy85_H
#define	_gy85_H
#include "stm32f10x.h"
void Send_HMC5883L(void);
void  Send_BMP085(void);
void  Send_ADXL345(void);
void Send_ITG3205(void);
void USART1_Configuration(void);
void RCC_Configuration(void);
extern void  GPIO_Configuration(void);
extern void  I2C_GPIO_Config(void);
extern void Init_HMC5883L(void);
extern void Init_ADXL345(void);
extern void Init_ITG3205(void);
extern void	read_hmc5883l(void);//读取磁力计
extern void	read_ADXL345(void);//读取三轴加速度计
extern void	READ_ITG3205(void);	 //读取三轴陀螺仪数据
#endif
