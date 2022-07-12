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


/* void* _eeprom_read(int start_address, int size); */


bool _calibration_check_exists(int eeprom_address, char calibration_start[16], char version[8], int sensor_num) {
	calibration_meta_t *read_meta = (calibration_meta_t *)_eeprom_read(eeprom_address, sizeof(calibration_meta_t));

	if (!strncmp(calibration_start,		read_meta->calibration_start,	16)) { return false; }
	if (!strncmp(version,				read_meta->version,				 8 )&& 
	    !strncmp(version,				_version_skip_key_g,			 8)) { return false; }
	if ( 	    sensor_num !=			read_meta->sensor_num	            &&
				sensor_num !=			0								   ) { return false; }
}


bool _calibration_check_sensor_data(int eeprom_address, int sensor_num) {
	calibration_meta_t *read_meta = (calibration_meta_t *)_eeprom_read(eeprom_address, sizeof(calibration_meta_t));
	sensor_data_t *sensor_data = (sensor_data_t *)_eeprom_read(eeprom_address + sizeof(calibration_meta_t), \
															   read_meta->sensor_num * sizeof(sensor_data_t));
	for (int i=0; i < sensor_num; i++) {
		if (0 >= sensor_data[i]._min || sensor_data[i]._min >= 1023) { return false; } 
		if (0 >= sensor_data[i]._max || sensor_data[i]._max >= 1023) { return false; } 
		if (0 >= sensor_data[i]._normal || sensor_data[i]._normal >= 1023) { return false; } 
		if (sensor_data[i]._min >= sensor_data[i]._normal || sensor_data[i]._normal >= sensor_data[i]._max) { return false; } 
	} return true;
}


bool _calibration_check_o(int eeprom_address, calibration_meta_t example_meta) /* wrapper function */
	{ return _calibration_check(eeprom_address, example_meta.calibration_start, example_meta.version, example_meta.sensor_num); }

bool _calibration_check(int eeprom_address, char calibration_start[16], char version[8], int sensor_num) {
	if (!_calibration_check_exists(eeprom_address, calibration_start, version, sensor_num)) { return false; }
	if (!_calibration_check_sensor_data(eeprom_address, sensor_num))						{ return false; }
	return true;
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
