#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "sx1280.h"        
#include "radio.h"           
#include "main.h"            
#include "sx1280-radio-config.h"
#include <string.h>
//TODO:RECONSIDER MESSAGE INSTEAD OF QUEUE
//TODO: RECONSIDER VARIABLE DMA SIZE INSTEAD OF FIXED (check Mateo`s GroundSTation Project where it works)
//TODO: there is a macro that allows to check whether the DMA is idle now and allows to get the counter
//TODO: ADD Usart_task.c and SD_task.c
//TODO: Adapt Buffer Pool methode to support Sd_Task and Usart_Task, Counting Semaphore?
//TODO: Switch the spi_transmit to spi_transmit_dma in sx1280 lib where its possible
extern SX1280_t sx1280_radio;

#define POOL_SIZE 5
#define SX1280_RX_PIN    GPIO_PIN_1 //TEMP SET IN CUBE MX
#define SX1280_TX_PIN    GPIO_PIN_2 //TEMP SET IN CUBE MX

#define RADIO_EVENT_RX_READY    (1 << 0)
#define RADIO_EVENT_TX_DONE     (1 << 1)

osThreadId_t sx1280taskhandle;


RadioPacket_t rxBufferPool[POOL_SIZE];
RadioPacket_t txBufferPool[POOL_SIZE];

osMessageQueueId_t rxFreeQueue;
osMessageQueueId_t rxTakenQueue;
osMessageQueueId_t txFreeQueue;
osMessageQueueId_t txTakenQueue;

const osThreadAttr_t sx1280_task_attr = {
  .name = "RadioTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (GPIO_Pin == SX1280_RX_PIN) {
        xTaskNotifyFromISR(sx1280taskhandle, RADIO_EVENT_RX_READY, eSetBits, &xHigherPriorityTaskWoken);
    } 
    else if (GPIO_Pin == SX1280_TX_PIN) {
        xTaskNotifyFromISR(sx1280taskhandle, RADIO_EVENT_TX_DONE, eSetBits, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void sx1280_transciever_task(void *pvParameters) {
    Radio_Setup();
    SX1280SetRx(&sx1280_radio, (TickTime_t){RADIO_TICK_SIZE_1000_US, 0xFFFF});

    uint32_t activeEvents;
    RadioPacket_t *pRxPacket = NULL;
    RadioPacket_t *pTxPacket = NULL;

    for(;;) {
        if (xTaskNotifyWait(0, 0xFFFFFFFF, &activeEvents, pdMS_TO_TICKS(10)) == pdTRUE) {
            
            if (activeEvents & RADIO_EVENT_RX_READY) {
                if (osMessageQueueGet(rxFreeQueue, &pRxPacket, NULL, 0) == osOK) {
                    SX1280GetPayload(&sx1280_radio, pRxPacket->data, &pRxPacket->size, 255);
                    pRxPacket->timestamp = xTaskGetTickCount();

                    PacketStatus_t pktStatus;
                    SX1280GetPacketStatus(&sx1280_radio, &pktStatus);
                    pRxPacket->rssi = pktStatus.Params.LoRa.RssiPkt;
                    pRxPacket->snr  = pktStatus.Params.LoRa.SnrPkt;

                    osMessageQueuePut(rxTakenQueue, &pRxPacket, 0, 0);
                }
                SX1280ClearIrqStatus(&sx1280_radio, IRQ_RADIO_ALL);
                SX1280SetRx(&sx1280_radio, (TickTime_t){RADIO_TICK_SIZE_1000_US, 0xFFFF});
            }

            if (activeEvents & RADIO_EVENT_TX_DONE) {
                SX1280ClearIrqStatus(&sx1280_radio, IRQ_RADIO_ALL);
                SX1280SetRx(&sx1280_radio, (TickTime_t){RADIO_TICK_SIZE_1000_US, 0xFFFF});
            }
        }

        if (osMessageQueueGet(txTakenQueue, &pTxPacket, NULL, 0) == osOK) {
            SX1280SetStandby(&sx1280_radio, STDBY_RC);
            SX1280SendPayload(&sx1280_radio, pTxPacket->data, pTxPacket->size, 
                             (TickTime_t){RADIO_TICK_SIZE_1000_US, 0});
            
            osMessageQueuePut(txFreeQueue, &pTxPacket, 0, 0);
        }
    }
}

bool sx1280_task_start(void) {

    rxFreeQueue = osMessageQueueNew(POOL_SIZE, sizeof(RadioPacket_t*), NULL);
    rxTakenQueue = osMessageQueueNew(POOL_SIZE, sizeof(RadioPacket_t*), NULL);

    txFreeQueue = osMessageQueueNew(POOL_SIZE, sizeof(RadioPacket_t*), NULL);
    txTakenQueue = osMessageQueueNew(POOL_SIZE, sizeof(RadioPacket_t*), NULL);

    if (!rxFreeQueue || !rxTakenQueue || !txFreeQueue || !txTakenQueue) return false;

    for(int i = 0; i < POOL_SIZE; i++) {
        RadioPacket_t* pRx = &rxBufferPool[i];
        RadioPacket_t* pTx = &txBufferPool[i];
        osMessageQueuePut(rxFreeQueue, &pRx, 0, 0);
        osMessageQueuePut(txFreeQueue, &pTx, 0, 0);
    }

    sx1280taskhandle = osThreadNew(sx1280_transciever_task, NULL, &sx1280_task_attr);
    return (sx1280taskhandle != NULL);
}