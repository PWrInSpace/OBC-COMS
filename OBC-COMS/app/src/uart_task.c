#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"    
#include "main.h"           
#include <string.h>

extern osMessageQueueId_t txFreeQueue;
extern osMessageQueueId_t txTakenQueue;

void uart_task(void *argument) {
    // You can use them now!
    for(;;)
    {
        
    }
}