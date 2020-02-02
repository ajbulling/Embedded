/*
Embedded Systems Lab 2 Task 2.2.2
Traffic signal controller specification #1
*/

#include "esos.h"
#include "revF14.h"

void user_init(void) {
    __esos_unsafe_PutString(HELLO_MSG);
    CONFIG_LED1();
    esos_RegisterTask(echo);
    esos_RegisterTimer(swTimerLED,250);
}

ESOS_USER_TIMER(swTimerLED) {
    LED1 = !LED1;
}

