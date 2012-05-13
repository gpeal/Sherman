#include "StateMachine.h"

void delegateState(int state)
{
    switch(state)
    {
        case 1:
            state1();
            break;
    }
}

void state1();