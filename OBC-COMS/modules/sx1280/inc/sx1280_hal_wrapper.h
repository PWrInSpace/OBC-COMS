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
    uint32_t frequency;
    uint8_t  tx_power;
    uint8_t  spreading_factor;
    uint8_t  bandwidth;      
    
    //Link Qual
    int8_t   last_rssi;
    int8_t   last_snr;
    uint32_t packet_count_rx;
    uint32_t packet_count_tx;
    uint32_t crc_error_count;

    volatile RadioStates_t state;
    bool     is_initialized;
    
    uint32_t last_irq_timestamp;
} sx1280_t;