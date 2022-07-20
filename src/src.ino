#include <EEPROM.h>

/* calibration.h dosyası zaten headerı includelıyor */
/* #include "header.h" */
#define CALIB_IMPL
#include "calibration.h"

/* 3 pinden 1 saniyede 2976 okuma (arduino uno)*/

calibration_data_t *_calibration_data_g;


/* encoder readini olabildiğince hızlandırmak için macro yapılabilir */
int encoder_read_m(sensor_data_t* sensor_datas, int sensor_num, int first_sensor_pin, int tolerance) {
	int *read_values = (int *)malloc(sensor_num * sizeof(bool));
	/* okumalar birbirine olabildiğince yakın olsun diye iki ayrı loop var */
	for (int i=0; i < sensor_num; i++)
		read_values[i] = analogRead(first_sensor_pin+i);

	int rv = -1;
	for (int i=0; i < sensor_num; i++) {
		if (sensor_datas[i]._max	 - tolerance < read_values[i] \
			&& read_values[i] < sensor_datas[i]._max	  + tolerance) rv = i;
	} free(read_values);
	return rv;
}

void setup() {
	dbg(Serial.begin(115200));
	serialdn("\nStarted.");
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(_calibration_pin_g, INPUT);

	bool valid_calib = 0;
	_calibration_data_g = calibration_read(_eeprom_address_g);
	if (calibration_check(_calibration_data_g, &_example_meta_g)) valid_calib = 1;

	if (!valid_calib || digitalRead(_calibration_pin_g) == HIGH) {
		while (!valid_calib) {
			valid_calib = calibrate_sensors(_calibration_data_g, _first_sensor_pin_g, _mode_g);
			dbg(if (valid_calib == 0) Serial.println("Calibration failed"));
		}
		calibration_write(_eeprom_address_g, _calibration_data_g);
	}
	dbg(else Serial.println("Found valid calibration data in eeprom."));

}


void loop() {
	serialdn("Loop started.");
	int active_sensor;

	while (1) {
		active_sensor = encoder_read_m(_calibration_data_g->sensor_datas, _sensor_num_g, _first_sensor_pin_g, 100);
		if (active_sensor >= 0) serialfn("degree: %d", 360/_sensor_num_g * active_sensor);
		else serialdn("No active sensor found.");
	}

	serialdn("Loop ended.");
	delay(9000);
}

