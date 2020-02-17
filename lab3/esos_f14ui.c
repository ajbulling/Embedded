/*******************************************************************
 * 
 * C code framework for ESOS user-interface (UI) service
 * 
 *    requires the EMBEDDED SYSTEMS target rev. F14
 * 
 * ****************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "esos.h"
#include "esos_f14ui.h"
#include "revF14.h"
#include "pic24_all.h"

int dptime = 100;
timer_on = 0;

// PRIVATE FUNCTIONS
// Update RPG counter
inline void _esos_uiF14_setRPGCounter (uint16_t newValue) {
    _st_esos_uiF14Data.u16_lastRPGCounter = _st_esos_uiF14Data.u16_RPGCounter;
    _st_esos_uiF14Data.u16_RPGCounter = newValue;
    return;
}

inline void _esos_uiF14_setLastRPGCounter (uint16_t newValue) {
    _st_esos_uiF14Data.u16_lastRPGCounter = newValue;
    return;
}

// PUBLIC SWITCH FUNCTIONS

// Check hardware
inline bool esos_uiF14_checkHW (void) {
    if (SW1_PRESSED) {
        //if the double pressed timer is not running, set single pressed
        if (timer_on != 1) {
            DOUBLE_PRESSED_TIMER();
            _st_esos_uiF14Data.b_SW1Pressed = true;
            _st_esos_uiF14Data.b_SW1DoublePressed = false;
        }
        //if double pressed timer is running, set double pressed true
        else {
            _st_esos_uiF14Data.b_SW1DoublePressed = true;
            _st_esos_uiF14Data.b_SW1Pressed = false;
        }

    } 
    if (SW1_RELEASED) _st_esos_uiF14Data.b_SW1Pressed = false;
    //if (SW1_DOUBLEPRESSED) _st_esos_uiF14Data.b_SW1DoublePressed = true;

    if (SW2_PRESSED) {
        //if the double pressed timer is not running, set single pressed
        if (timer_on != 1) {
            DOUBLE_PRESSED_TIMER();
            _st_esos_uiF14Data.b_SW2Pressed = true;
        }
        //if double pressed timer is running, set double pressed true
        else {
            _st_esos_uiF14Data.b_SW2DoublePressed = true;
            _st_esos_uiF14Data.b_SW2Pressed = false;
        }

    }

    if (SW2_RELEASED) _st_esos_uiF14Data.b_SW2Pressed = false;

    if (SW3_PRESSED) _st_esos_uiF14Data.b_SW3Pressed = true;
    if (SW3_RELEASED) _st_esos_uiF14Data.b_SW3Pressed = false;

    if (RPG_TURN_CW()) _esos_uiF14_setRPGCounter(_st_esos_uiF14Data.u16_RPGCounter + 1);
    if (RPG_TURN_CCW()) _esos_uiF14_setRPGCounter(_st_esos_uiF14Data.u16_RPGCounter - 1);
}

//SW1
inline bool esos_uiF14_isSW1Pressed (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==TRUE);
}

inline bool esos_uiF14_isSW1Released (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==FALSE);
}

inline bool esos_uiF14_isSW1DoublePressed (void) {
    return (_st_esos_uiF14Data.b_SW1DoublePressed==TRUE);
}

//SW2
inline bool esos_uiF14_isSW2Pressed (void) {
    return (_st_esos_uiF14Data.b_SW2Pressed==TRUE);
}

inline bool esos_uiF14_isSW2Released (void) {
    return (_st_esos_uiF14Data.b_SW2Pressed==FALSE);
}

inline bool esos_uiF14_isSW2DoublePressed (void) {
    return (_st_esos_uiF14Data.b_SW2DoublePressed==TRUE);
}

//SW3
inline bool esos_uiF14_isSW3Pressed (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==TRUE);
}

inline bool esos_uiF14_isSW3Released (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==FALSE);
}

inline bool esos_uiF14_isSW3DoublePressed (void) {
    return (_st_esos_uiF14Data.b_SW1DoublePressed==TRUE);
}

inline void esos_uiF14_SW1DoublePressedExpired (void) {
    _st_esos_uiF14Data.b_SW1DoublePressed = false;
    return;
}

inline void esos_uiF14_SW2DoublePressedExpired (void) {
    _st_esos_uiF14Data.b_SW1DoublePressed = false;
    return;
}

// PUBLIC LED FUNCTIONS

//LED1
inline bool esos_uiF14_isLED1On (void) {
    return (_st_esos_uiF14Data.b_LED1On==TRUE);
}

inline bool esos_uiF14_isLED1Off (void) {
    return (_st_esos_uiF14Data.b_LED1On==FALSE);
}

inline void esos_uiF14_turnLED1On (void) {
    _st_esos_uiF14Data.b_LED1On = TRUE;
    LED1_ON();
    return;
}

inline void esos_uiF14_turnLED1Off (void) {
    _st_esos_uiF14Data.b_LED1On = FALSE;
    LED1_OFF();
    return;
}

inline void esos_uiF14_toggleLED1 (void) {
    _st_esos_uiF14Data.b_LED1On ^= 1;
    LED1_HB_TOGGLE();
    return;
}

inline void esos_uiF14_flashLED1( uint16_t u16_period) {
    _st_esos_uiF14Data.u16_LED1FlashPeriod = u16_period;
    esos_uiF14_turnLED1On();
    DELAY_MS(u16_period / 2);
    esos_uiF14_turnLED1Off();
    DELAY_MS(u16_period / 2);
    return;
}


//LED2
inline bool esos_uiF14_isLED2On (void) {
    return (_st_esos_uiF14Data.b_LED2On==TRUE);
}

inline bool esos_uiF14_isLED2Off (void) {
    return (_st_esos_uiF14Data.b_LED2On==FALSE);
}

inline void esos_uiF14_turnLED2On (void) {
    _st_esos_uiF14Data.b_LED2On = TRUE;
    LED2_ON();
    return;
}

inline void esos_uiF14_turnLED2Off (void) {
    _st_esos_uiF14Data.b_LED2On = FALSE;
    LED2_OFF();
    return;
}

inline void esos_uiF14_toggleLED2 (void) {
    _st_esos_uiF14Data.b_LED2On ^= 1;
    LED2_HB_TOGGLE();
    return;
}

inline void esos_uiF14_flashLED2( uint16_t u16_period) {
    _st_esos_uiF14Data.u16_LED2FlashPeriod = u16_period;
    esos_uiF14_turnLED2On();
    DELAY_MS(u16_period / 2);
    esos_uiF14_turnLED2Off();
    DELAY_MS(u16_period / 2);
    return;
}

//LED3
inline bool esos_uiF14_isLED3On (void) {
    return (_st_esos_uiF14Data.b_LED3On==TRUE);
}

inline bool esos_uiF14_isLED3Off (void) {
    return (_st_esos_uiF14Data.b_LED3On==FALSE);
}

inline void esos_uiF14_turnLED3On (void) {
    _st_esos_uiF14Data.b_LED3On = TRUE;
    LED3_HB_ON();
    return;
}

inline void esos_uiF14_turnLED3Off (void) {
    _st_esos_uiF14Data.b_LED3On = FALSE;
    LED3_HB_OFF();
    return;
}

inline void esos_uiF14_toggleLED3 (void) {
    _st_esos_uiF14Data.b_LED3On ^= 1;
    LED3_HB_TOGGLE();
    return;
}

inline void esos_uiF14_flashLED3( uint16_t u16_period) {
    _st_esos_uiF14Data.u16_LED1FlashPeriod = u16_period;
    esos_uiF14_turnLED3On();
    DELAY_MS(u16_period / 2);
    esos_uiF14_turnLED3Off();
    DELAY_MS(u16_period / 2);
    return;
}

//RED LED / LED1
inline void esos_uiF14_turnRedLEDOn (void) {
    esos_uiF14_turnLED1On();
}

inline void esos_uiF14_turnRedLEDOff (void) {
    esos_uiF14_turnLED1Off();
}

//YELLOW LED / LED2
inline void esos_uiF14_turnYellowLEDOn (void) {
    esos_uiF14_turnLED2On();
}

inline void esos_uiF14_turnYellowLEDOff (void) {
    esos_uiF14_turnLED2Off();
}

//GREEN LED / LED3
inline void esos_uiF14_turnGreenLEDOn (void) {
    esos_uiF14_turnLED3On();
}

inline void esos_uiF14_turnGreenLEDOff (void) {
    esos_uiF14_turnLED3Off();
}

// PUBLIC RPG FUNCTIONS

inline uint16_t esos_uiF14_getRpgValue_u16 ( void ) {
    return _st_esos_uiF14Data.u16_RPGCounter;
}

inline bool esos_uiF14_isRpgTurning ( void ) {
    return (esos_uiF14_getRpgVelocity_i16() != 0);
}

inline bool esos_uiF14_isRpgTurningSlow( void ) {
    return (abs(esos_uiF14_getRpgVelocity_i16()) <= 5);
}

inline bool esos_uiF14_isRpgTurningMedium( void ) {
    return (5 < abs(esos_uiF14_getRpgVelocity_i16()) < 10);
}

inline bool esos_uiF14_isRpgTurningFast( void ) {
    return (10 <= abs(esos_uiF14_getRpgVelocity_i16()));
}

inline bool esos_uiF14_isRpgTurningCW( void ) {
    return (esos_uiF14_getRpgVelocity_i16() > 0);
}

inline bool esos_uiF14_isRpgTurningCCW( void ) {
    return (esos_uiF14_getRpgVelocity_i16() < 0);
}

int16_t esos_uiF14_getRpgVelocity_i16( void ) {
    return _st_esos_uiF14Data.u16_RPGCounter - _st_esos_uiF14Data.u16_lastRPGCounter;
}

// UIF14 INITIALIZATION FUNCTION
void config_esos_uiF14() {
  // Configure LEDs
  LED1_CONFIG();
  LED2_CONFIG();
  LED3_HB_CONFIG();

  // Configure switches
  SW1_CONFIG();
  SW2_CONFIG();
  SW3_CONFIG();

  // Configure rotary encoder
  CONFIG_RPGA();
  CONFIG_RPGB();

  // Set RPG counter to 0
  _esos_uiF14_setRPGCounter(0);

  esos_RegisterTask( __uiF14_task );
}

// UIF14 task to manage user-interface
ESOS_USER_TASK( __uiF14_task ){
  
  ESOS_TASK_BEGIN();
  while(TRUE) {
    // do your UI stuff here
    ESOS_TASK_WAIT_TICKS( __ESOS_UIF14_UI_PERIOD_MS );
  }
  ESOS_TASK_END();
}

// timer for checking if double pressed
ESOS_USER_TASK( DOUBLE_PRESSED_TIMER ){

    ESOS_TASK_BEGIN();
    timer_on = 1;
    timer_set(&timer, dptime);
    ESOS_TASK_WAIT_UNTIL(timer_expired(&timer));
    timer_on = 0;
    ESOS_TASK_END();
}

