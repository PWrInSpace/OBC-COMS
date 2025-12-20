#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "sx1280_task.h"
#include <stdbool.h>
#include "main.h"
void startup_task(void *pvParameters)
{
while(true){

    //Create sx1280_task
    if(sx1280_task_start()==true);
    else{Error_Handler();}

    //Create sd_task

    //Create board_com_task

    //xTaskDelete(NULL);
}
}