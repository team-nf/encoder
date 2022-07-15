#include <EEPROM.h>

#include "encoder.h"
#define CALIB_IMPL
#include "calibration.h"


calibration_data_t *_calibration_data_g;

void calibration_print(const calibration_data_t* data) {
	Serial.print("Start: ");	
	Serial.println(data->header.calibration_start);
	Serial.print("Version: ");	
	Serial.println(data->header.version);
	Serial.print("Encoder Count: ");
	Serial.println(data->header.sensor_num);
	for (int i=0; i < data->header.sensor_num; i++) {
		Serial.print("\tSensor ");
		Serial.print(i);
		Serial.print(": ");
		Serial.print(data->sensor_datas[i]._min);
		Serial.print(" ");
		Serial.print(data->sensor_datas[i]._normal);
		Serial.print(" ");
		Serial.println(data->sensor_datas[i]._max);
	}
}

void setup() {
	Serial.begin(115200);
	Serial.println("\nStarted.");

	_calibration_data_g = (calibration_data_t *)malloc(_sensor_num_g * sizeof(calibration_data_t));
	_calibration_data_g->header = _example_meta_g;
	for (int i=0; i < 3; i++)
		_calibration_data_g->sensor_datas[i] = {i, i+1, i+2};
	Serial.println("Configuration data: ");
	calibration_print(_calibration_data_g);
	
	//_eeprom_write(0, (byte *)_calibration_data_g, sizeof(calibration_data_t));
	calibration_write(0, _calibration_data_g);
	Serial.println("Data written to eeprom (i guess).");
}

void loop() {
	Serial.println("\nLoop begin");
	//calibration_data_t *read_data = (calibration_data_t *)_eeprom_read(0, sizeof(calibration_data_t));
	
	//calibration_data_t *read_data = (calibration_data_t *)malloc(sizeof(calibration_data_t));
	//calibration_read(read_data->sensor_datas);

	calibration_data_t *read_data = calibration_read(0);

	calibration_print(read_data);
	Serial.print("Check Exists: ");
	Serial.println(calibration_check_exists(&read_data->header, &_calibration_data_g->header));
	Serial.print("Check Sensor Data: ");
	Serial.println(calibration_check_sensor_data(read_data->sensor_datas, _calibration_data_g->header.sensor_num));

	Serial.print("Check: ");
	Serial.println(_calibration_check(read_data, &_calibration_data_g->header));
	Serial.println("End.");

	free(read_data);
	delay(12000);
}

