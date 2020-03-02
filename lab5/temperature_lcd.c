#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "esos_sensor.h"
#include "../lib/src/esos_sensor.c"
#include "pic24_all.h"
#include "revF14.h"
#include "esos_f14ui.h"
#include "esos_f14ui.c"
#include "esos.h"
#include "esos_pic24_sensor.h"
#include "esos_pic24_sensor.c"
#include "esos_lcd44780.h"
#include "../lib/src/esos_lcd44780.c"
#include "esos_pic24_lcd44780.h"
#include "esos_pic24_lcd44780.c"

uint16_t temp_data; // Temporary ADC data
char* processMode [1];
char* numSamples [1];
char temp_string[10]; // Temporary char array
uint8_t input1; // Process mode (integer)
uint8_t input2; // Number of samples (integer)
uint8_t pmode = 0; // Actual process mode argument (must be calculated)
bool continuousOutput = false;
bool displayTemp = false;

uint8_t temp_level; // Temp stands for temperature
uint8_t temp_bar[8][8] = { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F},   // 1/8 full block
                                              {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F},   // 1/4 full block
                                              {0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F},   // 3/8 full block
                                              {0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F},   // Half full block
                                              {0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F},   // 5/8 full block
                                              {0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F},   // 3/4 full block
                                              {0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F},   // 7/8 full block
                                              {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F} };  // Full block

// ESOS task for Heartbeat LED3
ESOS_USER_TASK ( LED3_blink ) {
    ESOS_TASK_BEGIN();
    while ( TRUE ){
        esos_uiF14_toggleLED3();
        ESOS_TASK_WAIT_TICKS(500);
    }
    ESOS_TASK_END();
}

ESOS_USER_TASK( LCD_INTERFACE ) {
    ESOS_TASK_BEGIN();
    ESOS_TASK_WAIT_TICKS(1000);
    esos_lcd44780_configDisplay();
    esos_lcd44780_clearScreen();
    esos_lcd44780_setCursorDisplay(false);
    esos_lcd44780_setCursorBlink(false);
    while (true) {
        esos_uiF14_checkHW();
        memset(temp_string, ' ', 10);
        if (displayTemp) {
            // display temperature
            ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH03, ESOS_SENSOR_VREF_1V0);
            ESOS_TASK_WAIT_SENSOR_READ(temp_data, 0, ESOS_SENSOR_FORMAT_VOLTAGE); // one-shot read
            ESOS_SENSOR_CLOSE();

            uint32_t pu32_out;
            uint16_t pu16_out;

            // Convert mV to degrees Celsius per datasheet
            pu32_out = (uint32_t)temp_data * 1000; // Cast to uint32_t to prevent overflow
            pu32_out = (pu32_out - 424000) / 625;
            pu32_out /= 100;
            pu16_out = (uint32_t) pu32_out;

            sprintf(temp_string, "%dC\0", pu16_out);
            esos_lcd44780_writeString(0, 0, "LM60    \0");
            esos_lcd44780_writeString(1, 0, temp_string);
            esos_lcd44780_writeString(1, 3, "     \0"); // Don't worry about it

            temp_level = (pu16_out % 20) % 8; //determing custom char
            esos_lcd44780_setCustomChar(0, temp_bar[temp_level]); //Displaying custom char
            if (pu16_out >= 35) {
                esos_lcd44780_writeChar(0, 7, 0xFF);
                esos_lcd44780_writeChar(1, 7, 0xFF);
            }
            else if (pu16_out > 27) {
                esos_lcd44780_writeChar(0, 7, 0);
                esos_lcd44780_writeChar(1, 7, 0xFF);
            }
            else if (pu16_out >= 20) {
                esos_lcd44780_writeChar(0, 7, (uint8_t)' ');
                esos_lcd44780_writeChar(1, 7, 0);
            }
            else {
                esos_lcd44780_setCustomChar(0, temp_bar[0]);
                esos_lcd44780_writeChar(0, 7, (uint8_t)' ');
                esos_lcd44780_writeChar(1, 7, 0);

            }
        }
        else {
            // display potentiometer
            ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH02, ESOS_SENSOR_VREF_1V0);
            ESOS_TASK_WAIT_SENSOR_READ(temp_data, 0, ESOS_SENSOR_FORMAT_VOLTAGE);
            ESOS_SENSOR_CLOSE();

            temp_data *= 13; // Normalize so it goes up to ~0xffff
            sprintf(temp_string, "0x%x\0", temp_data >> 8);
            esos_lcd44780_writeString(0, 0, "pot \0");
            esos_lcd44780_writeString(0, 4, temp_string);
/*
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
            ESOS_TASK_WAIT_ON_SEND_UINT16_AS_HEX_STRING(temp_data * 13);
            ESOS_TASK_WAIT_ON_SEND_UINT8('\n');
            ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
            */

            if (temp_data <= 0x10b7)
                esos_lcd44780_writeString(1, 0, "+-------\0");
            if ((temp_data > 0x10b7) && (temp_data <= 0x10b7 + 0x21dd))
                esos_lcd44780_writeString(1, 0, "-+------\0");
            if ((temp_data > 0x10b7 + 0x21dd) && (temp_data <= 0x10b7 + 0x43ba))
                esos_lcd44780_writeString(1, 0, "--+-----\0");
            if (temp_data > 0x10b7 + 0x43ba && temp_data <= 0x10b7 + 0x6597)
                esos_lcd44780_writeString(1, 0, "---+----\0");
            if (temp_data > 0x10b7 + 0x6597 && temp_data <= 0x10b7 + 0x8774)
                esos_lcd44780_writeString(1, 0, "----+---\0");
            if (temp_data > 0x10b7 + 0x8774 && temp_data <= 0x10b7 + 0xa951)
                esos_lcd44780_writeString(1, 0, "-----+--\0");
            if (temp_data > 0x10b7 + 0xa951 && temp_data <= 0x10b7 + 0xcb2e)
                esos_lcd44780_writeString(1, 0, "------+-\0");
            if (temp_data > 0x10b7 + 0xcb2e && temp_data <= 0x10b7 + 0xed0b)
                esos_lcd44780_writeString(1, 0, "-------+\0");
        }
        if (esos_uiF14_isSW3Pressed()) {
            outString("SW3 pressed\n");
            ESOS_TASK_WAIT_TICKS(100);
            displayTemp = !displayTemp;
        }
        ESOS_TASK_WAIT_TICKS(250);
    }
    ESOS_TASK_END();
}

void user_init() {
    // Config HW
    CONFIG_POT();
    CONFIG_TEMP();
    LED3_HB_CONFIG();
    SW3_CONFIG();
    esos_lcd44780_init();

    // Register ESOS tasks
    esos_RegisterTask( LED3_blink );
    esos_RegisterTask( LCD_INTERFACE );
}

