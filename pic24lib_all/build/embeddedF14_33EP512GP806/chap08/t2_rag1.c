/*
Embedded Systems Lab 2 Task 2.2.2
Traffic signal controller specification #1

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
} state_t;
/*
const *char state_names[] = {
    "RG",
    "RA",
    "GR",
    "AR",
};
*/
static state_t current_state = RG;

// ESOS task to control which LEDs are displayed on the hardware
ESOS_USER_TASK( button_press ){
    ESOS_TASK_BEGIN();
        while( TRUE ){
            switch (current_state){
                
                // Red - Green
                case RG:
                    if(SW3_PRESSED ){
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

                default:
                    break; 
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
                    ESOS_TASK_WAIT_TICKS(10000);
                    current_state = RA;
                    break;
                
                // Red - Amber
                case RA:
                    ESOS_TASK_WAIT_TICKS(3000);
                    current_state = GR;
                    break;
                
                // Green - Red
                case GR:
                    ESOS_TASK_WAIT_TICKS(10000);
                    current_state = AR;
                    break;
                
                // Amber - Red
                case AR:
                    ESOS_TASK_WAIT_TICKS(3000);
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