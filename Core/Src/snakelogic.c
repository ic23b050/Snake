#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "snakelogic.h"
#include "time.h"
#include "stdlib.h"
#include "MAX7129.h"
#include "PCA9538A_I²C.h"


Point snake[64]; // Maximum length of the snake
int snake_length = 3; // Initial length of the snake
Point direction = {0, 0}; // Current direction of the snake

/* Constants */
#define MATRIX_SIZE 8

/* Global Variables */

/**
  * @brief  Plays the snake game.
  *
  * This function runs the snake game by updating the direction, moving the snake,
  * and refreshing the LED matrix to show the current snake state.
  *
  * @retval None
  */
void playSnake(void)
{
    static int initialized = 0;
    if (!initialized) {
        srand(time(NULL));  // Initialize random number generator
        initSnake();        // Initialize the snake
        initialized = 1;    // Set initialized flag to true
    }

    updateDirection();    // Update the snake's direction
    moveSnake();          // Move the snake based on the current direction
    updateLEDMatrix();    // Update the LED matrix to reflect the new snake state
}

/**
  * @brief  Initializes the snake game.
  *
  * This function initializes the snake by setting the initial position of the
  * snake's segments.
  *
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
  *
  * This function calls `updateDirectionWithJoystick()` to update the snake's
  * direction based on joystick input.
  *
  * @retval None
  */
void updateDirection() {
    updateDirectionWithJoystick();  // Update direction based on joystick input
}

/**
  * @brief  Moves the snake in the current direction.
  *
  * This function moves the snake one step in the current direction. It shifts
  * the position of each segment, and updates the head's position according
  * to the current direction. If the snake goes out of bounds, it wraps around.
  *
  * @retval None
  */
void moveSnake() {
    // Move each segment to the position of the previous segment
    for (int i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    // Move the head of the snake in the current direction
    snake[0].x += direction.x;
    snake[0].y += direction.y;

    // Wrap the snake around the matrix if it goes out of bounds
    if (snake[0].x >= MATRIX_SIZE) snake[0].x = 0;
    if (snake[0].x < 0) snake[0].x = MATRIX_SIZE - 1;
    if (snake[0].y >= MATRIX_SIZE) snake[0].y = 0;
    if (snake[0].y < 0) snake[0].y = MATRIX_SIZE - 1;
}

/**
  * @brief  Updates the LED matrix to reflect the current state of the snake.
  *
  * This function clears the LED matrix and then lights up the LEDs that
  * correspond to the current positions of the snake segments.
  *
  * @retval None
  */
void updateLEDMatrix() {
    clearMatrix();  // Clear the LED matrix
    for (int i = 0; i < snake_length; i++) {
        setLED(snake[i].x, snake[i].y, 1);  // Set LEDs for each snake segment
    }
}

/**
  * @brief  Displays the menu on the LED matrix.
  *
  * This function clears the LED matrix and then displays an icon corresponding
  * to the selected menu option.
  *
  * @retval None
  */
void showMenu(void) {
    clearMatrix();  // Clear the LED matrix
    switch (selectedMenuIndex) {
        case 0:
            setLED(3, 3, 1);  // Display Snake Icon for option 0
            break;
        case 1:
            setLED(4, 4, 1);  // Display Tower Bloxx Icon for option 1
            break;
        case 2:
            setLED(5, 5, 1);  // Display N/A Icon for option 2
            break;
    }
}

/**
  * @brief  Plays the Tower Bloxx game.
  *
  * This function simulates a simple version of the Tower Bloxx game. It drops
  * blocks on the LED matrix based on joystick input. If the joystick button
  * is pressed, a block is placed at a specific position and moves down the
  * matrix. The game ends when the block reaches the bottom.
  *
  * @retval None
  */
void playTowerBloxx(void)
{
    clearMatrix();  // Clear the LED matrix
    int blockY = 0; // Initial Y position for the block
    while (1) {
        JoystickState joystick = readJoystick();  // Read joystick input
        if (joystick.button) {
            setLED(4, blockY, 1);  // Drop a block at the current Y position
            blockY++;
            if (blockY >= MATRIX_SIZE) break;  // End game if the block reaches the bottom
        }
        HAL_Delay(500);  // Delay for block drop speed
    }
}

/**
  * @brief  Handles inactivity (e.g., timeout or pause).
  *
  * This function handles inactivity by clearing the matrix, setting the
  * brightness to low, and showing the main menu.
  *
  * @retval None
  */
void handleInactivity(void) {
    clearMatrix();  // Clear the LED matrix
    uint8_t brightnessCommand[2] = {0x0A, 0x01};  // Low brightness command
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);  // SPI CS low
    HAL_SPI_Transmit(&hspi1, brightnessCommand, 2, HAL_MAX_DELAY);  // Send brightness command
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);  // SPI CS high

    showMenu();  // Display the menu after inactivity
}
