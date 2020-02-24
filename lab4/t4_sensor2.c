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
char* processMode [1];
char* numSamples [1];
uint8_t input1; // Process mode (integer)
uint8_t input2; // Number of samples (integer)
uint8_t pmode = 0; // Actual process mode argument (must be calculated)
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
    ESOS_TASK_WAIT_SENSOR_READ(pot_data, pmode, ESOS_SENSOR_FORMAT_VOLTAGE);
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
        // Update switch states
        esos_uiF14_checkHW();
        if (esos_uiF14_isSW1Pressed()) {
            continuousOutput = 0; // Stop printing continuously if sw1 pressed
            esos_RegisterTask(READ_ADC);
        }
        if (esos_uiF14_isSW2Pressed()) {
            ESOS_TASK_WAIT_UNTIL_UIF14_SW2_RELEASED();
            continuousOutput = !continuousOutput; // Print continuously if sw2 released
        }
        if (continuousOutput) {
            esos_RegisterTask(READ_ADC);
            ESOS_TASK_WAIT_TICKS(1000); // Prints potentiometer voltage every second
        }
        if (esos_uiF14_isSW3Pressed()) { // Display interface options for sw3 press
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
            ESOS_TASK_WAIT_ON_SEND_STRING("Enter a processing mode. Press 1 for one-shot, ");
            ESOS_TASK_WAIT_ON_SEND_STRING("2 for average, 3 for minimum, 4 for maximum, or 5 for median: ");
            ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

            // Get processing mode
            ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
            ESOS_TASK_WAIT_ON_GET_STRING(processMode);
            ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

            // Echo input
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
            ESOS_TASK_WAIT_ON_SEND_STRING(processMode);
            ESOS_TASK_WAIT_ON_SEND_STRING("\n");
            ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

            input1 = atoi(processMode); // Convert char array to uint8_t
            // Don't ask for samples if one-shot mode is selected
            if (input1 != 1) {
                ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
                ESOS_TASK_WAIT_ON_SEND_STRING("Enter the number of samples for the process. \n1) 2\n2) 4\n3) 8\n4) 16\n5) 32\n6) 64\n");
                ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

                // Get number of samples
                ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
                ESOS_TASK_WAIT_ON_GET_STRING(numSamples);
                ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

                // Echo input
                ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
                ESOS_TASK_WAIT_ON_SEND_STRING(numSamples);
                ESOS_TASK_WAIT_ON_SEND_STRING("\n");
                ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

                input2 = atoi(numSamples); // Convert char array to uint8_t
                // Don't do this for average mode (special case: bit shifting
                // does not work)
                if (input1 != 2) {
                    // Ignore one for one-shot, start at zero so subtract 2
                    // Then add 4 to move over to the upper 4 bits
                    // -2 + 4 = 2, so add 2 to input 1 to get bit shift amount
                    input1 += 2;
                    pmode = 1 << input1; // bit-shift over to select processing mode
                    pmode |= input2; // bit-wise OR to select number of samples (bottom 4 bits)
                }
                else {
                    pmode = input2; // Process mode is 0x01-0x06 for average mode
                }
            }
            else {
                pmode = 0; // Process mode is 0 for one-shot
            }
        }
        ESOS_TASK_YIELD();
    }
    ESOS_TASK_END();
}

void user_init() {
    // Config HW
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

