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

static uint16_t* pdata_buffer;
char print_string[5];
static ESOS_TASK_HANDLE read_adc;

ESOS_USER_TASK ( QUICK_READ_TEST ) {
    ESOS_TASK_BEGIN();
    while ( TRUE ){
        ESOS_ALLOCATE_CHILD_TASK( read_adc );
        ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH00, ESOS_SENSOR_VREF_1V0);
        ESOS_TASK_WAIT_SENSOR_QUICK_READ(pdata_buffer);
        ESOS_SENSOR_CLOSE();
        
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pdata_buffer);
        ESOS_TASK_WAIT_ON_SEND_UINT8('\n');
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        
    }
    ESOS_TASK_END();
}

void user_init() {
    CONFIG_POT();

    esos_RegisterTask( QUICK_READ_TEST );
}

