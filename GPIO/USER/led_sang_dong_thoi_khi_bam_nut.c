#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

// Khai báo bi?n toàn c?c
volatile uint8_t ledState = 0;       // Tr?ng thái dèn (0 = LED1 sáng, 1 = LED2 sáng)
volatile uint8_t buttonPressed = 0; // Tr?ng thái nút nh?n (0 = chua nh?n, 1 = dã nh?n)

// Khai báo các hàm
void Input_Init(void);                // Hàm kh?i t?o nút nh?n
void LED_Init(void);                  // Hàm kh?i t?o LED
void Delay_ms(uint16_t _time);        // Hàm delay don gi?n
void Toggle_LED(void);                // Hàm d?i tr?ng thái LED
void Update_ButtonState(void);        // Hàm ki?m tra tr?ng thái nút nh?n

int main(void) {
    LED_Init();        // Kh?i t?o các LED
    Input_Init();      // Kh?i t?o nút nh?n

    while (1) {
        Update_ButtonState(); // C?p nh?t tr?ng thái nút nh?n

        // C?p nh?t tr?ng thái các dèn LED
        if (ledState == 0) {
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // B?t LED1 (C13)
            GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);    // T?t LED2 (A7)
            GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);  // B?t LED3 (A0)
        } else {
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);   // T?t LED1 (C13)
            GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);  // B?t LED2 (A7)
            GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);    // T?t LED3 (A0)
        }
    }

    return 0;
}

// Hàm kh?i t?o nút nh?n
void Input_Init(void) {
    GPIO_InitTypeDef gpio_InitStruct;

    // B?t clock cho GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    gpio_InitStruct.GPIO_Pin = GPIO_Pin_9;  // Chân B9 cho nút nh?n
    gpio_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // Input Pull-Up
    GPIO_Init(GPIOB, &gpio_InitStruct);
}

// Hàm kh?i t?o các dèn LED
void LED_Init(void) {
    GPIO_InitTypeDef gpio_InitStruct;

    // B?t clock cho GPIOC và GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

    // C?u hình LED1 (C13)
    gpio_InitStruct.GPIO_Pin = GPIO_Pin_13;
    gpio_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  // Output Push-Pull
    gpio_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio_InitStruct);

    // C?u hình LED2 (A7)
    gpio_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &gpio_InitStruct);

    // C?u hình LED3 (A0)
    gpio_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &gpio_InitStruct);
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

// Hàm d?i tr?ng thái dèn LED
void Toggle_LED(void) {
    ledState = !ledState; // Ð?i tr?ng thái gi?a LED1 và LED2
}

// Hàm delay don gi?n
void Delay_ms(uint16_t _time) {
    volatile uint16_t i, j;
    for (i = 0; i < _time; i++) {
        for (j = 0; j < 601; j++);
    }
}