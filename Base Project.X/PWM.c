#include "PWM.h"
#include <plib.h>


void setupPWMTimer()
{
    OpenTimer2(T2_ON | T2_PS_1_4, 1000); // 20kHz PWM: 80000000Hz / 4ps / 1000 = 20 kHz
}

void setupPWM(int outputControlX)
{

    switch(outputControlX)
    {
        case 1:
            OpenOC1(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
        case 2:
            OpenOC2(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
        case 3:
            OpenOC3(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
        case 4:
            OpenOC4(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
        case 5:
            OpenOC5(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
        default:
            OpenOC1(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
    }

}

void setDutyCycle(int outputControlX, int dutyCycle)
{
    switch(outputControlX)
    {
        case 1:
            SetDCOC1PWM(dutyCycle);
            break;
        case 2:
            SetDCOC2PWM(dutyCycle);
            break;
        case 3:
            SetDCOC3PWM(dutyCycle);
            break;
        case 4:
            SetDCOC4PWM(dutyCycle);
            break;
        case 5:
            SetDCOC5PWM(dutyCycle);
            break;
        default:
            SetDCOC1PWM(dutyCycle);
            break;
    }

}