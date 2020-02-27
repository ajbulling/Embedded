/* ECE 4723 Embedded Systems

For Microchip dsPIC33EP512GP806

*/

#pragma once
#define REVF14_H

// Configure LED1
#define LED1_CONFIG() CONFIG_RF4_AS_DIG_OUTPUT();
#define LED1(_LATF4)
#define LED1_ON() (_LATF4 = 1)
#define LED1_OFF() (_LATF4 = 0)
#define LED1_HB_TOGGLE() (_LATB14 = !_LATB14)

// Configure LED2
#define LED2_CONFIG() CONFIG_RB14_AS_DIG_OUTPUT()
#define LED2(_LATB14)
#define LED2_ON() _LATB14 = 1;
#define LED2_OFF() _LATB14 = 0;
#define LED2_HB_TOGGLE() _LATB14 = !_LATB14;

// Configure LED3
#define LED3_HB_CONFIG() CONFIG_RB15_AS_DIG_OUTPUT()
#define LED3_HB (_LATB15)
// These may be backward but IDK.
// On the schematic the control pin is backward from the other two.
// I have swapped the control as the pin acts as the LED ground pin for LED3.
#define LED3_HB_ON() (_LATB15 = 0)
#define LED3_HB_OFF() (_LATB15 = 1)
#define LED3_HB_TOGGLE() (_LATB15 = !_LATB15)

// Configure SW1
#define SW1_CONFIG() {          \
    CONFIG_RB13_AS_DIG_INPUT(); \
    ENABLE_RB13_PULLUP();       \
}
#define DELAY_1US() DELAY_US(1);
#define SW1 (_RB13)
#define SW1_RELEASED (_RB13 == 1)
#define SW1_PRESSED (_RB13 == 0)
#define SW1_DOUBLEPRESSED 0

// Configure SW2
#define SW2_CONFIG() {          \
    CONFIG_RB12_AS_DIG_INPUT(); \
    ENABLE_RB12_PULLUP();       \
}
#define SW2 (_RB12)
#define SW2_RELEASED (_RB12 == 1)
#define SW2_PRESSED (_RB12 == 0)

// Configure SW3
#define SW3_CONFIG() {          \
    CONFIG_RC15_AS_DIG_INPUT(); \
    ENABLE_RC15_PULLUP();       \
}
#define SW3 (_RC15)
#define SW3_RELEASED (_RC15 == 1)
#define SW3_PRESSED (_RC15 == 0)

// Configure RPG
#define CONFIG_RPGA() CONFIG_RB8_AS_DIG_INPUT()
#define RPGA _RB8
#define CONFIG_RPGB() CONFIG_RB9_AS_DIG_INPUT()
#define RPGB _RB9
#define RPG_TURN_CW() (RPGA == 1 && RPGB == 0)
#define RPG_TURN_CCW() (RPGA == 0 && RPGB == 1)
#define RPG_STOPPED() (RPGA == 0 && RPGB == 0)

//Configure Potentiometer
#define CONFIG_POT() CONFIG_RB2_AS_ANALOG()
#define POT _RB2

//Configure Temp Sensor
#define CONFIG_TEMP() CONFIG_RB3_AS_ANALOG()
#define TEMP _RB3

// Configure LCD
#define CONFIG_LCD() { \
    CONFIG_RC12_AS_DIG_OUTPUT(); \
    CONFIG_RD10_AS_DIG_OUTPUT(); \
    CONFIG_RD11_AS_DIG_OUTPUT(); \
}
#define LCD_RS (_LATC12) // Register select pin
#define LCD_E (_LATD10) // Enable pin
#define LCD_RW (_LATD11) // Read/write pin

