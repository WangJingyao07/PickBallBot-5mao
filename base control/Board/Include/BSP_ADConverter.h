#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f10x.h"

void BSP_ADConverter_Init(void);

uint16_t BSP_ADConverter_Get(uint8_t ADConverterChannel);
 
#endif 
