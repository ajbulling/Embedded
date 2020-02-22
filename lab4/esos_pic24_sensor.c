#include "esos_pic24_sensor.h"
#include "esos_sensor.h"

void esos_sensor_initiate_hw(){
    esos_SetUserFlag(ESOS_SENSOR_IS_CONVERTING_FLAG);
    AD1CON1bits.SAMP = 1;
}

bool esos_sensor_is_converting_hw(){
    return !AD1CON1bits.DONE;
}

uint16_t esos_sensor_getvalue_u16_hw(){
    // refer to adc7scan1 from micro code repo
    uint16_t* au16_adcHWBuff = (uint16_t*) &ADC1BUF0;
    return *au16_adcHWBuff;
}

void esos_sensor_config_hw( esos_sensor_ch_t sensor_channel, esos_sensor_vref_t sensor_vref ){
    
}

void esos_sensor_release_hw(){
    esos_ClearUserFlag(ESOS_SENSOR_IS_CONVERTING_FLAG);
    AD1CON1bits.SAMP = 0;
}
