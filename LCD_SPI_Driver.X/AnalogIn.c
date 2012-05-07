#include "AnalogIn.h"
#include <plib.h>

void setupAnalogIn(int port0, int port1, int port2, int port3, int port4, int port5, int port6, int port7, int port8, int port9, int port10, int port11, int port12, int port13, int port14, int port15)
{
    #define PARAM1  ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON
    #define PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_ON | ADC_SAMPLES_PER_INT_1 | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF
    #define PARAM3  ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15

    #define PARAM4  (port0) | (port1 << 1) | (port2 << 2) | (port3 << 3) | (port4 << 4) | (port5 << 5) | (port6 << 6) | (port7 << 7) | (port8 << 8) | (port9 << 9) | (port10 << 10) | (port11 << 11) | (port12 << 12) | (port13 << 13) | (port14 << 14) | (port15 << 15)
    #define PARAM5  ~(PARAM4)

    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF);
    OpenADC10( PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 );

    EnableADC10();

}

int readAnalogIn(int portIndex)
{
    return ReadADC10(portIndex);
}
