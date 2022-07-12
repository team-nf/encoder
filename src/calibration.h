#ifndef _CALIB_H_INCLUDED
#define _CALIB_H_INCLUDED

#include "encoder.h"

#define _version_skip_key_g "99999999"

typedef struct calibration_meta_t {
	char calibration_start[16];
	char version[8];
	int sensor_num;
};

typedef struct calibration_data_t {
	calibration_meta_t header;
	sensor_data_t sensor_datas[_sensor_num_g];
} calibration_data_t;

const calibration_meta_t _example_meta_g = { _calibration_start_g, _version_g, _sensor_num_g };
	

#define eeprom_read(start_address) \
	(calibration_data_t *)_eeprom_read(start_address, sizeof(calibration_data_t))
void* _eeprom_read(int start_address, int size);


#define eeprom_write(start_address, buffer) \
	_eeprom_write(start_address, (void *)buffer, sizeof(calibration_data_t))
void _eeprom_write(int start_address, void* buffer, int size);



#define calibration_check_eeprom() _calibration_check(_eeprom_address_g, &_example_meta_g)
bool _calibration_check_eeprom(int eeprom_address, const calibration_meta_t* example_meta);
bool _calibration_check(const calibration_data_t *data, const calibration_meta_t* example_meta);

bool _calibration_check_exists_eeprom(int eeprom_address, const calibration_meta_t* example_meta);
bool _calibration_check_exists(const calibration_meta_t* data_header, const calibration_meta_t* example_meta);

bool _calibration_check_sensor_data_eeprom(int eeprom_address, int sensor_num);
bool _calibration_check_sensor_data(const sensor_data_t* sensor_data, int sensor_num);



#define calibration_read(buffer) _calibration_read(buffer, _eeprom_address_g, _sensor_num_g)
bool _calibration_read(sensor_data_t* buffer, int eeprom_address, int sensor_num);

#define calibration_write(sensor_datas) _calibration_write(sensor_datas, _eeprom_address_g)
bool _calibration_write(const calibration_data_t* data, int eeprom_address);


#define calibrate_sensors(buffer) _calibrate_sensors(buffer, _first_sensor_pin_g, _sensor_num_g, _mode_g, &_example_meta_g)
bool _calibrate_sensors(sensor_data_t* buffer, int first_sensor, int sensor_num, enum encoder_mode_g mode, const calibration_meta_t* example_meta);


#ifdef CALIB_IMPL
#include "impl/calibration_impl.h"
#endif

#endif
