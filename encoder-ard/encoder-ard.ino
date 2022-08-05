#include <EEPROM.h>

/* calibration.h dosyası zaten headerı includelıyor */
/* #include "header.h" */
#define CALIB_IMPL
#include "../src/calibration.h"

/* cinter.h, encoder.h dosyasında includelanıyor */
#define CINTER_IMPL
/* #include "cinter.h" */

#define ENCODER_IMPL
#include "../src/encoder.h"

/* 3 pinden 1 saniyede 2976 okuma (arduino uno)*/
calibration_data_t *_calibration_data_g;
struct ft_buffer *_ft_buffer_g;

point_t *_positions_g;
ftype *_distances_g;

void setup() {
	dbg(Serial.begin(115200));
	serialdn("\nStarted.");
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(_calibration_pin_g, INPUT);

	/* calibration.h setup */
/* 	bool valid_calib = 0; */
/* 	_calibration_data_g = calibration_read(_eeprom_address_g); */
/* 	if (calibration_check(_calibration_data_g, &_example_meta_g)) valid_calib = 1; */
/*  */
/* 	if (!valid_calib || digitalRead(_calibration_pin_g) == HIGH) { */
/* 		while (!valid_calib) { */
/* 			valid_calib = calibrate_sensors(_calibration_data_g, _first_sensor_pin_g); */
/* 			dbg(if (valid_calib == 0) Serial.println("Calibration failed")); */
/* 		} */
/* 		calibration_write(_eeprom_address_g, _calibration_data_g); */
/* 	} */
/* 	dbg(else Serial.println("Found valid calibration data in eeprom.")); */

	_calibration_data_g = (calibration_data_t *)malloc(sizeof(calibration_data_t));
	_calibration_data_g->magnet_projection.center = {0, 0};
	_calibration_data_g->magnet_projection.radius = 0.1;

	/* encoder.h setup */
	_positions_g = calc_sensor_positions(_sensor_num_g);
	if (_positions_g == NULL) { serialdn("calc_sensor_positions error"); }

	_ft_buffer_g = (struct ft_buffer *)malloc(sizeof(struct ft_buffer));
	if (_ft_buffer_g == NULL) { serialdn("_ft_buffer malloc error"); }
	
	_distances_g = (ftype *)malloc(_sensor_num_g * sizeof(ftype));
	if (_distances_g == NULL) { serialdn("_distances_g malloc error"); }

}


void loop() {
	serialdn("Loop started.");
	/* TODO: 
	 * calibrate_sensors, magnet_projection bulmalı 
	 *
	 */

	point_t test_point = {1, 1};
	/* int angle = 30; */
/* 	point_t test_point = {  */
/* 		_calibration_data_g->magnet_projection.radius * cos(to_radian(angle)), */
/* 		_calibration_data_g->magnet_projection.radius * sin(to_radian(angle))  */
/* 	}; */
	calc_test_distances_b(_distances_g, test_point, _positions_g, _sensor_num_g);

	/* create noise */
/* 	for (int i=0; i < _sensor_num_g; i++) */
/* 		_distances_g[i] = _distances_g[i] + 0.0001; */

	
	/* initialize circles */
	for (int i = 0; i < _sensor_num_g; i++) {
		_ft_buffer_g->circles[i].center = _positions_g[i];
		_ft_buffer_g->circles[i].radius = _distances_g[i];
	}

	find_target(_ft_buffer_g, _sensor_num_g, &_calibration_data_g->magnet_projection);
	Serial.println((int)_ft_buffer_g->ft_return_value.x);
	Serial.println((int)_ft_buffer_g->ft_return_value.y);


/* 	serialfn("test  target: (%d, %d), angle: %f",  */
/* 			test_point.x, test_point.y, to_degree(atan(test_point.y/test_point.x))); */
/* 	serialfn("found target: (%d, %d), angle: %f",  */
/* 			found_target.x, found_target.y, to_degree(atan(found_target.y/found_target.x))); */

/* 	if (!check_wt(test_point.x, found_target.x)) {  */
/* 		printf("x error at angle: %d; test: "_fmt", found: "_fmt"\n",  */
/* 				angle, test_point.x, found_target.x);  */
/* 	} else if (!check_wt(test_point.y, found_target.y)) {  */
/* 		printf("y error at angle: %d; test: "_fmt", found: "_fmt"\n",  */
/* 				angle, test_point.y, found_target.y);  */
/* 	} else { printf("%d degrees ok.\n", angle); } */

	serialdn("Loop ended.");
	delay(9000);
}

