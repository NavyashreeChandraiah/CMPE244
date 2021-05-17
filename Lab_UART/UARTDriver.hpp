#ifndef UARTDRIVER_H
#define UARTDRIVER_H

#include "FreeRTOS.h"
#include "LPC17xx.h"
#include "stdint.h"
#include "stdio.h"
#include "lpc_isr.h"


class UartDriver
{
private:
    uint8_t periph;

public:
    UartDriver();
    ~UartDriver();

    /**
     * 1) Powers on UART peripheral
     * 2) Select which UART 0/2/3 you want
     * 3) Sets pins for specified UART0/2/3
     * 4) Set baud rate
     * 
     * @param peripheral which peripheral UART0/2/3 you want to select.
     * @param pinSelect which pins you want to select
     * @param baudRate which baudrate you want to select
     *
     * @return true if initialization was successful
     */
    bool UartInit(uint8_t peripheral, uint32_t baudRate);

    /**
     * Sends a character
     * 
     * @param send the character you want to send
     * 
     */ 
    void transmitChar(uint8_t send);

    /**
     * gets a char and returns a uint8
     * 
     */ 
    char receiveChar();

};




#endif
