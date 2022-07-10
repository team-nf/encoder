#ifndef _CALIB_H_INCLUDED
#define _CALIB_H_INCLUDED

#include "encoder.h"

typedef struct calibration_data_t {
	char calibration_start[16];
	char version[8];
	int sensor_num;
	sensor_data_t sensor_datas[_sensor_num_g];
	char calibration_end[16];
} calibration_data_t;


#define eeprom_read(start_address) \
	(calibration_data_t *)_eeprom_read(start_address, sizeof(calibration_data_t))

void* _eeprom_read(int start_address, int size);


#define eeprom_write(start_address, buffer) \
	_eeprom_write(start_address, (void *)buffer, sizeof(calibration_data_t))

void _eeprom_write(int start_address, void* buffer, int size);


#define calibration_check() \
	_calibration_check(_eeprom_address_g, _calibration_start_g, _version_g, _sensor_num_g, _calibration_end_g)

bool _calibration_check(int eeprom_address, char calibration_start[16], char version[8], int sensor_num, char calibration_end[16]);


#define calibration_read(buffer) _calibration_read(buffer, _eeprom_address_g, _sensor_num_g)
bool _calibration_read(sensor_data_t* buffer, int eeprom_address, int sensor_num);


#define calibration_write(sensor_datas) \
	_calibration_write(sensor_datas, _eeprom_address_g, _calibration_start_g, _version_g, _sensor_num_g, _calibration_end_g)

bool _calibration_write(sensor_data_t sensor_datas[_sensor_num_g], int eeprom_address, \
	char calibration_start[16], char version[8], int sensor_num, char calibration_end[16]);


bool calibrate_sensors(sensor_data_t* buffer, int first_sensor, int sensor_num, enum encoder_mode_g mode);


#ifdef CALIB_IMPL
#include "impl/calibration_impl.h"
#endif

#endif