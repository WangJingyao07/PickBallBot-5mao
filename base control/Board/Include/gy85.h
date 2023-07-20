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
extern void	read_hmc5883l(void);//��ȡ������
extern void	read_ADXL345(void);//��ȡ������ٶȼ�
extern void	READ_ITG3205(void);	 //��ȡ��������������
#endif
