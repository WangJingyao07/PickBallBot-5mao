#ifndef __BSP_DEBUGUART_H
#define __BSP_DEBUGUART_H

#include <stdio.h>
#include "stm32f10x.h"

void BSP_DebugUART_Init(uint32_t BaudRate);

void ISRprintf(char* string,uint8_t num);

void Send_data(USART_TypeDef * USARTx,u8 *s);

void Usart_Text_Data(char Data[], int N, float x,float y,float z);
#endif
