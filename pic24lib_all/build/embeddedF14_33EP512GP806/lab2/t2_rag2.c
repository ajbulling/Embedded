/*
Embedded Systems Lab 2 Task 2.2.3
Traffic signal controller specification #2

By:
Jesse Tutor, Zach Fauver, Andrew Bullington, Will Gaines

*/

#include "esos.h"
#include "revF14.h"
#include "esos_pic24.h"

// Define FSM
// RG = 0, RA = 1, GR = 2, AR = 3, RR1 = 4, RR2 = 5
static int current_state = 0;


// ESOS task to control which LEDs are displayed on the hardware
ESOS_USER_TASK( button_press ){
    ESOS_TASK_BEGIN();
        while( TRUE ){

                // Red - Green
                if( current_state == 0 ){
                    if( SW3_PRESSED ){
                        // Display E-W Signals
                        LED1_OFF();
                        LED2_OFF();
                        LED3_HB_ON();
                    }
                    else {
                        // Display N-S Signals
                        LED1_ON();
                        LED2_OFF();
                        LED3_HB_OFF();
                    }
                }

                // Red - Amber
                if( current_state == 1 ){
                    if( SW3_PRESSED ){
                        // Display E-W Signals
                        LED1_OFF();
                        LED2_ON();
                        LED3_HB_OFF();
                    }
                    else {
                        // Display N-S Signals
                        LED1_ON();
                        LED2_OFF();
                        LED3_HB_OFF();
                    }
                }

                // Green - Red
                if( current_state == 2 ) {
                    if( SW3_PRESSED ){
                        // Display E-W Signals
                        LED1_ON();
                        LED2_OFF();
                        LED3_HB_OFF();
                    }
                    else {
                        // Display N-S Signals
                        LED1_OFF();
                        LED2_OFF();
                        LED3_HB_ON();
                    }
                }

                // Amber - Red
                if( current_state == 3 ){
                    if( SW3_PRESSED ){
                        // Display E-W Signals
                        LED1_ON();
                        LED2_OFF();
                        LED3_HB_OFF();
                    }
                    else {
                        // Display N-S Signals
                        LED1_OFF();
                        LED2_ON();
                        LED3_HB_OFF();
                    }
                }
                
                // Double red split into two states, allowing the FSM to maintain current postion.
                // Red - Red - Case following RA.
                if( current_state == 4 ){
                    LED1_ON();
                    LED2_OFF();
                    LED3_HB_OFF();
                }
                
                // Red - Red - Case following AR.
                if( current_state == 5 ){
                    LED1_ON();
                    LED2_OFF();
                    LED3_HB_OFF();
                }
	    ESOS_TASK_YIELD();
            }
    ESOS_TASK_END();
}

// ESOS task used to update the current state of the FSM.
// Contains wait timers and FSM decision logic
ESOS_USER_TASK( state_set ){
    ESOS_TASK_BEGIN();
        while( TRUE ){

                // Red - Green
                if( current_state == 0 ){
                    if( SW1_PRESSED ) {
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(30000);   
                    }
                    else{
                        ESOS_TASK_WAIT_TICKS(10000);
                    }
                    current_state = 1;
                }
                
                // Red - Amber
                if( current_state == 1 ){
                    if( SW1_PRESSED ) {
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(3000);
                        current_state = 4;
                    }
                    else{
                        ESOS_TASK_WAIT_TICKS(3000);
                        current_state = 2;
                    }
                }
                
                // Green - Red
                if( current_state == 2 ){
                    if( SW1_PRESSED ) {
                        //Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(30000);
                    }
                    else{
                        ESOS_TASK_WAIT_TICKS(10000);
                    }
                    current_state = 3;
                }
                
                // Amber - Red
                if( current_state == 3 ){
                    if ( SW1_PRESSED ){
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(3000);
                        current_state = 5;
                    }
                    else{
                        ESOS_TASK_WAIT_TICKS(3000);
                        current_state = 0;
                    }
                }

                // Double red split into two states, allowing the FSM to maintain current postion.
                // Red - Red - Case following RA
                if( current_state == 4 ){
                    if( SW1_PRESSED ){
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(1000);
                    }
                    current_state = 2;
                }

                // Red - Red - Case following AR
                if( current_state == 5 ){
                    if( SW1_PRESSED ){
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(1000);
                    }
                    current_state = 0;
                }
            }
    ESOS_TASK_END();
}

void user_init(){
    //Configure LED hardware
    LED1_CONFIG(); // Upper/Red LED
    LED2_CONFIG(); // Middle/Amber LED
    LED3_HB_CONFIG(); // Lower/Green LED

    //Configure switch hardware
    SW1_CONFIG();
    SW2_CONFIG();
    SW3_CONFIG();

    // Start ESOS tasks
    esos_RegisterTask( button_press );
    esos_RegisterTask( state_set );
}
