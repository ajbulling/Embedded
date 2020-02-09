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

// PRIVATE FUNCTIONS
inline void _esos_uiF14_setRPGCounter (uint16_t newValue) {
    _st_esos_uiF14Data.u16_RPGCounter = newValue;
    return;
}

inline void _esos_uiF14_setLastRPGCounter (uint16_t newValue) {
    _st_esos_uiF14Data.u16_lastRPGCounter = newValue;
    return;
}

// PUBLIC SWITCH FUNCTIONS

//SW1
inline bool esos_uiF14_getSW1Pressed (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==TRUE);
}

inline bool esos_uiF14_getSW1Released (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==FALSE);
}

inline bool esos_uiF14_getSW1DoublePressed (void) {
    return (_st_esos_uiF14Data.b_SW1DoublePressed==TRUE);
}

//SW2
inline bool esos_uiF14_getSW2Pressed (void) {
    return (_st_esos_uiF14Data.b_SW2Pressed==TRUE);
}

inline bool esos_uiF14_getSW2Released (void) {
    return (_st_esos_uiF14Data.b_SW2Pressed==FALSE);
}

inline bool esos_uiF14_getSW2DoublePressed (void) {
    return (_st_esos_uiF14Data.b_SW2DoublePressed==TRUE);
}

//SW3
inline bool esos_uiF14_getSW3Pressed (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==TRUE);
}

inline bool esos_uiF14_getSW3Released (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==FALSE);
}

inline bool esos_uiF14_getSW3DoublePressed (void) {
    return (_st_esos_uiF14Data.b_SW1DoublePressed==TRUE);
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
    return;
}

inline void esos_uiF14_turnLED1Off (void) {
    _st_esos_uiF14Data.b_LED1On = FALSE;
    return;
}

inline void esos_uiF14_toggleLED1 (void) {
    _st_esos_uiF14Data.b_LED1On ^= 1;
    return;
}

inline void esos_uiF14_flashLED1( uint16_t u16_period) {
    _st_esos_uiF14Data.u16_LED1FlashPeriod = u16_period;
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
    return;
}

inline void esos_uiF14_turnLED2Off (void) {
    _st_esos_uiF14Data.b_LED2On = FALSE;
    return;
}

inline void esos_uiF14_toggleLED2 (void) {
    _st_esos_uiF14Data.b_LED2On ^= 1;
    return;
}

inline void esos_uiF14_flashLED2( uint16_t u16_period) {
    _st_esos_uiF14Data.u16_LED2FlashPeriod = u16_period;
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
    return;
}

inline void esos_uiF14_turnLED3Off (void) {
    _st_esos_uiF14Data.b_LED3On = FALSE;
    return;
}

inline void esos_uiF14_toggleLED3 (void) {
    _st_esos_uiF14Data.b_LED3On ^= 1;
    return;
}

inline void esos_uiF14_flashLED3( uint16_t u16_period) {
    _st_esos_uiF14Data.u16_LED1FlashPeriod = u16_period;
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
    return _st_esos_uiF14Data.i16_RPGVelocity;
}

// UIF14 INITIALIZATION FUNCTION
void config_esos_uiF14() {
  // setup your UI implementation
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

void user_init() {}

