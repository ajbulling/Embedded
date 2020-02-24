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

ESOS_USER_TASK ( POT_INTERFACE ) {
    ESOS_TASK_BEGIN();
    while (true) {
        esos_uiF14_checkHW(); // Update switch states
        if (esos_uiF14_isSW1Pressed()) {
            continuousOutput = 0; // Stop printing continuously if sw1 pressed
            esos_RegisterTask(READ_ADC);
        }
        if (esos_uiF14_isSW2Pressed()) {
            ESOS_TASK_WAIT_UNTIL_UIF14_SW2_RELEASED();
            continuousOutput = !continuousOutput; // Print continuously after sw2 released
        }
        if (continuousOutput) {
            esos_RegisterTask(READ_ADC);
            ESOS_TASK_WAIT_TICKS(1000); // Prints potentiometer voltage every second
        }
        ESOS_TASK_YIELD();
    }
    ESOS_TASK_END();
}

void user_init() {
    // Configure HW
    CONFIG_POT();
    
    LED1_CONFIG();
    LED2_CONFIG();
    LED3_HB_CONFIG();

    SW1_CONFIG();
    SW2_CONFIG();
    SW3_CONFIG();

    // Register ESOS tasks
    esos_RegisterTask( LED3_blink );
    esos_RegisterTask( POT_INTERFACE );
}

