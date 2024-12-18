#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO


volatile char	flag1 = 0,  flag2 = 0;
volatile int  count1 = 0, count2 = 0;

GPIO_InitTypeDef  GPIO_InitStructure;
void Fn_GPIO_Init (void);

int main (void){
	
	SystemInit();
	SystemCoreClockUpdate();
	// 72000
	SysTick_Config(SystemCoreClock/1000);
	Fn_GPIO_Init();
	
	while(1)
	{
		
		if(flag1 == 1)
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_1, Bit_SET);
		}
		else
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_1, Bit_RESET);
		}
		
		if(flag2 == 1)
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_0, Bit_SET);
		}
		else
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_0, Bit_RESET);
		}
	}
}

void Fn_GPIO_Init (void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void SysTick_Handler(void){
	if(count1 < 1000)
	{
		count1 ++;
	}
	else
	{
		flag1 =! flag1;
		count1 = 0;
	}
	
	if(count2 < 500)
	{
		count2 ++;
	}
	else
	{
		flag2 =! flag2;
		count2 = 0;
	}
} 

