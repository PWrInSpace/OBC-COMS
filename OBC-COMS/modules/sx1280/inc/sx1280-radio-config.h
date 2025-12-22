#pragma once
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "sx1280.h"
#include "semphr.h"
void Radio_Setup(void);

typedef struct {
    uint8_t  data[256];
    uint8_t  size;
    uint32_t timestamp;
    int8_t   rssi;
    int8_t   snr;

    SemaphoreHandle_t xRefCounter;
} RadioPacket_t;

extern SX1280_t sx1280_radio;