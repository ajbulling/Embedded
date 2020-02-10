#include <stdint.h>
#include <stdbool.h>
#include "esos.h"
#include "esos_f14ui.h"
#include "esos_f14ui.c"
#include "revF14.h"
#include "pic24_all.h"

ESOS_USER_TASK ( SERIAL_PRINT ){
    while( TRUE ){
        if ( esos_uiF14_getSW1Pressed()){
            outString("SW1 Pressed");
        }

        if ( esos_uiF14_getSw2Pressed()){
            outString("SW2 Pressed");
        }

        if ( esos_uiF14_getSw3Pressed()){
            outString("SW3 Pressed");
        }

        if ( esos_uiF14_getSW1DoublePressed()){
            outString("SW1 Double Pressed");
        }

        if ( esos_uiF14_getSW2DoublePressed()){
            outString("SW2 Double Pressed");
        }

        if ( esos_uiF14_getSW3DoublePressed()){
            outString("SW3 Double Pressed");
        }

        if ( esos_uiF14_isRpgTurning()) {
            outString("RPG is turning");
            if ( esos_uiF14_isRpgTurningSlow()){
                if ( esos_uiF14_isRpgTurningCW()) {
                    outString("RPG is turning CW slow");
                }

                if ( esos_uiF14_isRpgTurningCCW()) {
                    outString("RPG is turning CCW slow");
                }
            }

            if ( esos_uiF14_isRpgTurningMedium()){
                if ( esos_uiF14_isRpgTurningCW()) {
                    outString("RPG is turning CW medium");
                }

                if ( esos_uiF14_isRpgTurningCCW()) {
                    outString("RPG is turning CCW medium");
                }
            }

            if ( esos_uiF14_isRpgTurningFast()) {
                if ( esos_uiF14_isRpgTurningCW()) {
                    outString("RPG is turning CW fast");
                }

                if ( esos_uiF14_isRpgTurningCCW()) {
                    outString("RPG is turning CCW fast");
                }
            }
        }
    }
}

ESOS_USER_TASK( LED3_blink ){
    while ( TRUE ){
        esos_uiF14_flashLED3(500);
    }
}

ESOS_USER_TASK( LED2_state ){
    while ( TRUE ){
        if ( esos_uiF14_getRpgVelocity_i16() == 0){
	    esos_uiF14_turnLED2Off();
	}
        else if ( esos_uiF14_isRpgTurningSlow() ){
	    esos_uiF14_turnLED2On();
	}
        else if ( esos_uiF14_isRpgTurningMedium() ){
	    while ( esos_uiF14_isRpgTurningMedium() ){
	        esos_uiF14_flashLED2(500);
            }
	}
	else if ( esos_uiF14_isRpgTurningFast() ){
	    while ( esos_uiF14_isRpgTurningFast() ){
                esos_uiF14_flashLED2(100);
	    }
        }
    }
}

ESOS_USER_TASK( LED1_state){
    while ( TRUE ){
        if ( esos_uiF14_getSW3Pressed() ){
            if ( esos_uiF14_getSW2Pressed() ){
                esos_uiF14_turnLED1On();
            }
            else {
                esos_uiF14_turnLED1Off();
            }
            if ( esos_uiF14_getSW2DoublePressed() ){
                esos_uiF14_flashLED1(50);
                esos_uiF14_flashLED1(50);
                esos_uiF14_flashLED1(50);
            }
        }
        else {
            if ( esos_uiF14_getSW1Pressed() ){
                esos_uiF14_turnLED1On();
            }
            else {
                esos_uiF14_turnLED1Off();
            }
            if (esos_uiF14_getSW1DoublePressed() ){
                esos_uiF14_flashLED1(50);
                esos_uiF14_flashLED1(50);
                esos_uiF14_flashLED1(50);
            }
        }
    }
}

void user_init() {
    // Configgure LED hardware
    LED1_CONFIG();
    LED2_CONFIG();
    LED3_HB_CONFIG();

    // Configure switch hardware
    SW1_CONFIG();
    SW2_CONFIG();
    SW3_CONFIG();

    // Start ESOS tasks
    esos_RegisterTask( LED3_blink );
    esos_RegisterTask( LED2_state );
    esos_RegisterTask( LED1_state );
}
