calibration_data_t *_calibration_data_g;
struct ft_buffer *_ft_buffer_g;

point_t *_positions_g;
ftype *_distances_g;

int encoder_init() {
	dbg(start_serial_connection());
	serialdn("\nStarted.");
	set_pin(LED_BUILTIN, OUTPUT);
	set_pin(_calibration_pin_g, INPUT);

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
	if (_positions_g == NULL) { serialdn("calc_sensor_positions error"); return 0; }

	_ft_buffer_g = (struct ft_buffer *)malloc(sizeof(struct ft_buffer));
	if (_ft_buffer_g == NULL) { serialdn("_ft_buffer malloc error"); return 0; }
	
	_distances_g = (ftype *)malloc(_sensor_num_g * sizeof(ftype));
	if (_distances_g == NULL) { serialdn("_distances_g malloc error"); return 0; }
}


int encoder_loop() {
	serialdn("Loop started.");

	/* point_t test_point = {1, 1}; */
	int angle = 30;
	point_t test_point = { 
		_calibration_data_g->magnet_projection.radius * cos(to_radian(angle)),
		_calibration_data_g->magnet_projection.radius * sin(to_radian(angle)) 
	}; calc_test_distances_b(_distances_g, test_point, _positions_g, _sensor_num_g);

	/* create noise */
/* 	for (int i=0; i < _sensor_num_g; i++) */
/* 		_distances_g[i] = _distances_g[i] + 0.0001; */

	
	/* initialize circles */
	for (int i = 0; i < _sensor_num_g; i++) {
		_ft_buffer_g->circles[i].center = _positions_g[i];
		_ft_buffer_g->circles[i].radius = _distances_g[i];
	}

	point_t found_target = find_target(_ft_buffer_g, _sensor_num_g, &_calibration_data_g->magnet_projection);
	serialf("Test target:  (%f, %f)\n", test_point.x, test_point.y);
	serialf("Found target: (%f, %f)\n", found_target.x, found_target.y);

	serialdn("Loop ended.");
	wait(6000);
}

