#include "FreeRTOS.h"
#include "semphr.h"
#include "cmsis_os2.h"
#include "radio.h"
#include "sx1280-radio-config.h"
#include <string.h>

#define BUFF_SIZE 10 
extern osMessageQueueId_t rxSDQueue;
extern osMessageQueueId_t rxFreeQueue;

RadioPacket_t sdBufferA[BUFF_SIZE];
RadioPacket_t sdBufferB[BUFF_SIZE];

void sd_task(void *argument) {
    RadioPacket_t *pRxData = NULL;
    RadioPacket_t *activeWriteBuffer;

    for(;;) {
        if (osMessageQueueGet(rxSDQueue, &pRxData, NULL, osWaitForever) == osOK) {

            xSemaphoreGive(pRxData->xRefCounter);
            if (uxSemaphoreGetCount(pRxData->xRefCounter) == 2) {
                osMessageQueuePut(rxFreeQueue, &pRxData, 0, 0);
            }

            }
        }
    }