#ifndef TIMER_H
#define TIMER_H

int setupTimer(int timer, int frequency, int priority);
void closeTimer(int timer);

//ISR TEMPLATE
/*
void __ISR(_TIMER_1_VECTOR, ipl1) TimerIsr(void)
{
    mT1ClearIntFlag();
}
 */

#endif