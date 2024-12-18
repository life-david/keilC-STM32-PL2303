#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

GPIO_InitTypeDef GPIO_Config;
void Delay_ms(uint16_t _time);
int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_Config.GPIO_Pin 	 = GPIO_Pin_13;
	GPIO_Config.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_Config);
	
	GPIO_Config.GPIO_Pin 	 = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Config.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Config);
	
	GPIO_Config.GPIO_Pin 	 = GPIO_Pin_2;
	GPIO_Config.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Config);
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_RESET);
	while(1)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_RESET);
		Delay_ms(1000);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_SET);
		Delay_ms(1000);
		
		GPIO_WriteBit(GPIOA,GPIO_Pin_0, Bit_RESET);
		Delay_ms(1000);
		GPIO_WriteBit(GPIOA,GPIO_Pin_0, Bit_SET);
		Delay_ms(1000);
		
		GPIO_WriteBit(GPIOA,GPIO_Pin_1, Bit_RESET);
		Delay_ms(1000);
		GPIO_WriteBit(GPIOA,GPIO_Pin_1, Bit_SET);
		Delay_ms(1000);
		
		GPIO_WriteBit(GPIOB,GPIO_Pin_2, Bit_RESET);
		Delay_ms(1000);
		GPIO_WriteBit(GPIOB,GPIO_Pin_2, Bit_SET);
		Delay_ms(1000);
		
		
	}
	return 0;
}

void Delay_ms(uint16_t _time)
{
	volatile uint16_t i,j;
	for(i = 0; i < _time; i++)
	{
		for(j = 0; j < 601; j++);
	}
}
