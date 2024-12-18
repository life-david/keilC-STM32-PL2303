#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

#define LED_CHUC GPIO_Pin_1             // LED hàng ch?c
#define LED_DONVI GPIO_Pin_0            // LED hàng don v?
#define BUTTON GPIO_Pin_9

// M?ng mã hi?n th? 7 do?n (Common Cathode)
unsigned char NUM[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

// Hàm t?o d? tr?
void delay(unsigned int time) {
    for (unsigned int i = 0; i < time; i++) {
        for (volatile unsigned int j = 0; j < 0x2AFF; j++); // Tang vòng l?p n?u c?n
    }
}

// C?u hình GPIO
void configure_gpio() {
    GPIO_InitTypeDef gpio;

    // C?p xung clock cho GPIOA và GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);

    // C?u hình các chân PA0->PA6 làm Output Push-Pull (cho LED 7 do?n)
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    // C?u hình các chân PB0, PB1 di?u khi?n LED hàng ch?c và don v?
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    // C?u hình nút b?m PB9 làm Input Pull-up
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    gpio.GPIO_Pin = BUTTON;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);
}

// Hàm chính
int main() {
    configure_gpio();  // C?u hình GPIO

    int i, j;
    int current_number = 0;  // Bi?n ch?a s? hi?n t?i (ban d?u là 0)
    int reset = 0;  // Bi?n ki?m tra tr?ng thái reset

    while (1) {
        // Ki?m tra tr?ng thái nút b?m
        if (GPIO_ReadInputDataBit(GPIOB, BUTTON) == Bit_RESET) {  // N?u nút b?m du?c nh?n (PB9 v? m?c th?p)
            reset = 1;  // Ðánh d?u r?ng nút dã du?c nh?n
            delay(200);  // Delay d? tránh bounce c?a nút b?m
        }

        if (reset == 1) {  // N?u nút b?m dã du?c nh?n
            current_number = 0;  // Reset giá tr? s? v? 0 (hi?n th? 00 trên LED 7 do?n)
            // Hi?n th? 00 trên LED 7 do?n
            GPIO_ResetBits(GPIOB, LED_CHUC);  // B?t hàng ch?c
            GPIO_SetBits(GPIOB, LED_DONVI);  // T?t hàng don v?
            GPIOA->ODR = (GPIOA->ODR & 0xFFFFFF80) | NUM[0];  // Hi?n th? hàng ch?c
            delay(1);  // Delay 1ms

            GPIO_ResetBits(GPIOB, LED_DONVI);  // B?t hàng don v?
            GPIO_SetBits(GPIOB, LED_CHUC);  // T?t hàng ch?c
            GPIOA->ODR = (GPIOA->ODR & 0xFFFFFF80) | NUM[0];  // Hi?n th? hàng don v?
            delay(1);  // Delay 1ms
        } else {
            // Hi?n th? s? trên LED 7 do?n
            for (i = 0; i < 100; i++) {  // Ð?m t? 00 d?n 99
                for (j = 0; j < 200; j++) {  // L?p l?i d? duy trì hi?n th?
                // Quét LED hàng ch?c
                    GPIO_ResetBits(GPIOB, LED_CHUC);  // B?t hàng ch?c
                    GPIO_SetBits(GPIOB, LED_DONVI);  // T?t hàng don v?
                    GPIOA->ODR = (GPIOA->ODR & 0xFFFFFF80) | NUM[i / 10];  // Hi?n th? hàng ch?c
                    delay(1);  // Delay 1ms

                    // Quét LED hàng don v?
                    GPIO_ResetBits(GPIOB, LED_DONVI);  // B?t hàng don v?
                    GPIO_SetBits(GPIOB, LED_CHUC);  // T?t hàng ch?c
                    GPIOA->ODR = (GPIOA->ODR & 0xFFFFFF80) | NUM[i % 10];  // Hi?n th? hàng don v?
                    delay(1);  // Delay 1ms
                }
                current_number = (current_number + 1) % 100;  // Ti?n hành d?m t? 00 d?n 99
            }
        }
    }

    return 0;
}