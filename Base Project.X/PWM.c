#include "PWM.h"
#include <plib.h>

void setupPWM()
{
    OpenTimer2(T2_ON | T2_PS_1_4, 1000); // 20kHz PWM: 80000000Hz / 4ps / 1000 = 20 kHz
    OpenOC1(OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
}

void setDutyCycle(int dutyCycle)
{
    SetDCOC1PWM(dutyCycle);
}