#include "main.h"

// các thu vien bo sung 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Bien toa cuc
UART_HandleTypeDef huart1;        // Cau hinh UART1
DMA_HandleTypeDef hdma_usart1_rx; // cau hinh DMA cho UART 1 RX

/* USER CODE BEGIN PV */
uint8_t RxBuff[8];							// Bo dem nhan du lieu (8 byte)
uint8_t TxBuff[8] = "START..."; // buffer truyen di
uint16_t rxLength = 0;					// Do dai du lieu nhan duoc
uint32_t count = 0;							// Bo dem

volatile uint8_t running = 1;		// Luu tru trang thai cua bo dem (1: chay, 0: dung )
volatile uint8_t counter = 0;		// Bo dem hien thi 
uint8_t segment_map[] = {   		// Mang anh xa LED 7 doan
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90  // 9
};

/* USER CODE END PV */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// Kiem tra nhan du lieu tu UART1 
  UNUSED(huart);
	if(huart->Instance == USART1)
	{
		// Lay so byte nhan duoc qua DMA.
		// Sao chép du lieu tu RxBuff sang TxBuff.
		// Phan hoi qua UART bang HAL_UART_Transmit.
		// Kiem tra neu chuoi nhan duoc là "Start" thì bat dem (running = 1) hoac "Stop" thì dung dem (running = 0).
		// Xóa bo dem RxBuff và kích hoat lai DMA de nhan du lieu moi.
		uint16_t receivedBytes = 8 - __HAL_DMA_GET_COUNTER(huart->hdmarx);
		
		for(int i = 0; i < receivedBytes; i++){
			TxBuff[i] = RxBuff[i];
		}

		HAL_UART_Transmit(&huart1, TxBuff, receivedBytes, HAL_MAX_DELAY);
		
		// Xu ly lenh "Start" va "stop"
		if (strncmp((char*)&TxBuff, "Start", 5) == 0)
		{
				running = 1; // Bat dau dem 
		}
		else if (strncmp((char*)&TxBuff, "Stop", 4) == 0)
		{
				running = 0; // dung dem
		}
				
		memset(RxBuff, 0, 8); // Xóa bo dem nhan 
		
		HAL_UART_Receive_DMA(&huart1, RxBuff, 8);	// Kich hoat lai nhan DMA
	}
}

// Callback khi nút nhan duoc nha
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BUTTON_Pin)
    {
        counter = 1; // Dat lai bo dem 
    }
}

// hien thi so len LED 7 doan 
void display_number(uint8_t number)
{
    uint8_t tens = number / 10;		// Hang chuc
    uint8_t units = number % 10;	// Hang don vi 
		
		// hien thi hang chuc
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    GPIOA->ODR = (GPIOA->ODR & 0xFF00) | segment_map[tens];
    HAL_Delay(2);

		// Hien thi hang don vi 
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    GPIOA->ODR = (GPIOA->ODR & 0xFF00) | segment_map[units];
    HAL_Delay(2);
}
/* USER CODE END 0 */

int main(void){
  HAL_Init();
	// Clock he thong GBIO, DMA, UART
  SystemClock_Config();
	
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  // Gui thong bao he thong qua UART va kich hoat du lieu DMA
	HAL_UART_Transmit(&huart1, TxBuff, sizeof(TxBuff),100);
	HAL_UART_Receive_DMA(&huart1, RxBuff, 8);
	
  while (1){
    
		for (int i = 0; i < 200; i++)
		{
				display_number(counter);
		}
		if (running)
		{
				
				counter++;
				if (counter > 99)
						counter = 0;
		}
				
  }
	return 0;
  
}

// Cau hinh clock 
void SystemClock_Config(void)
{	
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	//cau hinh dao dong RCC theo cac tham so duoc chi dinh 
	
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
	// Khoi dong clock CPU, bus AHB va APB
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_USART1_UART_Init(void){
	// Cau hinh UART1
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  

}

// Cau hinh DMA
static void MX_DMA_Init(void)
{

  // bat dau 
  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

// Cau hinh GPIO 
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

 
  HAL_GPIO_WritePin(GPIOA, SEGA_Pin|SEGB_Pin|SEGC_Pin|SEGD_Pin
                          |SEGE_Pin|SEGF_Pin|SEGG_Pin|SEGH_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOB, LED1_Pin|LED2_Pin, GPIO_PIN_RESET);

  // Cau hinh den LED 7 doan
  GPIO_InitStruct.Pin = SEGA_Pin|SEGB_Pin|SEGC_Pin|SEGD_Pin
                          |SEGE_Pin|SEGF_Pin|SEGG_Pin|SEGH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Cau hinh chan LED 1, LED 2
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Cau hinh nút bam 
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

  // che do ngat ngoai 
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

// Ham xu ly debug 
void Error_Handler(void){
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
	
}
#endif /* USE_FULL_ASSERT */
