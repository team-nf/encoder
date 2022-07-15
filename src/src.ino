#include <EEPROM.h>
#include <stdarg.h>

#include "encoder.h"
#define CALIB_IMPL
#include "calibration.h"

calibration_data_t *_calibration_data_g;

void setup() {
#ifdef _DEBUG
	Serial.begin(115200);
	Serial.println("\nStarted.");
#endif

	_calibration_data_g = (calibration_data_t *)malloc(_sensor_num_g * sizeof(calibration_data_t));
	_calibration_data_g->header = _example_meta_g;
	for (int i=0; i < 3; i++)
		_calibration_data_g->sensor_datas[i] = {i, i+1, i+2};

	calibration_write(0, _calibration_data_g);

#ifdef _DEBUG
	Serial.println("Configuration data: ");
	calibration_print(_calibration_data_g);

	Serial.println("Data written to eeprom.");
#endif
}

void loop() {
#ifdef _DEBUG
	Serial.println("\nLoop begin");
#endif
	calibration_data_t *read_data = calibration_read(0);
#ifdef _DEBUG
	calibration_print(read_data);
	Serial.print("Check Exists: ");
	Serial.println(calibration_check_exists(&read_data->header, &_calibration_data_g->header));
	Serial.print("Check Sensor Data: ");
	Serial.println(calibration_check_sensor_data(read_data->sensor_datas, _calibration_data_g->header.sensor_num));

	Serial.print("Check: ");
	Serial.println(calibration_check(read_data, &_calibration_data_g->header));
	Serial.println("End.");
#endif

	free(read_data);
	delay(12000);
}

