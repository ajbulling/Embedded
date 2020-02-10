#include <stdint.h>
#include <stdbool.h>
#include "esos.h"
#include "esos_f14ui.h"
#include "revF14.h"
#include "pic24_all.h"

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
            if ( esos_uiF14getSW2Pressed() ){
                esos_uiF14_turnLED1On();
            }
            else {
                esos_uiF14_turnLED1Off();
            }
            if ( esos_uiF14_SW2DoublePressed() ){
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
            if (esos_uiF14_SW1DoublePressed() ){
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
}
