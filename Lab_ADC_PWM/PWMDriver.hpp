#include <PWMDriver.hpp>
enum PWM_PIN
{
    PWM_PIN_2_0,    // PWM1.1
    PWM_PIN_2_1,    // PWM1.2
    PWM_PIN_2_2,    // PWM1.3
    PWM_PIN_2_3,    // PWM1.4
    PWM_PIN_2_4,    // PWM1.5
    PWM_PIN_2_5,    // PWM1.6
};


/**
* 1) Select PWM functionality on all PWM-able pins.
*/  
void pwmSelectAllPins()
{
    LPC_PINCON->PINSEL4 = (1 << 0) | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8) | (1 << 10);

}

/**
* 1) Select PWM functionality of pwm_pin_arg
*
* @param pwm_pin_arg is the PWM_PIN enumeration of the desired pin.
*/
void PWMDriver::pwmSelectPin(uint8_t pwm_pin_arg)
{
    switch(pwm_pin_arg)
    {
        case 0:
            LPC_PINCON->PINSEL4 |= 1;
            LPC_PINCON->PINSEL4 &= ~(1 << 1);
            break;
        case 1:
            LPC_PINCON->PINSEL4 |= (1 << 2);
            LPC_PINCON->PINSEL4 &= ~(1 << 3);
            break;
        case 2:
            LPC_PINCON->PINSEL4 |= (1 << 4);
            LPC_PINCON->PINSEL4 &= ~(1 << 5);
            break;
        case 3:
            LPC_PINCON->PINSEL4 |= (1 << 6);
            LPC_PINCON->PINSEL4 &= ~(1 << 7);
            break;
        case 4:
            LPC_PINCON->PINSEL4 |= (1 << 8);
            LPC_PINCON->PINSEL4 &= ~(1 << 9);
            break;
        case 5:
            LPC_PINCON->PINSEL4 |= (1 << 10);
            LPC_PINCON->PINSEL4 &= ~(1 << 11);
            break;
        default:
            LPC_PINCON->PINSEL4 |= 1;
            LPC_PINCON->PINSEL4 &= ~(1 << 1);
            break;
    }
}

/**
* 1) Power up the PWM peripheral
* 2) Set the PWM clock to divide by 1 (to simplify frequency calculation
* 3) Enable timer and prescalar counters. Enable PWM Mode
* 4) Configure Counter to reset when MR0 is matched (i.e. MR0 represents frequency)
* 5) Disable all capture features
* 6) Enable Timer mode and disable counter/capture modes
* 7) Enable single edge mode
* 8) Enable output on all six pwm channels
* 9) Set frequency
* 10) Set all pwm channels to zero duty cycle
*
* @param frequency_Hz is the initial frequency in Hz. 
*/
void PWMDriver::pwmInitSingleEdgeMode(uint32_t frequency_Hz)
{
    LPC_SC->PCONP |= (1 << 6);          //power up pwm peripheral
        
    LPC_SC->PCLKSEL0 |= (1 << 13);      //select clock = pckl/8 (01)
    LPC_SC->PCLKSEL0 |= (1 << 12);
    
    LPC_PWM1->TCR |= 1;                 //enable counter mode
    LPC_PWM1->TCR |= (1 << 2);          //PWM enable

    LPC_PWM1->MCR |= (1 << 1);

    setFrequency(frequency_Hz);

    LPC_PWM1->LER |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);    //put new value into MR1 for next cycle
    
    LPC_SC->PCLKSEL0 &= ~(1 << 13);      //select clock = pckl/1 (01)
    LPC_SC->PCLKSEL0 |= (1 << 12);

    LPC_PWM1->PCR |= (1 << 9) | (1 << 10) | (1 << 11) | (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15);           //enable pwm output for pwm1 bits 9-14 set
    
    //
}

/**
* 1) Convert duty_cycle_percentage to the appropriate match register value (depends on current frequency)
* 2) Assign the above value to the appropriate MRn register (depends on pwm_pin_arg)
*
* @param pwm_pin_arg is the PWM_PIN enumeration of the desired pin.
* @param duty_cycle_percentage is the desired duty cycle percentage.
*/
void PWMDriver::setDutyCycle(uint8_t pwm_pin_arg, float duty_cycle_percentage)
{
    switch(pwm_pin_arg)
    {
        case 0:
            LPC_PWM1->MR1 = duty_cycle_percentage;   // convert 0-4095 to 0-100
            break;
        case 1:
            LPC_PWM1->MR2 = duty_cycle_percentage;
            break;
        case 2:
            LPC_PWM1->MR3 = duty_cycle_percentage;
            break;
        case 3:
            LPC_PWM1->MR4 = duty_cycle_percentage;
            break;
        case 4:
            LPC_PWM1->MR5 = duty_cycle_percentage;
            break;
        case 5:
            LPC_PWM1->MR6 = duty_cycle_percentage;
            break;
        default:
            LPC_PWM1->MR1 = duty_cycle_percentage;
            break;
    }
}

/**
* 1) Convert frequency_Hz to the appropriate match register value
* 2) Assign the above value to MR0
*
* @param frequency_hz is the desired frequency of all pwm pins
*/  	
void PWMDriver::setFrequency(uint32_t frequency_Hz)
{
    LPC_PWM1->MR0 = frequency_Hz;       //cycle time 
}
