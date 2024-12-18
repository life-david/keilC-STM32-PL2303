#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO


volatile char	flag = 0;
volatile int count = 0;

GPIO_InitTypeDef  GPIO_InitStructure;
void Fn_GPIO_Init (void);

int main (void){
	
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
	Fn_GPIO_Init();
	
	while(1){
		if(flag == 1)
		{
				GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_SET);
		}
		else
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_13, Bit_RESET);
		}
	}
}


void Fn_GPIO_Init (void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void SysTick_Handler(void){
	if(count < 1000)
	{
		count ++;
	}
	else
	{
		flag =! flag;
		count = 0;
	}
	
} 

