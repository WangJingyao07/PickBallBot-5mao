#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "stm32f10x.h"
	 
void BSP_Delay_Init(void);
void BSP_Delay_us(uint32_t nus);
void BSP_Delay_ms(uint16_t nms);

#endif
