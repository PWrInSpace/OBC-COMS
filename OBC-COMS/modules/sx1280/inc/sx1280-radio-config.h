#pragma once
#include "sx1280.h"
void Radio_Setup(void);

typedef struct {
    uint8_t  data[256];
    uint8_t  size;
    uint32_t timestamp;
    int8_t   rssi;
    int8_t   snr;
} RadioPacket_t;

extern SX1280_t sx1280_radio;