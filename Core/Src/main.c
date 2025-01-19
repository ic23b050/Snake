/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @author         : 177013
  * @date		    : 19-01-2025
  ******************************************************************************
  * @attention
  * This program is designed to run on an STM32 microcontroller and makes use of
  * peripherals such as SPI, I2C, UART & GPIO to interact with external components.
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
  int x;
  int y;
} Point;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MATRIX_SIZE 8

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;
Point snake[64]; // Maximum length of the snake
int snake_length = 3; // Initial length of the snake
Point direction = {1, 0}; // Initial direction (moving right)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void clearMatrix(void);
void setLED(int x, int y, int state);
void initSnake(void);
void updateDirection(void);
void moveSnake(void);
void updateLEDMatrix(void);
void playSnake(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //HAL_I2C_Master_Transmit(&hi2c, DevAddress, &pData, Size, Timeout);
	  //HAL_I2C_Master_Receive(&hi2c, DevAddress, &pData, Size, Timeout);
	  Send_Command_To_PCA9538A(0x00);
	  playSnake();
	  HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Clears the LED matrix.
  * @retval None
  */
void clearMatrix(void) {
  for (int i = 1; i <= 8; i++) {
    uint8_t clearData[2] = {i, 0x00};
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // NSS low
    HAL_SPI_Transmit(&hspi1, clearData, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // NSS high
  }
}


/**
  * @brief  Plays the snake game.
  * @retval None
  */
void playSnake(void)
{
	  static int initialized = 0;
	  if (!initialized) {
	    srand(time(NULL));
	    initSnake();
	    initialized = 1;
	  }

	  updateDirection();
	  moveSnake();
	  updateLEDMatrix();
}

/**
  * @brief  Sets the state of an LED in the matrix.
  * @param  x: X coordinate of the LED
  * @param  y: Y coordinate of the LED
  * @param  state: State of the LED (1 for on, 0 for off)
  * @retval None
  */

void setLED(int x, int y, int state) {
	  if (x < 0 || x >= 8 || y < 0 || y >= 8) return; // Out of bounds

	  uint8_t row = y + 1;
	  uint8_t colData = 1 << x;

	  uint8_t currentData[2] = {row, 0x00};
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // NSS low
	  HAL_SPI_Receive(&hspi1, currentData, 2, HAL_MAX_DELAY);
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // NSS high

	  if (state) {
	    currentData[1] |= colData;
	  } else {
	    currentData[1] &= ~colData;
	  }

	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // NSS low
	  HAL_SPI_Transmit(&hspi1, currentData, 2, HAL_MAX_DELAY);
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // NSS high
}

/**
  * @brief  Initializes the snake game.
  * @retval None
  */
void initSnake() {
	  snake[0].x = 3;
	  snake[0].y = 3;
	  snake[1].x = 2;
	  snake[1].y = 3;
	  snake[2].x = 1;
	  snake[2].y = 3;
}

/**
  * @brief  Updates the direction of the snake.
  * @retval None
  */

void updateDirection() {
	  int dir = rand() % 4;
	  switch (dir) {
	    case 0: direction.x = 1; direction.y = 0; break; // Right
	    case 1: direction.x = -1; direction.y = 0; break; // Left
	    case 2: direction.x = 0; direction.y = 1; break; // Down
	    case 3: direction.x = 0; direction.y = -1; break; // Up
	  }
}

// Documentation

/**
  * @brief  Moves the snake in the current direction.
  * @retval None
  */

void moveSnake() {
	for (int i = snake_length - 1; i > 0; i--) {
	    snake[i] = snake[i - 1];
	  }
	  snake[0].x += direction.x;
	  snake[0].y += direction.y;

	  if (snake[0].x >= MATRIX_SIZE) snake[0].x = 0;
	  if (snake[0].x < 0) snake[0].x = MATRIX_SIZE - 1;
	  if (snake[0].y >= MATRIX_SIZE) snake[0].y = 0;
	  if (snake[0].y < 0) snake[0].y = MATRIX_SIZE - 1;
}

/**
  * @brief  Updates the LED matrix to reflect the current state of the snake.
  * @retval None
  */

void updateLEDMatrix() {
	  clearMatrix();
	  for (int i = 0; i < snake_length; i++) {
	    setLED(snake[i].x, snake[i].y, 1);
	  }
}


void playTowerBloxx(void)
{
  // Implement automated block dropping for Tower Bloxx
}

void playTetris(void)
{
  // Implement automated movements and rotations for Tetris
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
