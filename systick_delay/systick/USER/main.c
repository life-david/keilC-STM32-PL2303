#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "Delay.h"

volatile char vruc_SYSTICK_Flag = 0;    // Flag d? di?u khi?n toggle LED
volatile int count = 0;                 // Bi?n d?m SysTick

GPIO_InitTypeDef  GPIO_InitStructure;

// Khai báo prototype hàm
void Fn_GPIO_Init(void);
void Delay_ms(uint32_t ms);             // Hàm delay theo don v? ms

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);  // C?u hình SysTick v?i t?n s? 1ms
    Fn_GPIO_Init();

    while (1) 
		{
      // Delay 500ms
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
			Delay_ms(500);
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
			Delay_ms(500);
    }
}

void Fn_GPIO_Init(void) {
    // B?t clock cho GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
    
    // C?u hình chân PA0 làm output push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void Delay_ms(uint32_t ms) 
{   count = ms;            
    while (count);                
}

void SysTick_Handler(void) {
    if (0 <= count) 
		{         
      count--;
    }
		else
		{
			count = 0;
		}
}
