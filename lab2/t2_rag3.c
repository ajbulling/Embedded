/*
Embedded Systems Lab 2 Task 2.2.4
Traffic signal controller specification #3

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
    RT,
    TR,
} state_t

const *char state_names[] = {
    "RG",
    "RA",
    "GR",
    "AR",
    "RR1",
    "RR2",
    "RT",
    "TR",
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

                // Red - Turn
                case RT:
                    if( SW3_PRESSED ){
                        // Display E-W Signals
                        LED1_OFF();
                        LED2_OFF();
                        // Blink LED3
                        // Handled by led_flash ESOS_USER_TASK
                    }
                    else{
                        // Display N-S Signals
                        LED1_ON();
                        LED2_OFF();
                        LED3_HB_OFF()
                    }
                    break;
                
                // Turn - Red
                case TR:
                    if( SW3_PRESSED ){
                        // Display E-W Signals
                        LED1_ON()
                        LED2_OFF();
                        LED3_HB_OFF();
                    }
                    else{
                        // Display N-S Signals
                        LED1_OFF();
                        LED2_OFF();
                        // Blink LED3
                        // Handled by led_flash ESOS_USER_TASK
                    }
                    break;

                default:
                    break; 
            }    
        }
    ESOS_TASK_END():
}

// ESOS task used to control the blinking of LED3 during turn cases of FSM
ESOS_USER_TASK( led_flash ){
    ESOS_TASK_BEGIN();
        while( TRUE ){
            if( ( SW3_PRESSED ) && ( current_state == RT ) ){
                //Flash LED3 for 0.25s ON/OFF
                LED3_HB_ON();
                ESOS_TASK_WAIT_TICKS(250);
                LED3_HB_OFF();
                ESOS_TASK_WAIT_TICKS(250);
            }
            if( ( SW3_RELEASED ) && ( current_state == TR ) ){
                //Flash LED3 for 0.25s ON/OFF
                LED3_HB_ON();
                ESOS_TASK_WAIT_TICKS(250);
                LED3_HB_OFF();
                ESOS_TASK_WAIT_TICKS(250);
            }
        }
    ESOS_TASK_END();
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
                        // Determine if turn functionality is required
                        if( SW2_PRESSED ){
                            current_state = TR;
                        }
                        else{
                            current_state = GR;
                        }
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
                        //Determine if turn functionality is required
                        if( SW2_PRESSED ){
                            current_state = RT;
                        }
                        else{
                            current_state = RG;
                        }
                    }
                    break;
                
                // Double red split into two states, allowing the FSM to maintain current postion.
                // Red - Red - Case following RA
                case RR1:
                    if( SW1_PRESSED ){
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(1000);
                    }
                    if( SW2_PRESSED ){
                        current_state = TR;
                    }
                    else{
                        current_state = GR;
                    }
                    break;

                // Red - Red - Case following AR
                case RR2:
                    if( SW1_PRESSED ){
                        // Rush Hour Conditions
                        ESOS_TASK_WAIT_TICKS(1000);
                    }
                    if( SW2_PRESSED ){
                        current_state = RT;
                    }
                    else{
                        current_state = RG;
                    }
                    break;

                // Red - Turn
                case RT:
                    // Rush Hour and standard signaling use same wait times
                    ESOS_TASK_WAIT_TICKS(10000);
                    current_state = RG;
                    break;

                // Turn - Red
                case TR:
                    // Rush Hour and standard signaling use same wait times
                    ESOS_TASK_WAIT_TICKS(10000);
                    current_state = GR;
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
    esos_register_task( led_flash );
}
