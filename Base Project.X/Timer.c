#include "Initialize.h"
#include <plib.h>

int setupTimer(int timer, int frequency, int priority)
{
    int tX_on;
    int tX_source_int;
    int tX_int_on;
    int tX_ps_1_X;

    long countTo = SYS_FREQ / frequency;

    switch (timer)
    {
        case 1:
            tX_on = T1_ON;
            tX_source_int = T1_SOURCE_INT;
            tX_int_on = T1_INT_ON;

            while (1)
            {
                if (countTo >= 65536)
                {
                    countTo = countTo / 8;
                    if (countTo >= 65536)
                    {
                        countTo = countTo / 8;
                        if (countTo >= 65536)
                        {
                            countTo = countTo / 8;
                            if (countTo >= 65536)
                            {
                                return 0;
                            }
                            tX_ps_1_X = T1_PS_1_256;
                            break;
                        }
                        tX_ps_1_X = T1_PS_1_64;
                        break;
                    }
                    tX_ps_1_X = T1_PS_1_8;
                    break;
                }
                else
                {
                    tX_ps_1_X = T1_PS_1_1;
                    break;
                }
            }
            OpenTimer1(tX_on | tX_source_int | tX_ps_1_X, countTo);
            ConfigIntTimer1(tX_int_on | priority);
            break;

        case 2:
            tX_on = T2_ON;
            tX_source_int = T2_SOURCE_INT;
            tX_int_on = T2_INT_ON;

            while (1)
            {
                if (countTo >= 65536)
                {
                    countTo = countTo / 2;
                    if (countTo >= 65536)
                    {
                        countTo = countTo / 2;
                        if (countTo >= 65536)
                        {
                            countTo = countTo / 2;
                            if (countTo >= 65536)
                            {
                                countTo = countTo / 2;
                                if (countTo >= 65536)
                                {
                                    countTo = countTo / 2;
                                    if (countTo >= 65536)
                                    {
                                        countTo = countTo / 2;
                                        if (countTo >= 65536)
                                        {
                                            countTo = countTo / 2;
                                            if (countTo >= 65536)
                                            {
                                                return 0;
                                            }
                                            tX_ps_1_X = T2_PS_1_256;
                                            break;
                                        }
                                        tX_ps_1_X = T2_PS_1_64;
                                        break;
                                    }
                                    tX_ps_1_X = T2_PS_1_32;
                                    break;
                                }
                                tX_ps_1_X = T2_PS_1_16;
                                break;
                            }
                            tX_ps_1_X = T2_PS_1_8;
                            break;
                        }
                        tX_ps_1_X = T2_PS_1_4;
                        break;
                    }
                    tX_ps_1_X = T2_PS_1_2;
                    break;
                }
                else
                {
                    tX_ps_1_X = T2_PS_1_1;
                    break;
                }
            }
            OpenTimer2(tX_on | tX_source_int | tX_ps_1_X, countTo);
            ConfigIntTimer2(tX_int_on | priority);
            break;

        case 3:
            tX_on = T3_ON;
            tX_source_int = T3_SOURCE_INT;
            tX_int_on = T3_INT_ON;

            while (1)
            {
                if (countTo >= 65536)
                {
                    countTo = countTo / 2;
                    if (countTo >= 65536)
                    {
                        countTo = countTo / 2;
                        if (countTo >= 65536)
                        {
                            countTo = countTo / 2;
                            if (countTo >= 65536)
                            {
                                countTo = countTo / 2;
                                if (countTo >= 65536)
                                {
                                    countTo = countTo / 2;
                                    if (countTo >= 65536)
                                    {
                                        countTo = countTo / 2;
                                        if (countTo >= 65536)
                                        {
                                            countTo = countTo / 2;
                                            if (countTo >= 65536)
                                            {
                                                return 0;
                                            }
                                            tX_ps_1_X = T3_PS_1_256;
                                            break;
                                        }
                                        tX_ps_1_X = T3_PS_1_64;
                                        break;
                                    }
                                    tX_ps_1_X = T3_PS_1_32;
                                    break;
                                }
                                tX_ps_1_X = T3_PS_1_16;
                                break;
                            }
                            tX_ps_1_X = T3_PS_1_8;
                            break;
                        }
                        tX_ps_1_X = T3_PS_1_4;
                        break;
                    }
                    tX_ps_1_X = T3_PS_1_2;
                    break;
                }
                else
                {
                    tX_ps_1_X = T3_PS_1_1;
                    break;
                }
            }
            OpenTimer3(tX_on | tX_source_int | tX_ps_1_X, countTo);
            ConfigIntTimer3(tX_int_on | priority);
            break;

        case 4:
            tX_on = T4_ON;
            tX_source_int = T4_SOURCE_INT;
            tX_int_on = T4_INT_ON;

            while (1)
            {
                if (countTo >= 65536)
                {
                    countTo = countTo / 2;
                    if (countTo >= 65536)
                    {
                        countTo = countTo / 2;
                        if (countTo >= 65536)
                        {
                            countTo = countTo / 2;
                            if (countTo >= 65536)
                            {
                                countTo = countTo / 2;
                                if (countTo >= 65536)
                                {
                                    countTo = countTo / 2;
                                    if (countTo >= 65536)
                                    {
                                        countTo = countTo / 2;
                                        if (countTo >= 65536)
                                        {
                                            countTo = countTo / 2;
                                            if (countTo >= 65536)
                                            {
                                                return 0;
                                            }
                                            tX_ps_1_X = T4_PS_1_256;
                                            break;
                                        }
                                        tX_ps_1_X = T4_PS_1_64;
                                        break;
                                    }
                                    tX_ps_1_X = T4_PS_1_32;
                                    break;
                                }
                                tX_ps_1_X = T4_PS_1_16;
                                break;
                            }
                            tX_ps_1_X = T4_PS_1_8;
                            break;
                        }
                        tX_ps_1_X = T4_PS_1_4;
                        break;
                    }
                    tX_ps_1_X = T4_PS_1_2;
                    break;
                }
                else
                {
                    tX_ps_1_X = T4_PS_1_1;
                    break;
                }
            }
            OpenTimer4(tX_on | tX_source_int | tX_ps_1_X, countTo);
            ConfigIntTimer4(tX_int_on | priority);
            break;
    }
    return 1;
}