#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

GPIO_InitTypeDef GPIO_Config;

// Khai báo bi?n toàn c?c
volatile uint8_t ledState = 0;       // 0 = C13 t?t, A0 t?t; 1 = C13 b?t, A0 t?t; 2 = C13 t?t, A0 b?t
volatile uint8_t buttonPressed = 0; // Tr?ng thái nút nh?n: 0 = chua nh?n, 1 = dã nh?n

// Khai báo các hàm
void Input_Init(void);
void LED_Init(void);
void Delay_ms(uint16_t _time);
void Toggle_LED(void);
void Update_ButtonState(void);

int main() {
    // Kh?i t?o h? th?ng
    LED_Init();      // Kh?i t?o LED
    Input_Init();    // Kh?i t?o nút nh?n

    while (1) {
        Update_ButtonState();

        // C?p nh?t tr?ng thái dèn
        if (ledState == 0) {
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);   // T?t LED C13
            GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);   // T?t LED A0
        } else if (ledState == 1) {
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // B?t LED C13
            GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);    // T?t LED A0
        } else if (ledState == 2) {
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);   // T?t LED C13
            GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET); // B?t LED A0
        }
    }

    return 0;
}

// Hàm kh?i t?o LED
void LED_Init(void) {
    // Kh?i t?o LED C13
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_Config.GPIO_Pin = GPIO_Pin_13;
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_Config);

    // Kh?i t?o LED A0
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_Config.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_Config);
}

// Hàm kh?i t?o nút nh?n
void Input_Init(void) {
    GPIO_InitTypeDef gpio_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    gpio_InitStruct.GPIO_Pin = GPIO_Pin_9;
    gpio_InitStruct.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_Init(GPIOB, &gpio_InitStruct);
}

// Hàm ki?m tra tr?ng thái nút nh?n
void Update_ButtonState(void) {
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == Bit_RESET) { // Nút du?c nh?n
        if (!buttonPressed) { // N?u nút v?a m?i du?c nh?n
            Toggle_LED();       // Ð?i tr?ng thái LED
            buttonPressed = 1;  // Ðánh d?u nút dã du?c nh?n
        }
    } else { // Nút không du?c nh?n
        buttonPressed = 0; // Ð?t l?i tr?ng thái nút nh?n
    }
}

// Hàm d?i tr?ng thái LED
void Toggle_LED(void) {
    if (ledState == 0) {
        ledState = 1; // B?t LED C13, t?t LED A0
    } else if (ledState == 1) {
        ledState = 2; // B?t LED A0, t?t LED C13
    } else {
        ledState = 0; // T?t c? hai LED
    }
}

// Hàm delay don gi?n
void Delay_ms(uint16_t _time) {
    volatile uint16_t i, j;
    for (i = 0; i < _time; i++) {
        for (j = 0; j < 601; j++);
    }
}