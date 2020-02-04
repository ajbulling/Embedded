/*
Embedded Systems Lab 2 Task 2.2.2
Traffic signal controller specification #1
*/

#include "esos.h"
#include "revF14.h"
#include "esos_pic24.h"

typedef enum{
    RG,
    RA,
    GR,
    AR,
} state_t

const *char state_names[] = {
    "RG",
    "RA",
    "GR",
    "AR",
};

ESOS_USER_TASK( button_press ){
    ESOS_TASK_BEGIN();
        while( TRUE ){
            switch (current_state){

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
    ESOS_TASK_END():
}


static state_t current_state = RG;

ESOS_USER_TASK( state_set ){
    ESOS_TASK_BEGIN();
        while( TRUE ){
            switch (current_state){

                case RG:
                    ESOS_TASK_WAIT_TICKS(10000);
                    current_state = RA;
                    break;
                
                case RA:
                    ESOS_TASK_WAIT_TICKS(3000);
                    current_state = GR;
                    break;
                
                case GR:
                    ESOS_TASK_WAIT_TICKS(10000);
                    current_state = AR;
                    break;
                
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

    esos_register_task( button_press );
    esos_register_task( state_set );
}