/*
 * MAX7129.c
 *
 *  Created on: Jan 21, 2025
 *      Author: Yukkuri
 */


#include "PCA9538A_I²C.h"
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "MAX7129.h"



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

