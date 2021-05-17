
    #include "UartDriver.hpp"

    #include "queue.h"

  

    UartDriver::UartDriver()
    {
        periph = 3;
    }
    //UartDriver::~UartDriver(){}

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
    bool UartDriver::UartInit(uint8_t peripheral, uint32_t baudRate)
    {
        periph = peripheral;
        uint32_t temp = 48000000 / (16 * baudRate);
        LPC_SC->PCLKSEL1 &= ~(3 << 18);                     //clear
        LPC_SC->PCLKSEL1 |= (1 << 18);                      //set clock for UART3 = pclk/8 (01)

        if(peripheral == 3)
        {
            LPC_SC->PCONP |= (1 << 25);                         //enable UART3
            LPC_UART3->LCR = (1 << 7);                          //DLAB enable
            LPC_UART3->LCR = 3;                                 //8 bit transfer
            LPC_UART3->FCR |= (1 << 0);                         //enable the fifo for uart
            LPC_UART3->DLL = temp;                              //setting baud rate
            LPC_UART3->DLM = (temp << 8);
            LPC_UART3->LCR &= ~(1 << 7);                        //DLAB disable
            LPC_PINCON->PINSEL9 |= (3 << 24) | (3 << 26);       //4.28 as TXD3 4.29 as RXD3 (11)

            LPC_UART3->IER |= 1;                            //enable interupts for uart3 for rbr
            NVIC_EnableIRQ(UART3_IRQn);
        }
        else
        {
            LPC_SC->PCONP |= (1 << 24);                         //enable UART2
            LPC_UART2->LCR = (1 << 7);                          //DLAB enable
            LPC_UART2->LCR = 3;                                 //8 bit transfer
            LPC_UART2->FCR |= (1 << 0);                         //enable the fifo for uart
            LPC_UART2->DLL = temp;                              //setting baud rate
            LPC_UART2->DLM = (temp << 8);
            LPC_UART2->LCR &= ~(1 << 7);                        //DLAB disable
            LPC_PINCON->PINSEL4 &= ~(1 << 16) & ~(1 << 18);     //P2.8 as TXD2 P2.9 as RXD2 (10)
            LPC_PINCON->PINSEL4 |= (1 << 17) | (1 << 19);         

            LPC_UART2->IER |= 1;                            //enable interupts for uart2 for rbr
            NVIC_EnableIRQ(UART2_IRQn);   
        }
        return true;
    }

    /**
     * Sends a character
     * 
     * @param send the character you want to send
     * 
     */ 
    void UartDriver::transmitChar(uint8_t send)
    {
        if(periph == 2)
        {
            LPC_UART2->THR = send;
            while(!(LPC_UART2->LSR & (1 << 5))){}       //wait for end of transmission
        }
        else
        {
            LPC_UART3->THR = send;
            while(!(LPC_UART3->LSR & (1 << 5))){}       //wait for end of transmission
        }
        //printf("Sent %c, ", send);
    }

    /**
     * gets a char and returns a uint8
     * 
     */ 
    char UartDriver::receiveChar()
    {
        char data;
        if(periph == 2)
        {
            //while(! (LPC_UART2->LSR & (1 << 0)));   //wait for character to read
            data = LPC_UART2->RBR;
        }
        else
        {
            //while(! (LPC_UART3->LSR & (1 << 0)));   //wait for character to read
            data = LPC_UART3->RBR;
        }
        LPC_UART3->IER &= ~(1 << 0);                    //clear interupts for uart3 for rbr
        LPC_UART3->IER |= 1;                            //enable interupts for uart3 for rbr
        return data;
        //printf("Received: %c\n", data);     
    }
