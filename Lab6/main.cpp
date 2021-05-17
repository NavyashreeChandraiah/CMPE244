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
#include "i2c2.hpp"
#include "i2c_base.hpp"
#include "printf_lib.h"

inline bool CHECK_BIT(int var, int pos)
{
    return (bool)(var & (1 << pos));
}

void vTaskCode(void * pvParameters)
{
    char c = (char)((uint32_t)pvParameters);
    while(1)
    {
        for(int i = 0; i < 16; i++)
        {
            for(int j = 1; j < 5; j++)
            {
                LE.set((5-j), CHECK_BIT(i,j-1));
            }
            LD.setNumber(i);
            // printf("(%c) Hello World 0x%X\n", c, i);
	    	vTaskDelay(1000);
        }
    }
}



/*
 * I2CONSET bits
 * 0x04 AA
 * 0x08 SI
 * 0x10 STOP
 * 0x20 START
 * 0x40 ENABLE
 *
 * I2CONCLR bits
 * 0x04 AA
 * 0x08 SI
 * 0x20 START
 * 0x40 ENABLE
 */
I2C_Base::mStateMachineStatus_t I2C_Base::i2cStateMachine()
{
    
    enum
    {
        // General states :
        busError        = 0x00,
        start           = 0x08,
        repeatStart     = 0x10,
        arbitrationLost = 0x38,

        // Master Transmitter States:
        slaveAddressAcked  = 0x18,
        slaveAddressNacked = 0x20,
        dataAckedBySlave   = 0x28,
        dataNackedBySlave  = 0x30,

        readAckedBySlave      = 0x40,
        // Master Receiver States:
        readModeNackedBySlave = 0x48,
        dataAvailableAckSent  = 0x50,
        dataAvailableNackSent = 0x58,

        //slave states...

        //slave Receiver
        addressAcked            = 0x60,
        arbitrationWriteAddressAcked = 0x68,
        generalCallAcked        = 0x70,
        dataReceivedAcked       = 0x80,
        dataReceivedNacked      = 0x88,
        generaldataRecAcked     = 0x90,
        SorPReceived            = 0xA0,

        //slave Transmitter
        addressReceived         = 0xA8,
        arbitrationReadAddressAcked = 0xB0,
        dataSentAckReceived     = 0xB8,
        LastByteSentNacked      = 0xC0,
        LastByteSentAcked      = 0xC8

    };

    state = busy;

    /*
     ***********************************************************************************************************
     * Write-mode state transition :
     * start --> slaveAddressAcked --> dataAckedBySlave --> ... (dataAckedBySlave) --> (stop)
     *
     * Read-mode state transition :
     * start --> slaveAddressAcked --> dataAcked --> repeatStart --> readAckedBySlave
     *  For 2+ bytes:  dataAvailableAckSent --> ... (dataAvailableAckSent) --> dataAvailableNackSent --> (stop)
     *  For 1  byte :  dataAvailableNackSent --> (stop)
     ***********************************************************************************************************
     */

    switch (mpI2CRegs->I2STAT)
    {
        case start:
            //u0_dbg_printf("sta\n");
            if(mTransaction.writeLength == 0)
            {
                //u0_dbg_printf("ard-%X\n", I2C_READ_ADDR(mTransaction.slaveAddr));
                mpI2CRegs->I2DAT = I2C_READ_ADDR(mTransaction.slaveAddr);
            }
            else
            {
                //u0_dbg_printf("awr-%X\n", I2C_WRITE_ADDR(mTransaction.slaveAddr));
                mpI2CRegs->I2DAT = I2C_WRITE_ADDR(mTransaction.slaveAddr);
            }
            clearSIFlag();
            break;
        case repeatStart:
            //u0_dbg_printf("rsta-%X\n", I2C_READ_ADDR(mTransaction.slaveAddr));
            mpI2CRegs->I2DAT = I2C_READ_ADDR(mTransaction.slaveAddr);
            clearSIFlag();
            break;

        case slaveAddressAcked:
            //u0_dbg_printf("sack\n");
            clearSTARTFlag();
            // No data to transfer, this is used just to test if the slave responds
            if (0 == mTransaction.readLength && 0 == mTransaction.writeLength)
            {
                //u0_dbg_printf("sto\n");
                setStop();
            }
            else if(0 != mTransaction.writeLength)
            {
                mpI2CRegs->I2DAT = *(mTransaction.dataWrite);
                ++mTransaction.dataWrite;
                --mTransaction.writeLength;
                clearSIFlag();

                //u0_dbg_printf("1st-%X\n", mpI2CRegs->I2DAT);
            }
            break;

        case dataAckedBySlave:
            if (0 == mTransaction.writeLength)
            {
                //u0_dbg_printf("wend\n");
                if (I2C_CHECK_READ_MODE(mTransaction.slaveAddr))
                {
                    //u0_dbg_printf("rread\n");
                    setSTARTFlag(); // Send Repeat-start for read-mode
                    clearSIFlag();
                }
                else
                {
                    //u0_dbg_printf("sto\n");
                    setStop();
                }
            }
            else
            {
                mpI2CRegs->I2DAT = *(mTransaction.dataWrite);
                ++mTransaction.dataWrite;
                --mTransaction.writeLength;
                clearSIFlag();
                //u0_dbg_printf("wlo-%X\n", mpI2CRegs->I2DAT);
            }
            break;

        /* In this state, we are about to initiate the transfer of data from slave to us
         * so we are just setting the ACK or NACK that we'll do AFTER the byte is received.
         */
        case readAckedBySlave:
            clearSTARTFlag();
            if (mTransaction.readLength > 1)
            {
                //u0_dbg_printf("ack\n");
                setAckFlag();  // 1+ bytes: Send ACK to receive a byte and transition to dataAvailableAckSent
            }
            else
            {
                //u0_dbg_printf("nack\n");
                setNackFlag();  //  1 byte : NACK next byte to go to dataAvailableNackSent for 1-byte read.
            }
            clearSIFlag();
            break;
        case dataAvailableAckSent:
            *mTransaction.dataRead = mpI2CRegs->I2DAT;
            ++mTransaction.dataRead;
            --mTransaction.readLength;

            //u0_dbg_printf("rdat-%X\n", mpI2CRegs->I2DAT);

            if (1 == mTransaction.readLength)  // Only 1 more byte remaining
            {
                //u0_dbg_printf("nack\n");
                setNackFlag();// NACK next byte --> Next state: dataAvailableNackSent
            }
            else
            {
                //u0_dbg_printf("ack\n");
                setAckFlag(); // ACK next byte --> Next state: dataAvailableAckSent(back to this state)
            }

            clearSIFlag();
            break;
        case dataAvailableNackSent: // Read last-byte from Slave
            //u0_dbg_printf("nack-sto-%X\n", mpI2CRegs->I2DAT);
            *mTransaction.dataRead = mpI2CRegs->I2DAT;
            setStop();
            break;

        case arbitrationLost:
            // We should not issue stop() in this condition, but we still need to end our  transaction.
            state = I2C_CHECK_READ_MODE(mTransaction.slaveAddr) ? readComplete : writeComplete;
            mTransaction.error = mpI2CRegs->I2STAT;
            break;

        case slaveAddressNacked:    // no break
        case dataNackedBySlave:     // no break
        case readModeNackedBySlave: // no break
        case busError:              // no break

        //slave receiver states
        case addressAcked:
            u0_dbg_printf("In state 0x60\n");
            setAckFlag();
            clearSIFlag();
            mTransaction.slaveRegPosition = 256;
            mTransaction.slaveByteCounter = 0;
            mTransaction.slaveDataBuffer = mTransaction.slaveOriginPointer;
            break;

        case arbitrationWriteAddressAcked:
            u0_dbg_printf("In state 0x68\n");
            setSTARTFlag();
            setAckFlag();
            clearSIFlag();
            break;
        
        case generalCallAcked:
            u0_dbg_printf("In state 0x70\n");

            break;

        case dataReceivedAcked:
            u0_dbg_printf("In state 0x80\n");
            if(mTransaction.slaveByteCounter == 0)                      
            {
                mTransaction.slaveRegPosition = mpI2CRegs->I2DAT;       //reading the first byte to get register address
                mTransaction.slaveByteCounter++;
                setAckFlag();
                clearSIFlag();
            }
            else                                                                    
            {
                mTransaction.slaveDataBuffer += mTransaction.slaveRegPosition;
                *(mTransaction.slaveDataBuffer) = mpI2CRegs->I2DAT;
                mTransaction.slavedataCount--;
                mTransaction.slaveByteCounter++;
                if(mTransaction.slavedataCount == 0)
                {
                    setNackFlag();
                    clearSIFlag();
                    break;
                }
                mTransaction.slaveDataBuffer++;
                setAckFlag();
                clearSIFlag();
            }
            break;

        case dataReceivedNacked:
            u0_dbg_printf("In state 0x88\n");
            setAckFlag();
            clearSIFlag();
            break;

        case generaldataRecAcked:
            u0_dbg_printf("In state 0x90\n");
            *(mTransaction.slaveDataBuffer) = mpI2CRegs->I2DAT;
            setNackFlag();
            clearSIFlag();
            break;

        case SorPReceived:
            u0_dbg_printf("In state 0xA0\n");
            setAckFlag();
            clearSIFlag();
            break;

        
        //slave transmitter states
        case addressReceived:
            u0_dbg_printf("In state 0xA8\n");

            break;

        case arbitrationReadAddressAcked:
            u0_dbg_printf("In state 0xB0\n");

            break;

        case dataSentAckReceived:
            u0_dbg_printf("In state 0xBB\n");

            break;

        case LastByteSentNacked:
            u0_dbg_printf("In state 0xC0\n");

            break;

        case LastByteSentAcked:
            u0_dbg_printf("In state 0xC8\n");

            break;

        default:
            mTransaction.error = mpI2CRegs->I2STAT;
            setStop();
            break;
    }

    return state;
}

