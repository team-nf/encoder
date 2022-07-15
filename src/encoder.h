#ifndef _ENCODER_H_INCLUDED
#define _ENCODER_H_INCLUDED


#define _calibration_pin_g 2
#define _sensor_num_g 3
#define _DEBUG

/* release için derleme hızı artsın diye */
#ifdef _DEBUG
#define serialf(...) ({ \
	int _size = snprintf(NULL, 0, __VA_ARGS__) + 1; \
	char* _str = (char *)malloc(_size * sizeof(char)); \
	snprintf(_str, _size, __VA_ARGS__); \
	Serial.print(_str); \
	free(_str); \
})
#endif

#define blink(time, repetition) ({ \
	for (int i=0; i < repetition+2; i++) { \
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); \
		delay(time); \
	} \
})


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
