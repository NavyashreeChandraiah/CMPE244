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
#include "uart0_min.h"  //uart0_puts("UH")
#include "LPC17xx.h"
#include "cstdint"
#include <stdlib.h>
#include <unistd.h>
#include "ADCDriver.hpp"
#include "PWMDriver.hpp"



inline bool CHECK_BIT(int var, int pos)
{
    return (bool)(var & (1 << pos));
}

void vTaskLED(void * pvParameters)
{
    ADCDriver * adcDriver = static_cast<ADCDriver *>(pvParameters);
    float rawData;
    float rBrightness = 0, gBrightness = 0, bBrightness = 0;
    PWMDriver PWMr, PWMg, PWMb;

    PWMr.pwmSelectPin(0);
    PWMr.pwmInitSingleEdgeMode(100);

    PWMg.pwmSelectPin(1);
    PWMg.pwmInitSingleEdgeMode(100);

    PWMb.pwmSelectPin(2);
    PWMb.pwmInitSingleEdgeMode(100);


    uint8_t mode = 1;
    int count = 0;
    float voltage = 0;
    while(1)
    {   
        vTaskDelay(1);
        count++;
        if(count == 1000)
        {
            voltage = rawData/29.57;
            printf("\nADC voltage: %f\n", voltage);
            printf("Red Duty Cycle: %f,\nGreen Duty Cycle: %f,\nBlue Duty Cycle: %f\n", (100 - rBrightness), (100 - gBrightness), (100 - bBrightness));
            count = 0;
        }
        //vTaskDelay(1000);
        if(SW.getSwitch(1))
        {
            mode = 1;
        }
        else if(SW.getSwitch(2))
        {
            mode = 2;
        }
        else if(SW.getSwitch(3))
        {
            mode = 3;
        }

        switch(mode)
        {
            case 1:
                 LPC_PINCON->PINSEL4 &= 0;      //deselect all colors
                 break;
            case 2:
                PWMr.pwmSelectPin(0);
                PWMg.pwmSelectPin(1);
                PWMb.pwmSelectPin(2);
                rawData = (adcDriver->readADCRawByChannel(3));
                //printf("rawData: %f", rawData);
                rawData = rawData/41;
                rBrightness = rawData;
                bBrightness = 100 - rawData;
                if(rawData <= 50)
                {
                    gBrightness = 100 - (2 * rBrightness);
                }
                else if(rawData > 50)
                {
                    gBrightness = 100 - (2 * bBrightness);
                }

                PWMr.setDutyCycle(0, rBrightness);
                PWMg.setDutyCycle(1, gBrightness);
                PWMb.setDutyCycle(2, bBrightness);
                //printf("rBrightness: %f, gBrightness: %f, bBrightness: %f\n", rBrightness, gBrightness, bBrightness);
                break;
            case 3:
                LPC_PINCON->PINSEL4 = 0x10;    //turn off all but blue pwm
                vTaskDelay(100);
                rawData = (adcDriver->readADCRawByChannel(3))/42;
                bBrightness = rawData;

                PWMb.setDutyCycle(2, bBrightness);
                printf("bBrightness: %f\n", bBrightness);
                break;
            default:
                mode = 1;
                break;
        }
    }
}


int main(void)
{
    ADCDriver * frontSeat;
    frontSeat->adcInitBurstMode(3);
    frontSeat->adcSelectPin(3);

    scheduler_add_task(new terminalTask(PRIORITY_HIGH));
	xTaskCreate(vTaskLED, "vTaskLED", 512, ( void * ) frontSeat, 1, NULL );

    //xTaskCreate(vTaskPrint, "vTaskPrint", 512, ( void * ) 'A', 1, NULL );
    // Alias to vSchedulerStart();
    scheduler_start();
    return -1;
}
