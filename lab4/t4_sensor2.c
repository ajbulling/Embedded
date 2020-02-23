#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "esos_sensor.h"
#include "../lib/src/esos_sensor.c"
#include "pic24_all.h"
#include "revF14.h"
#include "esos_f14ui.h"
#include "esos_f14ui.c"
#include "esos.h"
#include "esos_pic24_sensor.h"
#include "esos_pic24_sensor.c"

uint16_t pot_data;
char* numArray [5];
char* processMode [1];
char* numSamples [1];
bool continuousOutput = false;

// ESOS task for Heartbeat LED3
ESOS_USER_TASK ( LED3_blink ) {
    ESOS_TASK_BEGIN();
    while ( TRUE ){
        esos_uiF14_toggleLED3();
        ESOS_TASK_WAIT_TICKS(250);
    }
    ESOS_TASK_END();
}

ESOS_USER_TASK ( READ_ADC ) {
    ESOS_TASK_BEGIN();
    // Potentiometer is on channel 2
    ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH02, ESOS_SENSOR_VREF_1V0);
    ESOS_TASK_WAIT_SENSOR_QUICK_READ(pot_data);
    ESOS_SENSOR_CLOSE();

    // Output data from ADC
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_UINT16_AS_HEX_STRING(pot_data);
    ESOS_TASK_WAIT_ON_SEND_UINT8('\n');
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_END();
}

ESOS_USER_TASK ( QUICK_READ_TEST ) {
    ESOS_TASK_BEGIN();
    while ( TRUE ){
        esos_uiF14_checkHW();
        if (esos_uiF14_isSW1Pressed()) {
            continuousOutput = 0;
            esos_RegisterTask(READ_ADC);
        }
        if (esos_uiF14_isSW2Pressed()) {
            // Had to change this to make it tied to the hardware 
            // or it wouldn't get updated without a call to checkHW
            ESOS_TASK_WAIT_UNTIL_UIF14_SW2_RELEASED();
            continuousOutput = !continuousOutput;
        }
        if (continuousOutput) {
            esos_RegisterTask(READ_ADC);
            ESOS_TASK_WAIT_TICKS(1000);
        }
        if (esos_uiF14_isSW3Pressed()) {
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
            ESOS_TASK_WAIT_ON_SEND_STRING("Enter a processing mode. Press 1 for one-shot, ");
            ESOS_TASK_WAIT_ON_SEND_STRING("2 for average, 3 for minimum, 4 for maximum, or 5 for median: ");
            ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

            ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
            ESOS_TASK_WAIT_ON_GET_STRING(processMode);
            ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
            ESOS_TASK_WAIT_ON_SEND_STRING(processMode);
            ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
            ESOS_TASK_WAIT_ON_SEND_STRING("\nEnter the number of samples for the process. \n1) 2\n2) 4\n3) 8\n4) 16\n5) 32\n6) 64\n");
            ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

            ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
            ESOS_TASK_WAIT_ON_GET_STRING(numSamples);
            ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
            ESOS_TASK_WAIT_ON_SEND_STRING(numSamples);
            ESOS_TASK_WAIT_ON_SEND_STRING("\n");
            ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

        }
        ESOS_TASK_YIELD();        
    }
    ESOS_TASK_END();
}

void user_init() {
    CONFIG_POT();
    
    LED1_CONFIG();
    LED2_CONFIG();
    LED3_HB_CONFIG();

    SW1_CONFIG();
    SW2_CONFIG();
    SW3_CONFIG();

    esos_RegisterTask( LED3_blink );
    esos_RegisterTask( QUICK_READ_TEST );
}

