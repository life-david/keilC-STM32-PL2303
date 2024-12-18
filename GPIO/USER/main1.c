#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

GPIO_InitTypeDef GPIO_Config;

int main(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
		GPIO_Config.GPIO_Pin = GPIO_Pin_13;
	GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_Config);
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
	while(1){
	}
	return 0;
}