#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#define STATE_NORMAL 0
void state0();
#define STATE_INITIALIZATION 1
void state1();

void DelegateState(int state);
void ChangeState(int state);

void state1();

#endif