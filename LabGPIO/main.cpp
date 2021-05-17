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
#include "LabGPIO_0.hpp"
#include "uart0_min.h"

bool turnInternalLEDon;
bool turnExternalLEDon;


void vControlLED( void * pvParameters )
{
    /* Get Parameter */
    uint32_t param = ((uint32_t)(pvParameters));
    uint8_t port = param/10;
    uint8_t pin = param%10;
    turnInternalLEDon = false;
    turnExternalLEDon = true;
    LabGPIO_0 LED (port, pin);
    LED.setAsOutput();
    while(1)
    {
      if(port == 1)
      {
        if(turnInternalLEDon)
        {
          LED.setLow();
        }
        else if(!turnInternalLEDon)
        {
          LED.setHigh();
        }
      }
      else if(port == 0)
      {
        if(turnExternalLEDon)
        {
          LED.setLow();
        }
        else if(!turnExternalLEDon)
        {
          LED.setHigh();
        }
      }
        /* Insert Loop Code */
    }
}

void vReadSwitch( void * pvParameters )
{

    uint32_t param = ((uint32_t)(pvParameters));
    uint8_t port = param/10;
    uint8_t pin = param%10;
    turnInternalLEDon = false;
    LabGPIO_0 Switch (port, pin);
    Switch.setAsInput();

    while(1)
    {
      if(port == 1)                     //internal LED
      {
        while(Switch.getLevel())       //while button is pressed
        {
          vTaskDelay(100);
          if(!Switch.getLevel())       //check for when it will be released
          {
            turnInternalLEDon = !turnInternalLEDon;
          }
        }
      }
      else if(port == 0)          //externalLED
      {
        while(Switch.getLevel())       //while button is pressed
        {
          vTaskDelay(100);
          if(!Switch.getLevel())       //check for when it will be released
          {
            turnExternalLEDon = !turnExternalLEDon;
          }
        }
      }
    }
}

int main(void)
{

    scheduler_add_task(new terminalTask(PRIORITY_HIGH));
	  //xTaskCreate(vTaskCode, "vTaskCode", 512, ( void * ) 'A', tskIDLE_PRIORITY, NULL );
    xTaskCreate(vControlLED, "vControlInternalLed", 512, ( void *) 10, 1, NULL);
    xTaskCreate(vReadSwitch, "vReadInternalSwitch", 512, ( void *) 19, 1, NULL);

    xTaskCreate(vControlLED, "vControlExternalLed", 512, ( void *) 01, 1, NULL);
    xTaskCreate(vReadSwitch, "vReadExternalSwitch", 512, ( void *) 00, 1, NULL);
    // Alias to vSchedulerStart();
    scheduler_start();
    return -1;
}
