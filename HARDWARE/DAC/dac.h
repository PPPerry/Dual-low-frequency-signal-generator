#ifndef __DAC_H
#define __DAC_H
#include "sys.h"

#define tableSize 256
void sin_Generation1(double r);
void sin_Generation2(double r,u32 position);
void GPIO_Configuration(void);
void TIM6_Configuration(void);
void TIM7_Configuration(void);
void DAC_DMA_Configuration1(void);
void DAC_DMA_Configuration2(void);


#endif
