#include "esos_pic24_sensor.h"
#include "esos_sensor.h"

void esos_sensor_initiate_hw(){
    esos_SetUserFlag(ESOS_SENSOR_IS_CONVERTING_FLAG);
    AD1CON1bits.SAMP = 1;
}

bool esos_sensor_is_converting_hw(){
    return AD1CON1bits.SAMP;
}

uint16_t esos_sensor_getvalue_u16_hw(){
    // refer to adc7scan1 from micro code repo
    return ADC1BUF0;
}

void esos_sensor_config_hw( esos_sensor_ch_t sensor_channel, esos_sensor_vref_t sensor_vref ){
    // Turn off ADC for config register changes
    AD1CON1bits.ADON = 0;

    // Select CH0 negative value = Vref
    AD1CHS0bits.CH0NA = 0;

    // Set sensor channel
    AD1CHS0bits.CH0SA = sensor_channel;

    // Disable Stop on idle
    AD1CON1bits.ADSIDL = 0;

    // Configure DMA buffer memory
    AD1CON1bits.ADDMABM = 1;

    // Enable 12 Bit mode
    AD1CON1bits.AD12B = 1;

    // Configure Data Output format
    AD1CON1bits.FORM = 0;

    // Configure Sample Clock
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.SSRCG = 0;

    // ADC autostart config
    AD1CON1bits.ASAM = 0;

    // Configure Sample Enable
    AD1CON1bits.SAMP = 0;

    // Turn on ADC
    AD1CON1bits.ADON = 1;
}

void esos_sensor_release_hw(){
    esos_ClearUserFlag(ESOS_SENSOR_IS_CONVERTING_FLAG);
    AD1CON1bits.SAMP = 0;
}
