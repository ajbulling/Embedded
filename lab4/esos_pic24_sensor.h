
#pragma once

#define ESOS_SENSOR_IS_CONVERTING_FLAG ESOS_USER_FLAG_0

void esos_sensor_initiate_hw();
bool esos_sensor_is_converting_hw();
uint16_t esos_sensor_getvalue_u16_hw();
void esos_sensor_config_hw(esos_sensor_ch_t, esos_sensor_vref_t);
void esos_sensor_release_hw();
