#include <stdint.h>
#include <stdbool.h>
#include "esos.h"
#include "esos_f14ui.h"
#include "esos_f14ui.c"
#include "revF14.h"
#include "pic24_all.h"

uint8_t num;
uint16_t doublePressPeriod;

char* numArray [5];

ESOS_USER_TASK ( CHECK_HW ){
    ESOS_TASK_BEGIN();
    while ( true ){
        esos_uiF14_checkHW();
        //ESOS_TASK_WAIT_TICKS(10);
        _esos_uiF14_calculateVelocity();
        ESOS_TASK_YIELD();
    }
    ESOS_TASK_END();
}

ESOS_USER_TASK ( SERIAL_PRINT ){
    ESOS_TASK_BEGIN();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Enter period for double press flash (1-10): ");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_STRING(numArray);
    ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING(numArray);
    ESOS_TASK_WAIT_ON_SEND_STRING("\n");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    // Subtract stupid ascii number and scale up
    doublePressPeriod = atoi(numArray);
    doublePressPeriod *= 50;

    // Input for slow RPG behavior
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Enter RPG slow threshold (1-10): ");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_STRING(numArray);
    ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING(numArray);
    ESOS_TASK_WAIT_ON_SEND_STRING("\n");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    _esos_uiF14_setSlowThreshold(atoi(numArray) * 1000);

    // Input for medium RPG behavior
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Enter RPG medium threshold (1-10): ");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_STRING(numArray);
    ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING(numArray);
    ESOS_TASK_WAIT_ON_SEND_STRING("\n");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    _esos_uiF14_setMediumThreshold(atoi(numArray) * 1000);

    // Input for fast RPG behavior
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Enter RPG fast threshold (1-10): ");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_STRING(numArray);
    ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING(numArray);
    ESOS_TASK_WAIT_ON_SEND_STRING("\n");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    _esos_uiF14_setFastThreshold(atoi(numArray) * 500);

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
                esos_uiF14_flashLED1(doublePressPeriod);
                //ESOS_TASK_WAIT_TICKS(50);
                esos_uiF14_flashLED1(doublePressPeriod);
                //ESOS_TASK_WAIT_TICKS(50);
                esos_uiF14_flashLED1(doublePressPeriod);
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
                esos_uiF14_flashLED1(doublePressPeriod);
                //ESOS_TASK_WAIT_TICKS(50);
                esos_uiF14_flashLED1(doublePressPeriod);
                //ESOS_TASK_WAIT_TICKS(50);
                esos_uiF14_flashLED1(doublePressPeriod);
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
    esos_RegisterTask( SERIAL_PRINT );
}
