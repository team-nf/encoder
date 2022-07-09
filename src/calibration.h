#ifndef _CALIB_H_INCLUDED
#define _CALIB_H_INCLUDED

#define _calibration_start_g "calibrationstart"
#define _calibration_end_g "calibration_end_"
#define _version_g "00000000"
#define _eeprom_address_g 0


enum encoder_mode_g {
	em_analog,
	em_digital,
	em_analog_digital,
	em_count
};


typedef struct sensor_data_t {
	int _min, _normal, _max;
} sensor_data_t;


sensor_data_t* _init_sensor_datas(sensor_data_t limits) {
	sensor_data_t new_data[_sensor_num_g];
	for (int i=0; i < _sensor_num_g; i++)
		new_data[i] = limits;
	return new_data;
}

typedef struct calibration_data_t {
	char calibration_start[16];
	char version[8];
	int sensor_num;
	sensor_data_t sensor_datas[_sensor_num_g];
	char calibration_end[16];
} calibration_data_t;



#define eeprom_read(start_address) \
	(calibration_data_t *)_eeprom_read(start_address, sizeof(calibration_data_t))

void* _eeprom_read(int start_address, int size) {
	byte *buffer = (byte *)malloc(size);
	for (size_t i=0; i < size; i++) {
		buffer[i] = EEPROM.read(start_address + i);
	} return (void *)buffer;
}


#define eeprom_write(start_address, buffer) \
	_eeprom_write(start_address, (void *)buffer, sizeof(calibration_data_t))

void _eeprom_write(int start_address, void* buffer, int size) {
	for (size_t i=0; i < size; i++) {
		EEPROM.write(start_address + i, *((byte *)buffer + i));
	}
}


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

#include "impl/calibration_impl.h"

#endif
