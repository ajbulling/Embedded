#include <stdint.h>
#include <stdbool.h>
#include "esos.h"
#include "esos_f14ui.h"
#include "esos_f14ui.c"
#include "revF14.h"
#include "pic24_all.h"

ESOS_USER_TASK ( CHECK_HW ){
    ESOS_TASK_BEGIN();
    while ( true ){
        esos_uiF14_checkHW();
        _esos_uiF14_calculateVelocity();
        ESOS_TASK_YIELD();
    }
    ESOS_TASK_END();
}

uint8_t num;
ESOS_USER_TASK ( SERIAL_READ ){
    ESOS_TASK_BEGIN();
    while (true) {
        ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
        ESOS_TASK_WAIT_ON_GET_UINT8(num);
        ESOS_TASK_WAIT_ON_SEND_STRING(num);
        ESOS_TASK_YIELD();
    }
    ESOS_TASK_END();
}

ESOS_USER_TASK ( SERIAL_PRINT ){
    ESOS_TASK_BEGIN();
    while( TRUE ){
        ESOS_TASK_WAIT_TICKS( __ESOS_UIF14_UI_PERIOD_MS );
        if ( esos_uiF14_isSW1Pressed() ){
            outString("SW1 Pressed\n");
        }

        if ( esos_uiF14_isSW2Pressed() ){
            outString("SW2 Pressed\n");
        }

        if ( esos_uiF14_isSW3Pressed() ){
            outString("SW3 Pressed\n");
        }

        if ( esos_uiF14_isSW1DoublePressed() ){
            outString("SW1 Double Pressed\n");
            //esos_uiF14_SW1DoublePressedExpired();
        }

        if ( esos_uiF14_isSW2DoublePressed() ){
            outString("SW2 Double Pressed\n");
        }

        if ( esos_uiF14_isSW3DoublePressed() ){
            outString("SW3 Double Pressed\n");
        }

        if ( esos_uiF14_isRpgTurning() ) {
            outString("RPG is turning\n");
            if ( esos_uiF14_isRpgTurningSlow() ){
                if ( RPG_TURN_CW() ) {
                    outString("RPG is turning CW slow\n");
                }

                if ( RPG_TURN_CCW() ) {
                    outString("RPG is turning CCW slow\n");
                }
            }

            if ( esos_uiF14_isRpgTurningMedium() ){
                if ( RPG_TURN_CW() ) {
                    outString("RPG is turning CW medium\n");
                }

                if ( RPG_TURN_CCW() ) {
                    outString("RPG is turning CCW medium\n");
                }
            }

            if ( esos_uiF14_isRpgTurningFast() ) {
                if ( RPG_TURN_CW() ) {
                    outString("RPG is turning CW fast\n");
                }

                if ( RPG_TURN_CCW() ) {
                    outString("RPG is turning CCW fast\n");
                }
            }
            esos_uiF14_resetVelocity();
        }
        ESOS_TASK_YIELD();
    }
    ESOS_TASK_END();
}

ESOS_USER_TASK( LED3_blink ){
    ESOS_TASK_BEGIN();
    while ( TRUE ){
        esos_uiF14_toggleLED3();
        ESOS_TASK_WAIT_TICKS(250);
    }
    ESOS_TASK_END();
}

ESOS_USER_TASK( LED2_state ){
    ESOS_TASK_BEGIN();
    while ( TRUE ){
        if ( esos_uiF14_getRpgVelocity_i16() == 0){
	        esos_uiF14_turnLED2Off();
	    }
        else if ( esos_uiF14_isRpgTurningSlow() ){
	        esos_uiF14_turnLED2On();
	    }
        else if ( esos_uiF14_isRpgTurningMedium() ){
	        while ( esos_uiF14_isRpgTurningMedium() ){
	            esos_uiF14_toggleLED2();
                ESOS_TASK_WAIT_TICKS(125);
            }
	    }
	    else if ( esos_uiF14_isRpgTurningFast() ){
	        while ( esos_uiF14_isRpgTurningFast() ){
                esos_uiF14_toggleLED2();
                ESOS_TASK_WAIT_TICKS(50);
	        }
        }
        ESOS_TASK_YIELD();
    }
    ESOS_TASK_END();
}

ESOS_USER_TASK( LED1_state){
    ESOS_TASK_BEGIN();
    while ( TRUE ){
        if ( esos_uiF14_isSW3Pressed() ){
            if ( esos_uiF14_isSW2Pressed() ){
                esos_uiF14_turnLED1On();
            }
            else {
                esos_uiF14_turnLED1Off();
            }
            if ( esos_uiF14_isSW2DoublePressed() ){
                esos_uiF14_flashLED1(500);
                //ESOS_TASK_WAIT_TICKS(50);
                esos_uiF14_flashLED1(500);
                //ESOS_TASK_WAIT_TICKS(50);
                esos_uiF14_flashLED1(500);
                //ESOS_TASK_WAIT_TICKS(50);
                esos_uiF14_SW2DoublePressedExpired();


            }
        }
        else {
            if ( esos_uiF14_isSW1Pressed() ){
                esos_uiF14_turnLED1On();
            }
            else {
                esos_uiF14_turnLED1Off();
            }
            if (esos_uiF14_isSW1DoublePressed() ){
                esos_uiF14_flashLED1(500);
                //ESOS_TASK_WAIT_TICKS(50);
                esos_uiF14_flashLED1(500);
                //ESOS_TASK_WAIT_TICKS(50);
                esos_uiF14_flashLED1(500);
                //ESOS_TASK_WAIT_TICKS(50);
                esos_uiF14_SW1DoublePressedExpired();
            }
        }
        ESOS_TASK_YIELD();
    }
    ESOS_TASK_END();
}

void user_init() {
/*
    // Configured in other .c file
    // Configure LED hardware
    LED1_CONFIG();
    LED2_CONFIG();
    LED3_HB_CONFIG();

    // Configure switch hardware
    SW1_CONFIG();
    SW2_CONFIG();
    SW3_CONFIG();
*/
    config_esos_uiF14();

    // Start ESOS tasks
    esos_RegisterTask( LED3_blink );
    esos_RegisterTask( LED2_state );
    esos_RegisterTask( LED1_state );
    esos_RegisterTask( CHECK_HW );
    //esos_RegisterTask( SERIAL_PRINT );
    esos_RegisterTask( SERIAL_READ );
}