void I2C_Base::initSlave(uint8_t slaveAddr, uint8_t* buff, uint32_t size)
{
    mpI2CRegs->I2ADR0 = slaveAddr;
    mpI2CRegs->I2ADR1 = 0;
    mpI2CRegs->I2ADR2 = 0;
    mpI2CRegs->I2ADR3 = 0;

    mpI2CRegs->I2MASK0 = 0;
    mpI2CRegs->I2MASK1 = 0;
    mpI2CRegs->I2MASK2 = 0;
    mpI2CRegs->I2MASK3 = 0;

    mpI2CRegs->I2CONSET = 0x44;
    NVIC_EnableIRQ(mIRQ);

    mTransaction.slaveOriginPointer = &buff[0];
    mTransaction.slavedataCount = size;
}

int main(void)
{
    I2C2& i2c = I2C2::getInstance(); // Get I2C driver instance
    const uint8_t slaveAddr = 0xC0;  // Pick any address other than an existing one at i2c2.hpp
    uint8_t buffer[256] = { 0 }; // Our slave read/write buffer (This is the memory your other master board will read/write)

    // I2C is already initialized before main(), so you will have to add initSlave() to i2c base class for your slave driver
    i2c.initSlave(slaveAddr, &buffer[0], sizeof(buffer));

    // I2C interrupt will (should) modify our buffer.
    // So just monitor our buffer, and print and/or light up LEDs
    // ie: If buffer[0] == 0, then LED ON, else LED OFF
    uint8_t prev = buffer[0];
    while(1)
    {
        if (prev != buffer[0]) {
            prev = buffer[0];
            printf("buffer[0] changed to %#x by the other Master Board\n", buffer[0]);
        }
    }

    return 0;
    scheduler_add_task(new terminalTask(PRIORITY_HIGH));
	xTaskCreate(vTaskCode, "vTaskCode", 512, ( void * ) 'A', tskIDLE_PRIORITY, NULL );
    // Alias to vSchedulerStart();
    scheduler_start();
    return -1;
}
