#ifndef _CALIB_H_INCLUDED
#define _CALIB_H_INCLUDED

#include "encoder.h"

#define _eeprom_address_g 0
#define _calibration_start_g "calibeg"
#define _version_g "0000000"
#define _version_skip_key_g "9999999"


typedef struct calibration_meta_t {
	char calibration_start[8];
	char version[8];
	int sensor_num;
};

typedef struct calibration_data_t {
	calibration_meta_t header;
	sensor_data_t sensor_datas[_sensor_num_g];
} calibration_data_t;

const calibration_meta_t _example_meta_g = { _calibration_start_g, _version_g, _sensor_num_g };
	

#define calibration_read(start_address) \
	(calibration_data_t *)_eeprom_read(start_address, sizeof(calibration_data_t))
byte* _eeprom_read(int start_address, int size);


#define calibration_write(start_address, buffer) \
	_eeprom_write(start_address, (byte *)buffer, sizeof(calibration_data_t))
void _eeprom_write(int start_address, void* buffer, int size);



#define calibration_check_eeprom() _calibration_check_eeprom(_eeprom_address_g, &_example_meta_g)
bool _calibration_check_eeprom(int eeprom_address, const calibration_meta_t* example_meta);
bool _calibration_check(const calibration_data_t *data, const calibration_meta_t* example_meta);

bool calibration_check_exists(const calibration_meta_t* data_header, const calibration_meta_t* example_meta);
bool calibration_check_sensor_data(const sensor_data_t* sensor_data, int sensor_num);
bool calibration_check_equals(const calibration_data_t* data1, const calibration_data_t* data2);

#ifdef _DEBUG
void calibration_print(const calibration_data_t* data);
#endif


#define calibrate_sensors(buffer) _calibrate_sensors(buffer, _first_sensor_pin_g, _sensor_num_g, _mode_g, &_example_meta_g)
bool _calibrate_sensors(sensor_data_t* buffer, int first_sensor, int sensor_num, enum encoder_mode_g mode, const calibration_meta_t* example_meta);


#ifdef CALIB_IMPL
#include "impl/calibration_impl.h"
#endif

#endif
