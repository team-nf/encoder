#include "calculator.h"

#ifndef _PICO
/* picoda eeprom olmadığından ufak bir problem */
#include "calibration.h"
#endif


struct encoder_init_rv {
	calibration_data_t *calibration_data;
	struct ft_buffer *ft_buffer;

	point_t *positions;
	ftype *distances;
};

struct encoder_init_parameters {
	int sensor_num, calibration_pin, eeprom_address, first_sensor_pin;
	calibration_meta_t example_meta;
};

void encoder_free(struct encoder_init_rv* rv) {
	free(rv->calibration_data);
	free(rv->ft_buffer);
	free(rv->positions);
	free(rv->distances);
}

bool encoder_init(struct encoder_init_rv* rv_buffer, struct encoder_init_parameters* parameters) {
#define pr parameters
#define rv rv_buffer

	serialdn("\nStarted.");
	set_pin(LED_BUILTIN, OUTPUT);
	set_pin(pr->calibration_pin, INPUT);

	/* calibration.h setup */
#ifdef _ARDUINO
 	bool valid_calib = 1;
 	rv->calibration_data = calibration_read(pr->eeprom_address);
 	if (calibration_check(rv->calibration_data, &pr->example_meta)) 
		valid_calib = 1;

 	if (!valid_calib || read_pin(pr->calibration_pin) == HIGH) {
 		while (!valid_calib) {
 			valid_calib = calibrate_sensors(rv->calibration_data, pr->first_sensor_pin);
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
	rv->positions = calc_sensor_positions(pr->sensor_num);
	if (rv->positions == NULL) { serialdn("calc_sensor_positions error"); return false; }

	rv->ft_buffer = (struct ft_buffer *)malloc(sizeof(struct ft_buffer));
	if (rv->ft_buffer == NULL) { serialdn("ft_buffer malloc error"); return false; }
	
	rv->distances = (ftype *)malloc(pr->sensor_num * sizeof(ftype));
	if (rv->distances == NULL) { serialdn("distances malloc error"); return false; }
	return true;

#undef pr
#undef rv
}


bool encoder_loop(struct encoder_init_rv* init_rv, struct encoder_init_parameters* parameters) {
#define pr parameters
#define rv init_rv
	/* point_t test_point = {1, 1}; */
	for (int angle = 0; angle < 360; angle++) {
		point_t test_point = { 
			rv->calibration_data->magnet_projection.radius * cos(to_radian(angle)),
			rv->calibration_data->magnet_projection.radius * sin(to_radian(angle)) 
		}; calc_test_distances_b(rv->distances, test_point, rv->positions, pr->sensor_num);

		/* create noise */
/* 		for (int i=0; i < pr->sensor_num; i++) */
/* 			pr->distances[i] = pr->distances[i] + 0.0001; */

		
		/* initialize circles */
		for (int i = 0; i < pr->sensor_num; i++) {
			rv->ft_buffer->circles[i].center = rv->positions[i];
			rv->ft_buffer->circles[i].radius = rv->distances[i];
		}

		point_t found_target = find_target(rv->ft_buffer, pr->sensor_num, \
				&rv->calibration_data->magnet_projection);
/* 		serialf("Test target:  (%f, %f)\n", test_point.x, test_point.y); */
/* 		serialf("Found target: (%f, %f)\n", found_target.x, found_target.y); */

		if (!check_wt(test_point.x, found_target.x)) { return false; }
		if (!check_wt(test_point.y, found_target.y)) { return false; }
	} return true;

#undef pr
#undef rv
}


