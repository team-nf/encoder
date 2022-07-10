#ifndef _CALIB_IMPL_H_INCLUDED
#define _CALIB_IMPL_H_INCLUDED

#ifdef CALIB_IMPL
#include <EEPROM.h>


void* _eeprom_read(int start_address, int size) {
	byte *buffer = (byte *)malloc(size);
	for (size_t i=0; i < size; i++) {
		buffer[i] = EEPROM.read(start_address + i);
	} return (void *)buffer;
}


void _eeprom_write(int start_address, void* buffer, int size) {
	for (size_t i=0; i < size; i++) {
		EEPROM.write(start_address + i, *((byte *)buffer + i));
	}
}



bool _calibration_check(int eeprom_address, char calibration_start[16], char version[8], int sensor_num, char calibration_end[16]) {
	calibration_data_t *read_data = eeprom_read(eeprom_address);
	if (strncmp(calibration_start,	read_data->calibration_start, 16)) { return false; }
	if (strncmp(version,			read_data->version		    , 8)) { return false; }
	if ( 	    sensor_num !=		read_data->sensor_num	        ) { return false; }
	if (strncmp(calibration_end,	read_data->calibration_end  , 16)) { return false; }


	sensor_data_t sensor_data;
	for (int i=0; i < sensor_num; i++) {
		sensor_data = read_data->sensor_datas[i]; 
		if (0 >= sensor_data._min || sensor_data._min >= 1023) { return false; } 
		if (0 >= sensor_data._max || sensor_data._max >= 1023) { return false; } 
		if (0 >= sensor_data._normal || sensor_data._normal >= 1023) { return false; } 
		if (sensor_data._min >= sensor_data._normal || sensor_data._normal >= sensor_data._max) { return false; } 
	} return true;
}


bool _calibration_read(sensor_data_t* buffer, int eeprom_address, int sensor_num) {
#ifdef _DEBUG
	if(!calibration_check()) { 
		Serial.println("Check before you read!"); 
		return false;
	}
#endif
	calibration_data_t *read_data = eeprom_read(eeprom_address);
	memcpy(buffer, read_data->sensor_datas, sensor_num * sizeof(sensor_data_t));
	return true;
}


bool _calibration_write(sensor_data_t sensor_datas[_sensor_num_g], int eeprom_address, \
	char calibration_start[16], char version[8], int sensor_num, char calibration_end[16]) 
{
	calibration_data_t data = {calibration_start, version, sensor_num, sensor_datas, calibration_end};
	eeprom_write(eeprom_address, &data);
	return true;
}


bool calibrate_sensors(sensor_data_t* buffer, int first_sensor, int sensor_num, enum encoder_mode_g mode) {
	return true;
}


#endif
#endif
