//#include "PCA9538A_I²C.h"
//#include "main.h"


#include "PCA9538A_I²C.h"
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "usart.h"
#include "spi.h"
#include "stdint.h"


typedef struct {
    int x;
    int y;
} Direction;

// Declare the direction variable
Direction direction1 = {0, 0};

JoystickState readJoystick() {
    JoystickState state = {0, 0, 0};
    uint8_t buffer[3]; // Example: 3 bytes for X, Y, and button state

    // Read joystick data from I2C
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&hi2c1, JOYSTICK_I2C_ADDR, buffer, 3, HAL_MAX_DELAY);
    if (status == HAL_OK) {
        state.x = buffer[0];      // X-axis position
        state.y = buffer[1];      // Y-axis position
        state.button = buffer[2]; // Button state (if available)
    } else {
        // Handle I2C read error
    }
    return state;
}

void updateDirectionWithJoystick() {
    JoystickState joystick = readJoystick();

    // Threshold values to detect meaningful movement (tune as necessary)
    int threshold = 20;

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







