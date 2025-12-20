#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void (*spi_transmit)(uint8_t* pData, uint16_t size);
    void (*spi_receive)(uint8_t* pData, uint16_t size);
    void (*set_cs)(bool state);
    void (*set_reset)(bool state);
    void (*delay_ms)(uint32_t ms);
} sx1280_hw_t;

// The main device handle
typedef struct {
    sx1280_hw_t hw;
    // Add internal radio states here (e.g., current frequency)
} sx1280_t;

// Function to link STM32 HAL to the driver
void sx1280_port_init(sx1280_t *dev);