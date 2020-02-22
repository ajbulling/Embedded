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

uint16_t pot_data;

ESOS_USER_TASK ( QUICK_READ_TEST ) {
    ESOS_TASK_BEGIN();
    while ( TRUE ){
        // Ch 2
        ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH02, ESOS_SENSOR_VREF_1V0);
        ESOS_TASK_WAIT_SENSOR_QUICK_READ(pot_data);
        ESOS_SENSOR_CLOSE();
        
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING((uint32_t)pot_data);
        ESOS_TASK_WAIT_ON_SEND_UINT8('\n');
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        
    }
    ESOS_TASK_END();
}

void user_init() {
    CONFIG_POT();

    esos_RegisterTask( QUICK_READ_TEST );
}

