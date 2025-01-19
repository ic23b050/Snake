#include "PCA9538A_I²C.h"
#include "main.h"

// Function to send the command byte to the PCA9538A
void Send_Command_To_PCA9538A(uint8_t commandByte) {
    uint8_t pData[1];  // Buffer for the command byte

    pData[0] = commandByte;  // Put the command byte in the buffer

    uint16_t DevAddress = 0x70;  // 7-bit address of the PCA9538A

    // Timeout value in milliseconds (you can adjust it)
    uint32_t Timeout = 100;

    // Send the command byte to the PCA9538A via I2C
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, DevAddress<<1, pData, sizeof(pData), Timeout);

    if (status == HAL_OK) {
        // Data was successfully transmitted
        printf("Command byte sent successfully.\n");
    } else {
        // Error handling
        printf("I2C transmission error.\n");
    }
}

void Set_Output_Port(uint8_t outputData) {
    uint8_t pData[1];

    pData[0] = outputData;  // Data to write to output port

    uint16_t DevAddress = 0x70;  // 7-bit address of the PCA9538A

    uint32_t Timeout = 100;

    // First, send the command byte (0x01 for Output Port Register)
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, DevAddress<<1, &pData[0], 1, Timeout);

    if (status == HAL_OK) {
        // Data was successfully written
        printf("Output port data sent successfully.\n");
    } else {
        // Error handling
        printf("I2C transmission error.\n");
    }
}
