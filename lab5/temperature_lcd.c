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

uint16_t adc_data;
char string_data[10];
bool displayTemp = false;

uint8_t temp_level; // Temperature level
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
    ESOS_TASK_WAIT_TICKS(1000); // Give the LCD time to initialize
    esos_lcd44780_configDisplay();
    esos_lcd44780_clearScreen();
    esos_lcd44780_setCursorDisplay(false);
    esos_lcd44780_setCursorBlink(false);
    while (true) {
        esos_uiF14_checkHW();
        memset(string_data, ' ', 10); // clear data array
        if (displayTemp) {
            // display temperature
            ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH03, ESOS_SENSOR_VREF_1V0);
            ESOS_TASK_WAIT_SENSOR_READ(adc_data, 0, ESOS_SENSOR_FORMAT_VOLTAGE); // one-shot read
            ESOS_SENSOR_CLOSE();

            uint32_t pu32_out;
            uint16_t pu16_out;

            // Convert mV to degrees Celsius per datasheet
            pu32_out = (uint32_t)adc_data * 1000; // Cast to uint32_t to prevent overflow
            pu32_out = (pu32_out - 424000) / 625;
            pu32_out /= 100;
            pu16_out = (uint32_t) pu32_out; // Cast back to uint16_t for sprintf to work

            sprintf(string_data, "%dC\0", pu16_out); // Convert uint16_t to char array
            esos_lcd44780_writeString(0, 0, "LM60    \0");
            esos_lcd44780_writeString(1, 0, string_data);
            esos_lcd44780_writeString(1, 3, "     \0"); // Clear bottom part of screen

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
            ESOS_TASK_WAIT_SENSOR_READ(adc_data, 0, ESOS_SENSOR_FORMAT_VOLTAGE); // one-shot read
            ESOS_SENSOR_CLOSE();

            adc_data *= 13; // Normalize so it goes up to ~0xffff
            sprintf(string_data, "0x%x\0", adc_data >> 8); // Shift over to only display top 8 bits
            esos_lcd44780_writeString(0, 0, "pot \0");
            esos_lcd44780_writeString(0, 4, string_data);

            // Custom potentiometer voltage intervals for "slider" bar
            if (adc_data <= 0x13b7)
                esos_lcd44780_writeString(1, 0, "+-------\0");
            if ((adc_data > 0x13b7) && (adc_data <= 0x10b7 + 0x21dd))
                esos_lcd44780_writeString(1, 0, "-+------\0");
            if ((adc_data > 0x13b7 + 0x21dd) && (adc_data <= 0x10b7 + 0x43ba))
                esos_lcd44780_writeString(1, 0, "--+-----\0");
            if (adc_data > 0x13b7 + 0x43ba && adc_data <= 0x10b7 + 0x6597)
                esos_lcd44780_writeString(1, 0, "---+----\0");
            if (adc_data > 0x13b7 + 0x6597 && adc_data <= 0x10b7 + 0x8774)
                esos_lcd44780_writeString(1, 0, "----+---\0");
            if (adc_data > 0x13b7 + 0x8774 && adc_data <= 0x10b7 + 0xa951)
                esos_lcd44780_writeString(1, 0, "-----+--\0");
            if (adc_data > 0x13b7 + 0xa951 && adc_data <= 0x10b7 + 0xcb2e)
                esos_lcd44780_writeString(1, 0, "------+-\0");
            if (adc_data > 0x13b7 + 0xcb2e && adc_data <= 0x10b7 + 0xed0b)
                esos_lcd44780_writeString(1, 0, "-------+\0");
        }
        // Change display state when SW3 is pressed
        if (esos_uiF14_isSW3Pressed()) {
            outString("SW3 pressed\n");
            ESOS_TASK_WAIT_TICKS(100); // Wait to give it time to be released
            displayTemp = !displayTemp;
        }
        ESOS_TASK_WAIT_TICKS(250); // Wait so screen has time to update
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

