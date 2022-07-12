#ifndef _CALIB_IMPL_H_INCLUDED
#define _CALIB_IMPL_H_INCLUDED

#ifdef CALIB_IMPL
#include <EEPROM.h>
/* #include "../calibration.h" */


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


bool _calibration_check_exists_eeprom(int eeprom_address, const calibration_meta_t* example_meta) {
	calibration_meta_t *read_meta = (calibration_meta_t *)_eeprom_read(eeprom_address, sizeof(calibration_meta_t));
	bool rv = _calibration_check_exists(read_meta, example_meta);
	free(read_meta); return rv;
}

bool _calibration_check_exists(const calibration_meta_t* data_header, const calibration_meta_t* example_meta) {
	if (!strncmp(example_meta->calibration_start,		data_header->calibration_start,	16)) { return false; }
	if (!strncmp(example_meta->version,					data_header->version,			 8 )&& 
	    !strncmp(example_meta->version,					_version_skip_key_g,			 8)) { return false; }
	if (		 example_meta->sensor_num !=			data_header->sensor_num				&&
				 example_meta->sensor_num !=			0								   ) { return false; }
	return true;
}


bool _calibration_check_sensor_data_eeprom(int eeprom_address, int sensor_num) {
	calibration_meta_t *read_meta = (calibration_meta_t *)_eeprom_read(eeprom_address, sizeof(calibration_meta_t));
	sensor_data_t *sensor_datas = (sensor_data_t *)_eeprom_read(eeprom_address + sizeof(calibration_meta_t), \
															   read_meta->sensor_num * sizeof(sensor_data_t));
	bool rv = _calibration_check_sensor_data(sensor_datas, sensor_num);
	free(read_meta); free(sensor_datas); return rv;
}

bool _calibration_check_sensor_data(const sensor_data_t* sensor_datas, int sensor_num) {
	for (int i=0; i < sensor_num; i++) {
		if (0 >= sensor_datas[i]._min || sensor_datas[i]._min >= 1023) { return false; } 
		if (0 >= sensor_datas[i]._max || sensor_datas[i]._max >= 1023) { return false; } 
		if (0 >= sensor_datas[i]._normal || sensor_datas[i]._normal >= 1023) { return false; } 
		if (sensor_datas[i]._min >= sensor_datas[i]._normal || sensor_datas[i]._normal >= sensor_datas[i]._max) { return false; } 
	} return true;
}

bool _calibration_check_eeprom(int eeprom_address, const calibration_meta_t* example_meta) {
	calibration_data_t *read_data = eeprom_read(eeprom_address);
	bool rv = _calibration_check(read_data, example_meta);
	free(read_data); return rv;
}

bool _calibration_check(const calibration_data_t *data, const calibration_meta_t* example_meta) {
	if (!_calibration_check_exists(&data->header, example_meta))												{ return false; }
	if (!_calibration_check_sensor_data((sensor_data_t *)&data->sensor_datas, example_meta->sensor_num))		{ return false; }
	return true;
}



bool _calibration_read(sensor_data_t* buffer, int eeprom_address, int sensor_num) {
	calibration_data_t *read_data = eeprom_read(eeprom_address);
	memcpy(buffer, read_data->sensor_datas, sensor_num * sizeof(sensor_data_t));
	free(read_data); return true;
}


bool _calibration_write(const calibration_data_t* data, int eeprom_address) {
	eeprom_write(eeprom_address, &data);
	return true;
}


bool _calibration_update(const calibration_data_t* data, int eeprom_address) {
	calibration_data_t *read = (calibration_data_t *)_eeprom_read(eeprom_address, sizeof(calibration_data_t));
	return true;
}


bool _calibrate_sensors(sensor_data_t* buffer, int first_sensor, int sensor_num, enum encoder_mode_g mode) {
	return true;
}


#endif
#endif
