#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"    
#include "main.h"           
#include <string.h>
#include "stdbool.h"

extern osMessageQueueId_t rxFreeQueue;
extern osMessageQueueId_t rxUartQueue;

extern osMessageQueueId_t txTakenQueue;

bool UART_RX_DATA_READY = false; //zamienic na xTaskNotify z callbacka z dma
//Tutaj dodac callback po DMA i odblokowywanie transmisji mcb_uart->sx1280

void uart_task(void *argument) {
    RadioPacket_t *pRxData = NULL;
    RadioPacket_t *pTxData = NULL;
    for(;;)
    {
        // if(RX_DATA_READY) ->
        if (osMessageQueueGet(rxUartQueue, &pRxData, NULL, 0) == osOK) {
            
            // ADD HERE HAL_UART_TRANSMIT_DMA
            xSemaphoreGive(pRxData->xRefCounter);

            if (uxSemaphoreGetCount(pRxData->xRefCounter) == 2) {
                osMessageQueuePut(rxFreeQueue, &pRxData, 0, 0);
            }
        }

        if (UART_RX_DATA_READY) {
            
            if (osMessageQueueGet(txFreeQueue, &pTxData, NULL, 0) == osOK) {
                //tutaj sb skopiuj dane z bufora dma do pTxData
                osMessageQueuePut(txTakenQueue, &pTxData, 0, 0);
                UART_TX_DATA_READY = false;
            }
        }


        
    }
}