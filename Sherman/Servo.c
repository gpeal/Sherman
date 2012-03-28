#include "PWM.h"
#include "LCD.h"
#include <plib.h>

void setupServo(int timer, int outputControl)
{
    int oc_timerX_src = OC_TIMER2_SRC;
    //priority 3 for no particular reason (purposefully not 1)
    //5hz
    setupTimer(timer, 5, 3);

    switch(timer)
    {
        case 2:
            oc_timerX_src = OC_TIMER2_SRC;
            break;
        case 3:
            oc_timerX_src = OC_TIMER3_SRC;
            break;
    }

    switch(outputControl)
    {
        case 1:
            OpenOC1(OC_ON | oc_timerX_src | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
        case 2:
            OpenOC2(OC_ON | oc_timerX_src | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
        case 3:
            OpenOC3(OC_ON | oc_timerX_src | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
        case 4:
            OpenOC4(OC_ON | oc_timerX_src | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
        case 5:
            OpenOC5(OC_ON | oc_timerX_src | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
        default:
            OpenOC1(OC_ON | oc_timerX_src | OC_PWM_FAULT_PIN_DISABLE, 0, 0); // PWM output on Pin D0, 0 duty cycle
            break;
    }

    setServoPosition(outputControl, 90);
}

void setServoPosition(int outputControl, int angle)
{
    /*
      range of pulse = 0.9 to 2.1ms so 1.2ms
      multiply by 1024 to get the duty cycle
      0.9ms to 2.1ms pulses translates to a duty cycle
      of 47-108 so you have to scale the angle down to a range
      of 47-108
    */
    int dutyCycle = angle; // (double)angle/180*61+47;
    sprintf(LCDBuffer, "Duty Cycle: %d", dutyCycle);
    LCDWriteString(LCDBuffer, 2, 1);
    //set it to neutral
    switch(outputControl)
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

