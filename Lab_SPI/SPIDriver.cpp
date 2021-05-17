    #include "LabSPI.hpp"
    #include <stdio.h>
    #include "utilities.h"
    #include "io.hpp"


    LabSPI::LabSPI(){}
    /**
     * 1) Powers on SPPn peripheral
     * 2) Set peripheral clock
     * 3) Sets pins for specified peripheral to MOSI, MISO, and SCK
     *
     * @param peripheral which peripheral SSP0 or SSP1 you want to select.
     * @param data_size_select transfer size data width; To optimize the code, look for a pattern in the datasheet
     * @param format is the code format for which synchronous serial protocol you want to use.
     * @param divide is the how much to divide the clock for SSP; take care of error cases such as the value of 0, 1, and odd numbers
     *
     * @return true if initialization was successful
     */
    bool LabSPI::init(Peripheral peripheral, uint8_t data_size_select, uint8_t format, uint8_t divide) //should be 1, 7, 8
    {
        bool err = false;  
        if(divide & 1 || divide == 0)
        {
            err = true;
            return err;
        }
        if(peripheral == 0)
        {
            LPC_SC->PCONP |= (1 << 23); //ssp0
            LPC_SC->PCLKSEL1 &= ~(3 << 10);         //set pclk = pclk
            LPC_SC->PCLKSEL1 |= (1 << 10);
            LPC_PINCON->PINSEL0 |= (2 << 30);               //select pck0
            LPC_PINCON->PINSEL1 |= (2 << 2) | (2 << 4);      //select MOSI0 MISO0
        }
        else //peripheral == 1
        {
            LPC_SC->PCONP |= (1 << 10); //ssp1
            LPC_SC->PCLKSEL0 &= ~(3 << 20);         //set pclk = pclk
            LPC_SC->PCLKSEL0 |= (1 << 20);
            LPC_PINCON->PINSEL0 |= (2 << 14) | (2 << 16) | (2 << 18);      //select MOSI1 MISO1 PCK1

        }

        if(format == 1)
        {            
            LPC_SSP1->CR0 = data_size_select;           //set transfer size data width (7)
            LPC_SSP1->CR1 = (1 << 1);                   //set SSP as master
            LPC_SSP1->CPSR = divide;                    //divide master clock (8)
        }
        else
        {
            err = true;
            return err;
        }
        return err;
    }

    /**
     * Transfers a byte via SSP to an external device using the SSP data register.
     * This region must be protected by a mutex static to this class.
     *
     * @return received byte from external device via SSP data register.
     */

    static SemaphoreHandle_t spi_bus_lock = xSemaphoreCreateMutex();

    uint8_t LabSPI::transfer(uint8_t send)
    {
        if(xSemaphoreTake(spi_bus_lock, 100))
        {
            //printf("Semaphore created...");
            LPC_SSP1->DR = send;
            while (LPC_SSP1->SR & (1 << 4)){}       //wait until done
            xSemaphoreGive(spi_bus_lock);
            //printf("Semaphore given up...");
            return LPC_SSP1->DR;
        }
        
    }
    
    //set gpio 0.6 low to select flash
    void LabSPI::selectChip(void)
    {
        LPC_GPIO0->FIOPIN &= ~(1 << 6);
    }

    //set gpio 0.6 high to deselect flash
    void LabSPI::deselectChip(void)
    {
        LPC_GPIO0->FIOPIN |= (1 << 6);
    }
    LabSPI::~LabSPI(){}
