#include <EEPROM.h>

#include "encoder.h"
#define CALIB_IMPL
#include "calibration.h"

/* 3 pinden 1 saniyede 2976 okuma (arduino uno)*/

calibration_data_t *_calibration_data_g;


bool calib_test(calibration_data_t* _data, int first_sensor) {
#ifdef _DEBUG
	if (!calibration_check_exists(&_data->header, &_example_meta_g)) { return false; }
#endif
	int *read_min	 = (int *)malloc(_data->header.sensor_num * sizeof(int));
	int *read_max	 = (int *)malloc(_data->header.sensor_num * sizeof(int));
	int *read_normal = (int *)malloc(_data->header.sensor_num * sizeof(int));
	blink(100, 10);

	for (int i=0; i < _data->header.sensor_num; i++) {
		read_normal[i] = analogRead(first_sensor+i);
		read_min[i] = read_normal[i];
		read_max[i] = read_normal[i];
	}

	int counter = 0;
	int start_time = millis();
	while (millis() - start_time <= 6000) {
		for (int i=0; i < _data->header.sensor_num; i++) {
			int read_value = analogRead(first_sensor+i);
			if (read_value < read_min[i])	read_min[i] = read_value;
			else if (read_value > read_max[i])		read_max[i] = read_value;
		} counter++;
	} 
	blink(100, 10);

	/* yeni malloc yerine direkt _data->sensor_datas da kullanılabilirdi? *
	 * ama datanın geçerli olup olmadığını görmeden yazmak mantıklı değil */
	for (int i=0; i < _data->header.sensor_num; i++) {
		//_data->sensor_datas[i]._normal = read_normal[i];
		_data->sensor_datas[i]._normal = (read_max[i] - read_min[i]) / 2;
		_data->sensor_datas[i]._min = read_min[i];
		_data->sensor_datas[i]._max = read_max[i];
	}

	serialf("Read %d sensors %d times in 6 seconds.\n", _data->header.sensor_num, counter);
	for (int i=0; i < _data->header.sensor_num; i++) {
		serialf("Sensor %d:\n\tmin: %d\n\tnormal: %d\n\tmax: %d\n", i, read_min[i], read_normal[i], read_max[i]);
	}

	free(read_normal);
	free(read_max);
	free(read_min);

	return calibration_check_sensor_data(_data->sensor_datas, _data->header.sensor_num);
}

void setup() {
#ifdef _DEBUG
	Serial.begin(115200);
	Serial.println("\nStarted.");
#endif
	
	pinMode(LED_BUILTIN, OUTPUT);
	_calibration_data_g = (calibration_data_t *)malloc(_sensor_num_g * sizeof(calibration_data_t));
	_calibration_data_g->header = _example_meta_g;
	for (int i=0; i < 3; i++)
		_calibration_data_g->sensor_datas[i] = {i, i+1, i+2};

	//calibration_write(0, _calibration_data_g);
	//Serial.println("Data written to eeprom.");
}

void loop() {
	Serial.println("Loop begin");
	//calibration_data_t *read_data = calibration_read(0);
	serialf("Return Value: %d\n", calib_test(_calibration_data_g, _first_sensor_pin_g));



	Serial.println("End.");
	//free(read_data);
	delay(9000);
}

