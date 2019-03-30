#include "dac.h"
#include "sys.h"
#include "math.h"
#define PI  3.14159
#define DAC_DHR12R1_ADDRESS     0x40007408
#define DAC_DHR12R2_ADDRESS		0x40007408-0x08+0x14

u16 sinTable1[tableSize];
u16 sinTable2[tableSize];

void sin_Generation1(double r)
{
	u32 n;
	for(n=0;n<tableSize;n++)
	{
		sinTable1[n] = (sin(2*PI*n/tableSize)+1)*2047*r;	
	}	
}
void sin_Generation2(double r,u32 position)
{
	u32 n;
	for(n=0;n<tableSize;n++)
	{
		sinTable2[n] = (sin(2*PI*(n+position)/tableSize)+1)*2047*r;	
	}	
}

void TIM6_Configuration(void) //PA4 channel 1
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);	

 	TIM_PrescalerConfig(TIM6, 1-1, TIM_PSCReloadMode_Update);
  	TIM_SetAutoreload(TIM6, 50-1);
  	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
	TIM_Cmd(TIM6, ENABLE);
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DAC_DMA_Configuration1(void)
{
	DAC_InitTypeDef  DAC_InitStructure1;	

	DMA_InitTypeDef DMA_InitStructure1;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC_InitStructure1.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStructure1.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure1.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure1);

	DMA_DeInit(DMA1_Stream5);
	DMA_InitStructure1.DMA_Channel = DMA_Channel_7;  
	DMA_InitStructure1.DMA_PeripheralBaseAddr = DAC_DHR12R1_ADDRESS;
	DMA_InitStructure1.DMA_Memory0BaseAddr = (uint32_t)&sinTable1;
	DMA_InitStructure1.DMA_BufferSize = tableSize;
	DMA_InitStructure1.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure1.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure1.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure1.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure1.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure1.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure1.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure1.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure1.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure1.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure1.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream5, &DMA_InitStructure1);    
	
	DMA_Cmd(DMA1_Stream5, ENABLE);
	
	DAC_Cmd(DAC_Channel_1, ENABLE);
	
	
} 

void TIM7_Configuration(void) //PA5 channel 2
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);	

 	TIM_PrescalerConfig(TIM7, 1-1, TIM_PSCReloadMode_Update);
  	TIM_SetAutoreload(TIM7, 50-1);
  	TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);
	TIM_Cmd(TIM7, ENABLE);
}

void DAC_DMA_Configuration2(void)
{
	DAC_InitTypeDef  DAC_InitStructure2;	

	DMA_InitTypeDef DMA_InitStructure2;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC_InitStructure2.DAC_Trigger = DAC_Trigger_T7_TRGO;
	DAC_InitStructure2.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure2.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_2, &DAC_InitStructure2);

	DMA_DeInit(DMA1_Stream6);
	DMA_InitStructure2.DMA_Channel = DMA_Channel_7;  
	DMA_InitStructure2.DMA_PeripheralBaseAddr = DAC_DHR12R2_ADDRESS;
	DMA_InitStructure2.DMA_Memory0BaseAddr = (uint32_t)&sinTable2;
	DMA_InitStructure2.DMA_BufferSize = tableSize;
	DMA_InitStructure2.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure2.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure2.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure2.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure2.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure2.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure2.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure2.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure2.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure2.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure2.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream6, &DMA_InitStructure2);    
	
	DMA_Cmd(DMA1_Stream6, ENABLE);
	
	DAC_Cmd(DAC_Channel_2, ENABLE);
	
	
} 
