#ifndef _ENCODER_H_INCLUDED
#define _ENCODER_H_INCLUDED


#define _calibration_pin_g 2
#define _sensor_num_g 3
#define _DEBUG


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


const int _first_sensor_pin_g						= A0;
enum encoder_mode_g _mode_g							= em_analog_digital;


typedef struct sensor_data_t {
	int _min, _normal, _max;
} sensor_data_t;


/* sensor_data_t* _init_sensor_datas(sensor_data_t limits) {
	sensor_data_t new_data[_sensor_num_g];
	for (int i=0; i < _sensor_num_g; i++)
		new_data[i] = limits;
	return new_data;
} */


#endif
