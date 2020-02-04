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

ESOS_USER_TASK( light_display ){
    ESOS_TASK_BEGIN();
    static state_t next_state = RG;
        while( TRUE ){
            switch (next_state){
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
                    next_state = RA;
                    ESOS_TASK_WAIT_TICKS(10000);
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
                    next_state = GR;
                    ESOS_TASK_WAIT_TICKS(3000);
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
                    next_state = AR;
                    ESOS_TASK_WAIT_TICKS(10000);
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
                    next_state = RG;
                    ESOS_TASK_WAIT_TICKS(3000);
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
    esos_register_task( light_display );
}

