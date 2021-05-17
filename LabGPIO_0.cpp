#include "LabGPIO_0.hpp"
#include "LPC17xx.h"

     	LabGPIO_0::LabGPIO_0(uint8_t port, uint8_t pin)
      {
        thePort = port;
        thePin = pin;
      }

         /**
          * Should alter the hardware registers to set the pin as an input
          */
     	void LabGPIO_0::setAsInput()
       {
         if(thePort == 0)
         {
           LPC_GPIO0->FIODIR &= ~(1 << thePin); //set to 0
         }
         else if(thePort == 1)
         {
           LPC_GPIO1->FIODIR &= ~(1 << thePin); //set to 0
         }
         else if(thePort == 2)
         {
           LPC_GPIO2->FIODIR &= ~(1 << thePin); //set to 0
         }
         else if(thePort == 3)
         {
           LPC_GPIO3->FIODIR &= ~(1 << thePin); //set to 0
         }
         else
         {
           //printf("Use port 0-3");
         }
       }

         /**
          * Should alter the hardware registers to set the pin as an output
          */
     	void LabGPIO_0::setAsOutput()
       {
         if(thePort == 0)
         {
           LPC_GPIO0->FIODIR |= (1 << thePin); //set to 1
         }
         else if(thePort == 1)
         {
           LPC_GPIO1->FIODIR |= (1 << thePin); //set to 1
         }
         else if(thePort == 2)
         {
           LPC_GPIO2->FIODIR |= (1 << thePin); //set to 1
         }
         else if(thePort == 3)
         {
           LPC_GPIO3->FIODIR |= (1 << thePin); //set to 1
         }
         else
         {
           //printf("Use ports 0-3");
         }
       }

       /**
        * Should alter the set the direction output or input depending on the input.
        *
        * @param {bool} output - true => output, false => set pin to input
        */
       void LabGPIO_0::setDirection(bool output)
       {
           if(output)
           {
             setAsOutput();
           }
           else
           {
             setAsInput();
           }
       }

       /**
        * Should alter the hardware registers to set the pin as high
        */
       void LabGPIO_0::setHigh()
       {
         if(thePort == 0)
         {
           LPC_GPIO0->FIOSET = (1 << thePin); //set to 1
         }
         else if(thePort == 1)
         {
           LPC_GPIO1->FIOSET = (1 << thePin); //set to 1
         }
         else if(thePort == 2)
         {
           LPC_GPIO2->FIOSET = (1 << thePin); //set to 1
         }
         else if(thePort == 3)
         {
           LPC_GPIO3->FIOSET = (1 << thePin); //set to 1
         }
         else
         {
           //printf("Use ports 0-3");
         }
       }

       /**
        * Should alter the hardware registers to set the pin as low
        */
       void LabGPIO_0::setLow()
       {
         if(thePort == 0)
         {
           LPC_GPIO0->FIOCLR = (1 << thePin); //set to 0
         }
         else if(thePort == 1)
         {
           LPC_GPIO1->FIOCLR = (1 << thePin); //set to 0
         }
         else if(thePort == 2)
         {
           LPC_GPIO2->FIOCLR = (1 << thePin); //set to 0
         }
         else if(thePort == 3)
         {
           LPC_GPIO3->FIOCLR = (1 << thePin); //set to 0
         }
         else
         {
           //printf("Use ports 0-3");
         }
       }

       /**
        * Should alter the hardware registers to set the pin as low
        *
        * @param {bool} high - true => pin high, false => pin low
        */
       void LabGPIO_0::set(bool high)
       {
         if(high)
         {
           setHigh();
         }
         else
         {
           setLow();
         }
       }

       /**
        * Should return the state of the pin (input or output, doesn't matter)
        *
        * @return {bool} level of pin high => true, low => false
        */
     	bool LabGPIO_0::getLevel()
      {
        bool level = false;
        if(thePort == 0)
        {
          if(LPC_GPIO0->FIOPIN & (1 << thePin))
          {
            level = true;
          }
          else
          {
            level = false;
          }
        }
        else if(thePort == 1)
        {
          if(LPC_GPIO1->FIOPIN & (1 << thePin))
          {
            level = true;
          }
          else
          {
            level = false;
          }
        }
        else if(thePort == 2)
        {
          if(LPC_GPIO2->FIOPIN & (1 << thePin))
          {
            level = true;
          }
          else
          {
            level = false;
          }
        }
        else if(thePort == 3)
        {
          if(LPC_GPIO3->FIOPIN & (1 << thePin))
          {
            level = true;
          }
          else
          {
            level = false;
          }
        }
        else
        {
          //printf("Use ports 0-3");
        }

        return level;
      }
     	LabGPIO_0::~LabGPIO_0()
      {

      }
