#include <EEPROM.h>

/* calibration.h dosyası zaten headerı includelıyor */
// #include "header.h"
#define CALIB_IMPL
#include "calibration.h"

/* 3 pinden 1 saniyede 2976 okuma (arduino uno)*/

calibration_data_t *_calibration_data_g;


void setup() {
#ifdef _DEBUG
	Serial.begin(115200);
	Serial.println("\nStarted.");
#endif
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(_calibration_pin_g, INPUT);

	bool valid_calib = 0;
	_calibration_data_g = calibration_read(_eeprom_address_g);
	if (calibration_check(_calibration_data_g, &_example_meta_g)) valid_calib = 1;

	if (!valid_calib || digitalRead(_calibration_pin_g) == HIGH) {
		while (!valid_calib)
			valid_calib = calibrate_sensors(_calibration_data_g, _first_sensor_pin_g, _mode_g);
		calibration_write(_eeprom_address_g, _calibration_data_g);
	}
#ifdef _DEBUG
	else Serial.println("Found valid calibration data in eeprom.");
#endif

}


void loop() {
	Serial.println("Loop started.");

	bool rv = calibrate_sensors(_calibration_data_g, _first_sensor_pin_g, _mode_g);
	serialf("Return Value: %d\n", rv);

	Serial.println("Loop ended.");
	delay(9000);
}

