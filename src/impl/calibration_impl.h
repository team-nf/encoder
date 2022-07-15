#ifndef _CALIB_IMPL_H_INCLUDED
#define _CALIB_IMPL_H_INCLUDED

#ifdef CALIB_IMPL
#include <EEPROM.h>


byte* eeprom_read(int start_address, int size) {
	byte *buffer = (byte *)malloc(size);
	for (size_t i=0; i < size; i++) {
		buffer[i] = EEPROM.read(start_address + i);
	} return buffer;
}


void eeprom_write(int start_address, byte* buffer, int size) {
	for (size_t i=0; i < size; i++) {
		EEPROM.write(start_address + i, buffer[i]);
	}
}


bool calibration_check_exists(const calibration_meta_t* data_header, const calibration_meta_t* example_meta) {
	if (strcmp(example_meta->calibration_start,	data_header->calibration_start) != 0) { return false; }
	if (strcmp(example_meta->version,			data_header->version		  ) != 0 && \
	    strcmp(example_meta->version,			_version_skip_key_g			  ) != 0) { return false; }
	if (	   example_meta->sensor_num !=			data_header->sensor_num			 && \
			   example_meta->sensor_num !=			0								) { return false; }
	return true;
}

bool calibration_check_sensor_data(const sensor_data_t* sensor_datas, int sensor_num) {
	for (int i=0; i < sensor_num; i++) {
		if (0 > sensor_datas[i]._min || sensor_datas[i]._min > 1023) { return false; } 
		if (0 > sensor_datas[i]._max || sensor_datas[i]._max > 1023) { return false; } 
		if (0 >= sensor_datas[i]._normal || sensor_datas[i]._normal >= 1023) { return false; } 
		if (sensor_datas[i]._min >= sensor_datas[i]._normal || sensor_datas[i]._normal >= sensor_datas[i]._max) { return false; } 
	} return true;
}

bool calibration_check_equals(const calibration_data_t* data1, const calibration_data_t* data2) {
	if (calibration_check_exists(&data1->header, &data2->header) == false)		{ return false; }
	for (int i=0; i < data1->header.sensor_num; i++) {
		if (data1->sensor_datas[i]._min != data2->sensor_datas[i]._min)			{ return false; }
		if (data1->sensor_datas[i]._max != data2->sensor_datas[i]._max)			{ return false; }
		if (data1->sensor_datas[i]._normal != data2->sensor_datas[i]._normal)	{ return false; }
	} return true;
}

bool calibration_check_eeprom(int eeprom_address, const calibration_meta_t* example_meta) {
	calibration_data_t *read_data = calibration_read(eeprom_address);
	bool rv = calibration_check(read_data, example_meta);
	free(read_data); return rv;
}

bool calibration_check(const calibration_data_t *data, const calibration_meta_t* example_meta) {
	if (!calibration_check_exists(&data->header, example_meta))											{ return false; }
	if (!calibration_check_sensor_data((sensor_data_t *)&data->sensor_datas, example_meta->sensor_num))	{ return false; }
	return true;
}


#ifdef _DEBUG
void calibration_print(const calibration_data_t* data) {
	serialf("Start: %s\nVersion: %s\nSensor Count: %d\n", \
		data->header.calibration_start, data->header.version, data->header.sensor_num);
	for (int i=0; i < data->header.sensor_num; i++) {
		serialf("\tSensor %d: %d %d %d\n", data->sensor_datas[i]._min, \
			data->sensor_datas[i]._normal, data->sensor_datas[i]._max);
	}
}
#endif


bool calibrate_sensors(sensor_data_t* buffer, int first_sensor, int sensor_num, enum encoder_mode_g mode, const calibration_meta_t* example_meta) {
	return true;
}


#endif
#endif
