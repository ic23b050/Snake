#include "PCA9538A_I²C.h"
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "stdint.h"

/**
 * @brief Structure to store direction data.
 *
 * This structure holds the x and y components of a direction vector.
 */
typedef struct {
    int x; /**< X-axis movement direction */
    int y; /**< Y-axis movement direction */
} Direction;

// Declare the direction variable
Direction direction1 = {0, 0};

/**
 * @brief Reads joystick data via I2C.
 *
 * This function communicates with the joystick over I2C to read the
 * x and y positions, as well as the button state.
 *
 * @retval JoystickState A structure containing the x and y positions,
 *         and the button state of the joystick.
 */
JoystickState readJoystick() {
    JoystickState state = {0, 0, 0};   // Initialize joystick state
    uint8_t buffer[3];  // Buffer to store joystick data (3 bytes for X, Y, and button state)

    // Read joystick data from I2C
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&hi2c1, JOYSTICK_I2C_ADDR, buffer, 3, HAL_MAX_DELAY);
    if (status == HAL_OK) {
        state.x = buffer[0];      // X-axis position from buffer
        state.y = buffer[1];      // Y-axis position from buffer
        state.button = buffer[2]; // Button state from buffer
    } else {
        // Handle I2C read error (currently no action specified)
    }
    return state;
}

/**
 * @brief Updates the direction of the snake based on joystick input.
 *
 * This function reads the joystick data and updates the `direction1`
 * variable based on the joystick's x and y values. The direction is
 * determined by comparing the joystick position with a defined threshold.
 *
 * @note The threshold value is used to filter small, accidental joystick movements.
 *
 * @retval None
 */
void updateDirectionWithJoystick() {
    JoystickState joystick = readJoystick();  // Read the joystick state

    // Threshold values to detect meaningful movement (tune as necessary)
    int threshold = 20;

    // Check joystick movement and update direction accordingly
    if (joystick.x > threshold) {
        direction1.x = 1; direction1.y = 0; // Right
    } else if (joystick.x < -threshold) {
        direction1.x = -1; direction1.y = 0; // Left
    } else if (joystick.y > threshold) {
        direction1.x = 0; direction1.y = 1; // Down
    } else if (joystick.y < -threshold) {
        direction1.x = 0; direction1.y = -1; // Up
    }
}
