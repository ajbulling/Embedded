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
typedef enum{
    RG,
    RA,
    GR,
    AR,
    RR1,
    RR2,
} state_t

const *char state_names[] = {
    "RG",
    "RA",
    "GR",
    "AR",
    "RR1",
    "RR2",
};

static state_t current_state = RG;

// ESOS task to control which LEDs are displayed on the hardware
ESOS_USER_TASK( button_press ){
    ESOS_TASK_BEGIN();
        while( TRUE ){
            switch (current_state){
                
                // Red - Green
                case RG:
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
                    break;

                // Red - Amber
                case RA:
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
                    break;

                // Green - Red
                case GR:
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
                    break;

                // Amber - Red
                case AR:
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
                    break;
                
                // Double red split into two states, allowing the FSM to maintain current postion.
                // Red - Red - Case following RA.
                case RR1:
                    LED1_ON();
                    LED2_OFF();
                    LED3_HB_OFF();
                    break;
                
                // Red - Red - Case following AR.
                case RR2:
                    LED1_ON();
                    LED2_OFF();
                    LED3_HB_OFF();
                    break;

                default:
                    break; 
            }    
        }
    ESOS_TASK_END():
}

// ESOS task used to update the current state of the FSM.
// Contains wait timers and FSM decision logic
ESOS_USER_TASK( state_set ){
    ESOS_TASK_BEGIN();
        while( TRUE ){
            switch (current_state){

                // Red - Green
                case RG:
                    if( SW1_PRESSED ) {
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(30000);   
                    }
                    else{
                        ESOS_TASK_WAIT_TICKS(10000);
                    }
                    current_state = RA;
                    break;
                
                // Red - Amber
                case RA:
                    if( SW1_PRESSED ) {
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(3000);
                        current_state = RR1;
                    }
                    else{
                        ESOS_TASK_WAIT_TICKS(3000);
                        current_state = GR;
                    }
                    break;
                
                // Green - Red
                case GR:
                    if( SW1_PRESSED ) {
                        //Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(30000);
                    }
                    else{
                        ESOS_TASK_WAIT_TICKS(10000);
                    }
                    current_state = AR;
                    break;
                
                // Amber - Red
                case AR:
                    if ( SW1_PRESSED ){
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(3000);
                        current_state = RR2;
                    }
                    else{
                        ESOS_TASK_WAIT_TICKS(3000);
                        current_state = RG;
                    }
                    break;

                // Double red split into two states, allowing the FSM to maintain current postion.
                // Red - Red - Case following RA
                case RR1:
                    if( SW1_PRESSED ){
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(1000);
                    }
                    current_state = GR;
                    break;

                // Red - Red - Case following AR
                case RR2:
                    if( SW1_PRESSED ){
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(1000);
                    }
                    current_state = RG;
                    break;

                default:
                    break;  
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
    esos_register_task( button_press );
    esos_register_task( state_set );
}