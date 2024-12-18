#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

GPIO_InitTypeDef GPIO_Config;
void Input_Init (void){
	GPIO_InitTypeDef gpio_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_9;
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_Init(GPIOB, &gpio_InitStruct);
}
void Delay_ms(uint16_t _time);
int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_Config.GPIO_Pin 	 = GPIO_Pin_13;
	GPIO_Config.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_Config);
	
	
	Input_Init();
	
	//GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_RESET);
		
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == Bit_SET){
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
		}
		else{
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
		}
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