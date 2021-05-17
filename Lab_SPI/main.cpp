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
/*
#include <stdio.h>
#include "tasks.hpp"
#include "utilities.h"
#include "io.hpp"
#include "LabSPI.hpp"

typedef union
{
    uint8_t byte;
    struct
    {
        uint8_t rdy1:       1;
        uint8_t comp:       1;
        uint8_t density:    4;
        uint8_t protect:    1;
        uint8_t page_size:  1;
    }__attribute__((packed));
}statReg1;

typedef union
{
    uint8_t byte;
    struct
    {
        uint8_t rdy2:       1;
        uint8_t :           1;
        uint8_t epe:        1;
        uint8_t :           1;
        uint8_t sle:        1;
        uint8_t ps2:        1;
        uint8_t ps1:        1;
        uint8_t es:         1;
    }__attribute__((packed));
}statReg2;


int main(void)
{    
    LabSPI mySPI;

    uint8_t manID;
    uint8_t devID[2];

    statReg1 myStatReg1;
    statReg2 myStatReg2;

    mySPI.init(LabSPI::SSP1, 7, 1, 8);
    
    mySPI.selectChip();
    mySPI.transfer(0x9F);               //send request for device info
    manID = mySPI.transfer(0);          //read manufacture id
    devID[0] = mySPI.transfer(0);       //read byte 1 of device id
    devID[1] = mySPI.transfer(0);       //read byte 2 of device id
    mySPI.deselectChip();

    printf("\nManufacture ID:   %x\n", manID);
    printf("Device ID:        %x%x\n", devID[0], devID[1]);
    
    mySPI.selectChip();                     
    mySPI.transfer(0xD7);                   //request status register
    myStatReg1.byte = mySPI.transfer(0);    //recieve status register byte 1
    myStatReg2.byte = mySPI.transfer(0);    //recieve status register byte 1
    mySPI.deselectChip();

    printf("\n-------------------Status Register Info-------------------\n");

    if(myStatReg1.rdy1)
    {
        printf("Ready/Busy Status = 1. Device is ready.\n");
    }
    else
    {
        printf("Ready/Busy Status = 0. Device is busy with an internal operation.\n");
    }
    if(myStatReg1.comp)
    {
        printf("Compare Result = 1. Main memory page data does not match buffer data.\n");
    }
    else
    {
        printf("Compare result = 0. Main memory page data matches buffer data.\n");
    }
    //density 5:2
    if(myStatReg1.protect)
    {
        printf("Sector protection status = 1. It is enabled.\n");
    }
    else
    {
        printf("Sector protection status = 0. It is disabled.\n");
    }
    if(myStatReg1.page_size)
    {
        printf("Page size configuration = 1. Device is configured for standard DataFlash page size (528 bytes).\n");
    }
    else
    {
        printf("Page size configureation = 0. Device is configured for power of 2 binary page size (512 bytes).\n");
    }
    if(myStatReg2.rdy2)
    {
        printf("Ready/Busy Status = 1. Device is ready to use.\n");
    }
    else
    {
        printf("Ready/Busy Status = 0. Device is busy with an internal operation.\n");
    }
    if(myStatReg2.epe)
    {
        printf("Erase/Program Error = 1. Erase or program error detected.\n");
    }
    else
    {
        printf("Erase/Program Error = 0. Erase or program operation was successful.\n");
    }
    if(myStatReg2.sle)
    {
        printf("Sector Lockdown Enabled = 1.\n");
    }
    else
    {
        printf("Sector Lockdown disabled = 0.\n");
    }
    if(myStatReg2.ps2)
    {
        printf("Program suspend status buffer 2 = 1. A sector is program suspended while using Buffer 2.\n");
    }
    else
    {
        printf("Program suspend status buffer 2 = 0. No program is suspended while using Buffer 2.\n");
    }
    if(myStatReg2.ps1)
    {
        printf("Program suspend status buffer 1 = 1. A sector is program suspended while using Buffer 1.\n");
    }
    else
    {
        printf("Program suspend status 1 = 0. No program is suspended while using Buffer 1.\n");
    }
    if(myStatReg2.es)
    {
        printf("Erase suspend = 1. A sector is erase suspended.\n");
    }
    else
    {
        printf("Erase suspend = 0. No sectors are erase suspended.\n");
    }
    
    return -1;
}
*/

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
    vTaskDelay(1000);
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
    vTaskDelay(1000);
    int item = 0;
    while(1)
    {
        puts("Going to send an item.");
        xQueueSend(qh, &item, 0);
        puts("Did I send an item?");

        xQueueSend(qh, &item, portMAX_DELAY);
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
