/*
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci

SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "pico/stdlib.h"
#include <stdio.h>


/* math.h */
/* stdlib.h */
/* string.h */


#define CINTER_IMPL
#include "cinter.h"

#define ENCODER_IMPL
#include "encoder.h"



/* ===(CALIBRATION FILE START)=== */
#define _eeprom_address_g 0
#define _calibration_start_g "calibeg"
#define _version_skip_key_g "9999999"


typedef struct calibration_meta_t {
	char calibration_start[8];
	char version[8];
	int sensor_num;
} calibration_meta_t;

typedef struct calibration_data_t {
	calibration_meta_t header;
	circle_t magnet_projection;
	sensor_data_t sensor_datas[_sensor_num_g];
} calibration_data_t;

const calibration_meta_t _example_meta_g = { _calibration_start_g, _version_g, _sensor_num_g };
/* ===(CALIBRATION FILE END)=== */



calibration_data_t *_calibration_data_g;
struct ft_buffer *_ft_buffer_g;

point_t *_positions_g;
ftype *_distances_g;


int main() {
	stdio_usb_init();
	printf("Started.\n");

    const uint led_pin = PICO_DEFAULT_LED_PIN;
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

	_calibration_data_g = (calibration_data_t *)malloc(sizeof(calibration_data_t));
	_calibration_data_g->magnet_projection.center = (point_t){0, 0};
	_calibration_data_g->magnet_projection.radius = 0.1;

	/* encoder.h setup */
	_positions_g = calc_sensor_positions(_sensor_num_g);
	if (_positions_g == NULL) { return 1; }

	_ft_buffer_g = (struct ft_buffer *)malloc(sizeof(struct ft_buffer));
	if (_ft_buffer_g == NULL) { return 1; }
	
	_distances_g = (ftype *)malloc(_sensor_num_g * sizeof(ftype));
	if (_distances_g == NULL) { return 1; }

	/* point_t test_point = {1, 1}; */
	int angle = 30;
	point_t test_point = { 
		_calibration_data_g->magnet_projection.radius * cos(to_radian(angle)),
		_calibration_data_g->magnet_projection.radius * sin(to_radian(angle)) 
	};
	calc_test_distances_b(_distances_g, test_point, _positions_g, _sensor_num_g);

	/* create noise */
/* 	for (int i=0; i < _sensor_num_g; i++) */
/* 		_distances_g[i] = _distances_g[i] + 0.0001; */

	
	/* initialize circles */
	for (int i = 0; i < _sensor_num_g; i++) {
		_ft_buffer_g->circles[i].center = _positions_g[i];
		_ft_buffer_g->circles[i].radius = _distances_g[i];
	}
	while (1) {
		point_t found_target = find_target(_ft_buffer_g, _sensor_num_g, &_calibration_data_g->magnet_projection);
		printf("Test target:  (%f, %f)\n", test_point.x, test_point.y);
		printf("Found target: (%f, %f)\n", found_target.x, found_target.y);
        sleep_ms(2000);
	}
}

