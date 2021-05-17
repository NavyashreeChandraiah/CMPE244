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
#include "tasks.hpp"
#include "utilities.h"
#include "io.hpp"
#include "LPC17xx.h"
#include "UartDriver.hpp"


QueueHandle_t q = xQueueCreate(50, sizeof(char));
UartDriver *myUart3 = new UartDriver();


void uart3RxInt()
{
    char data;
    data = myUart3->receiveChar();
    xQueueSendFromISR(q, &data, NULL);
    //clear interrupt flag
}

void vTaskCode(void * pvParameters)
{

    vTaskDelay(1);
    myUart3->transmitChar('1');
    vTaskDelay(1);
    myUart3->transmitChar('2');
    vTaskDelay(1);
    myUart3->transmitChar('+');

    printf("sent 1, 2, +\n");

    char result = 0;
    //xQueueReceive(q, &result, 1000000); //dummy
    xQueueReceive(q, &result, 1000000);

    printf("Result: %u\n", result- '0');

    LD.setNumber(result - '0');

    vTaskDelay(1000000000);

}

int main(void)
{
    isr_register(UART3_IRQn, uart3RxInt);
    myUart3->UartInit(3, 9600);
    
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));
	xTaskCreate(vTaskCode, "vTaskCode", 512, ( void * ) 'A', tskIDLE_PRIORITY, NULL );
    // Alias to vSchedulerStart();
    scheduler_start();
    return -1;
}
