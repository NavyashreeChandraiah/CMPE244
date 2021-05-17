#include <ADCDriver.hpp>

/**
* Nothing needs to be done within the default constructor
*/
ADCDriver::ADCDriver(){}

/**
* 1) Powers up ADC peripheral
* 2) Set peripheral clock
* 2) Enable ADC
* 3) Select ADC channels
* 4) Enable burst mode
*
* @param channels_to_enable (optional param) bit mask to enable up to 8 ADC channels
*/
void ADCDriver::adcInitBurstMode(uint8_t channels_to_enable)
{
    LPC_SC->PCONP |= (1 << 12);     //power up ADC
    LPC_ADC->ADCR |= (1 << 11);     //enable ADC

    LPC_SC->PCLKSEL0 |= (1 << 24);  //PCLK_peripheral = CLK/8 (00)
    LPC_SC->PCLKSEL0 |= (1 << 25);

    LPC_ADC->ADCR &= ~(0xFF);       //clear all channels
    LPC_ADC->ADCR |= (1 << 3);       //select channel 3 (p0.26)

    LPC_ADC->ADCR |= (1 << 16);     //enable burst mode
}

/**
* 1) Selects ADC functionality of any of the 8 pins that are ADC capable
* 
* @param adc_pin_arg is the ADC_PIN enumeration of the desired pin.
*
* WARNING: For proper operation of the SJOne board, do NOT configure any pins
*           as ADC except for 0.26, 1.31, 1.30
*/
void ADCDriver::adcSelectPin(uint8_t adc_pin_arg)//3 digit number for port pin pin
{
    if(adc_pin_arg == 3)
    {
        LPC_PINCON->PINSEL1 |= (1 << 20);   //select p0.26 as adc (01)
        LPC_PINCON->PINSEL1 &= ~(1 << 21);
    }
    else if(adc_pin_arg == 4)
    {
        LPC_PINCON->PINSEL3 |= (1 << 28);   //select p1.30 as adc (11)
        LPC_PINCON->PINSEL3 |= (1 << 29);
    }
    else if(adc_pin_arg == 5)
    {
        LPC_PINCON->PINSEL3 |= (1 << 30);   //select p1.31 as adc (11)
        LPC_PINCON->PINSEL3 &= (1 << 31);
    }
    else
    {
        //wong pin
    }
}

/**
* 1) Returns the value of the 12bit register reading of a given ADC pin
*
* @param adc_pin_arg is the ADC_PIN enumeration of the desired pin.
*/
uint16_t ADCDriver::readADCRawByPin(uint8_t adc_pin_arg)//not done
{
    uint16_t rawData;

    if(adc_pin_arg == 3)
    {
        rawData = (LPC_ADC->ADDR3 >> 4) & 0xFFF;
    }
    else if(adc_pin_arg == 4)
    {
        rawData = (LPC_ADC->ADDR4 >> 4) & 0xFFF;
    }
    else if(adc_pin_arg == 5)
    {
        rawData = (LPC_ADC->ADDR5 >> 4) & 0xFFF;
    }
    else
    {
        //wong pin
    }
    return rawData;   
}

/**
* 1) Returns the value of the 12bit register reading of a given ADC channel
*
* @param adc_channel_arg is the number (0 through 7) of the desired ADC channel.
*/
uint16_t ADCDriver::readADCRawByChannel(uint8_t adc_channel_arg)
{
    uint16_t rawData;

    if(adc_channel_arg == 3)
    {
        rawData = (LPC_ADC->ADDR3 >> 4) & 0xFFF;
    }
    else if(adc_channel_arg == 4)
    {
        rawData = (LPC_ADC->ADDR4 >> 4) & 0xFFF;
    }
    else if(adc_channel_arg == 5)
    {
        rawData = (LPC_ADC->ADDR5 >> 4) & 0xFFF;
    }
    else
    {
        //wong pin
    }
    return rawData;    
}

/**
* 1) Returns the voltage reading of a given ADC pin
*
* @param adc_pin_arg is the ADC_PIN enumeration of the desired pin.
*/
float ADCDriver::readADCVoltageByPin(uint8_t adc_pin_arg)
{
    float voltage = readADCRawByPin(adc_pin_arg);
    voltage = voltage/29.57;
    return voltage;
}

/**
* 1) Returns the voltage reading of a given ADC channel
*
* @param adc_channel_arg is the number (0 through 7) of the desired ADC channel.
*/
float ADCDriver::readADCVoltageByChannel(uint8_t adc_channel_arg)
{
    float voltage = readADCRawByChannel(adc_channel_arg);
    voltage = voltage/29.57;
    return voltage;
}
