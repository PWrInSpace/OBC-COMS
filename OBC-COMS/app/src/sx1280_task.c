#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"

#include "sx1280.h"       
#include "radio.h"          
#include "main.h"           
#include "sx1280-radio-config.h"

#define SX1280_RX_PIN   GPIO_PIN_1 
#define SX1280_TX_PIN   GPIO_PIN_2

volatile bool radio_rx_flag = false;
volatile bool radio_tx_flag = false;

osThreadId_t sx1280taskhandle;

const osThreadAttr_t sx1280_task_attr = {
  .name = "RadioTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == SX1280_RX_PIN) {
        radio_rx_flag = true; 
    } 
    else if (GPIO_Pin == SX1280_TX_PIN) {
        radio_tx_flag = true; 
    }
}

//TODO: ADD PREFIX SENDIND AND CHECKING
//TODO: ADD THE QUEUE FOR DATA to be send to SD task and Uart task !!! think how no to copy it
//TODO: ADD TASK TO SEND THAT DATA VIA UART TO MCB
//TODO: ADD PROTOBUFF
//TODO: ADD DataStruct to decode the data??? or save just protobuffed and decode later in script or smthg
//TODO: ADD SD TASK
//TODO: ADD MAX2201 handling and all the switching analog frontend logic etc
//TODO: ADD RSSI printing on recieved packets add RSSI to protobuff frame also!
//TODO: ADD sx1280 watchdog (reding some register maybe, if not working reset device)
//TODO: ADD CRC CHECKING
//TODO: ADD TIMESTAMP for recieved packet
//TODO: ADD CHANNEL ACTIVITY DETECTION when transmitting
//TODO: Adaptive Power Output based on RSSI
//TODO: ADD CMD QUEUE HIGH PRIORITY
//TODO: ADD RANGING
//TODO: ADD parameters to the SX1280 struct like RSSI, etc..



void sx1280_transciever_task(void *pvParameters)
{

    Radio_Setup(); 

    SX1280SetRx(&sx1280_radio, (TickTime_t){RADIO_TICK_SIZE_1000_US  , 0xFFFF});
    uint8_t buffer[256];
    uint8_t size;
    for(;;) 
    {
        if(radio_rx_flag)
        {
            radio_rx_flag = false;

            
           
            
            SX1280GetPayload(&sx1280_radio, buffer, &size, 255);
            SX1280ClearIrqStatus(&sx1280_radio, IRQ_RADIO_ALL);
            SX1280SetRx(&sx1280_radio, (TickTime_t){RADIO_TICK_SIZE_1000_US , 0xFFFF});
        }
        
        if(radio_tx_flag)
        {
            radio_tx_flag = false;
            SX1280ClearIrqStatus(&sx1280_radio, IRQ_RADIO_ALL);
            SX1280SetRx(&sx1280_radio, (TickTime_t){RADIO_TICK_SIZE_1000_US  , 0xFFFF});
        }

        osDelay(1);
    }
}

bool sx1280_task_start(void)
{

    sx1280taskhandle = osThreadNew(sx1280_transciever_task, NULL, &sx1280_task_attr);

    if (sx1280taskhandle != NULL) {
        return true;
    } else {
        return false;
    }
}