/*
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci

SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "calculator.h"
#include "calibration.h"


struct encoder_init_rv {
	calibration_data_t *calibration_data;
	struct ft_buffer *ft_buffer;
	circle_t *circles;
};

struct encoder_init_parameters {
	int sensor_num, calibration_pin, eeprom_address;
	const int *sensor_pins;
	calibration_meta_t example_meta;
};

void encoder_free(struct encoder_init_rv* rv) {
	free(rv->calibration_data);
	free(rv->ft_buffer);
	free(rv->circles);
}

bool encoder_init(struct encoder_init_rv* rv_buffer, struct encoder_init_parameters* parameters) {
#define pr parameters
#define rv rv_buffer

	set_pin(LED_BUILTIN, OUTPUT);
	set_pin(pr->calibration_pin, INPUT);
	for (int i = 0; i < pr->sensor_num; i++)
		set_pin(pr->sensor_pins[i], INPUT);

	/* calibration.h setup */
#ifdef _ARDUINO
 	bool valid_calib = 0;
 	rv->calibration_data = calibration_read(pr->eeprom_address);
 	if (calibration_check(rv->calibration_data, &pr->example_meta)) 
		valid_calib = 1;


#ifndef _DEBUG
 	if (!valid_calib || read_pin(pr->calibration_pin) == HIGH) {
#else
 	if (!valid_calib) {
#endif
		valid_calib = 0;
 		while (!valid_calib) {
 			valid_calib = calibrate_sensors(rv->calibration_data, pr->sensor_pins);
 			if (valid_calib == 0) serialdn("Calibration failed.");
 		}
 		calibration_write(pr->eeprom_address, rv->calibration_data);
 	} else serialdn("Found valid calibration data in eeprom.");
#else
	rv->calibration_data = (calibration_data_t *)malloc(sizeof(calibration_data_t));
	rv->calibration_data->magnet_projection.center = (point_t){0, 0};
	rv->calibration_data->magnet_projection.radius = 0.1;
#endif

	/* encoder.h setup */
	rv->ft_buffer = (struct ft_buffer *)malloc(sizeof(struct ft_buffer));
	if (rv->ft_buffer == NULL) { serialdn("ft_buffer malloc error"); return false; }
	
	rv->circles = (circle_t *)malloc(pr->sensor_num * sizeof(circle_t));
	if (rv->circles == NULL) { serialdn("circles malloc error"); return false; }

	calc_sensor_positions_bs(&rv->circles[0].center, sizeof(circle_t), pr->sensor_num);
	return true;

#undef pr
#undef rv
}


bool encoder_loop(struct encoder_init_rv* init_rv, struct encoder_init_parameters* parameters) {
#define pr parameters
#define rv init_rv
	/* point_t test_point = {1, 1}; */
	for (int angle = 0; angle < 360; angle+=90) {

		point_t test_point = { 
			rv->calibration_data->magnet_projection.radius * cos(to_radian(angle)),
			rv->calibration_data->magnet_projection.radius * sin(to_radian(angle)) 
		}; 

		calc_test_distances_bs(&rv->circles[0].radius, sizeof(circle_t), 
				test_point, &rv->circles[0].center, sizeof(circle_t), pr->sensor_num);


		/* create noise */
		for (int i=0; i < pr->sensor_num; i++)
			rv->circles[i].radius += _tolerance_g;


		point_t found_target = find_target(rv->ft_buffer, rv->circles, pr->sensor_num, \
				&rv->calibration_data->magnet_projection);
		
#ifdef _ARDUINO
		Serial.print("Test target: ");
		Serial.print(test_point.x);
		Serial.print(" ");
		Serial.println(test_point.y);

		Serial.print("Found target: ");
		Serial.print(found_target.x);
		Serial.print(" ");
		Serial.println(found_target.y);
#else
		serialf("Test target:  (%f, %f)\n", test_point.x, test_point.y);
		serialf("Found target: (%lf, %lf)\n", found_target.x, found_target.y);
#endif

		if (!check_wt(test_point.x, found_target.x)) { return false; }
		if (!check_wt(test_point.y, found_target.y)) { return false; }
	} return true;

#undef pr
#undef rv
}


