// Import the handle from spi.c
#include "sx1280.h"
#include "main.h"
#include "spi.h"

// No more hardcoded &hspi2 inside the functions!

static int32_t spi_write(void* ctx, uint8_t *prefix, uint16_t prefix_len, uint8_t* out, uint16_t out_len) {
    SPI_HandleTypeDef* hspi = (SPI_HandleTypeDef*)ctx; // Cast the context
    
    HAL_GPIO_WritePin(CS_SX1280_GPIO_Port, CS_SX1280_Pin, GPIO_PIN_RESET);
    
    HAL_SPI_Transmit(hspi, prefix, prefix_len, HAL_MAX_DELAY);
    if (out != NULL && out_len > 0) {
        HAL_SPI_Transmit(hspi, out, out_len, HAL_MAX_DELAY);
    }
    
    HAL_GPIO_WritePin(CS_SX1280_GPIO_Port, CS_SX1280_Pin, GPIO_PIN_SET);
    return 0;
}

static int32_t spi_read(void* ctx, uint8_t *prefix, uint16_t prefix_len, uint8_t* in, uint16_t in_len) {
    SPI_HandleTypeDef* hspi = (SPI_HandleTypeDef*)ctx; // Cast the context
    
    HAL_GPIO_WritePin(CS_SX1280_GPIO_Port, CS_SX1280_Pin, GPIO_PIN_RESET);
    
    HAL_SPI_Transmit(hspi, prefix, prefix_len, HAL_MAX_DELAY);
    if (in != NULL && in_len > 0) {
        HAL_SPI_Receive(hspi, in, in_len, HAL_MAX_DELAY);
    }
    
    HAL_GPIO_WritePin(CS_SX1280_GPIO_Port, CS_SX1280_Pin, GPIO_PIN_SET);
    return 0;
}

/*

static int32_t set_reset(void* ctx, bool value) {
    // Note: ensure these labels match your CubeMX "User Labels"
    HAL_GPIO_WritePin(NRST_SX1280_GPIO_Port, NRST_SX1280_Pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
    return 0;
}

static int32_t get_busy(void* ctx) {
    return (HAL_GPIO_ReadPin(BUSY_SX1280_GPIO_Port, BUSY_SX1280_Pin) == GPIO_PIN_SET) ? 1 : 0;
}

static int32_t get_dio1(void* ctx) {
    return (HAL_GPIO_ReadPin(DIO1_SX1280_GPIO_Port, DIO1_SX1280_Pin) == GPIO_PIN_SET) ? 1 : 0;
}

*/

// --- DELAY WRAPPER ---

static void delay_ms(void* ctx, uint32_t ms) {
    HAL_Delay(ms);
    return;
}



struct SX1280_s sx1280_radio = {
    .ctx = &hspi2,             // <--- ASSIGN HANDLE HERE ONCE
    .spi_read = spi_read,
    .spi_write = spi_write,
    .set_reset = NULL,
    .get_busy = NULL,
    .get_dio = NULL,
    .delay_ms = delay_ms
};