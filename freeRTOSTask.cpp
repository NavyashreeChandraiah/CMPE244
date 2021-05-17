/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * @brief This is the application entry point.
 * 			FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 * 			@see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "tasks.hpp"
#include "queue.h"

QueueHandle_t qh = 0;


void rx(void *p)
{
    int item = 0;
    puts("rx task");
    if (xQueueReceive(qh, &item, portMAX_DELAY))
    {
        puts("Rx received an item!");
    }
    vTaskSuspend(NULL);
    puts("Rx is suspended!");
}

void tx(void *p)
{
    int item = 0;
    while(1)
    {
        puts("Going to send an item.");
        xQueueSend(qh, &item, 0);
        puts("Did I send an item?");

        xQueueSend(qh), &item, portMAX_DELAY);
        puts("I must have sent an item");
    }
}

int main(void)
{
    qh = xQueueCreate(2, sizeof(int));
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));
	xTaskCreate(rx, "rx", 512, ( void * ) 'A', PRIORITY_LOW, NULL );
    xTaskCreate(tx, "tx", 512, ( void * ) 'A', PRIORITY_HIGH, NULL );
    
    // Alias to vSchedulerStart();
    scheduler_start();
    return -1;
}
