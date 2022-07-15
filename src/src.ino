#include <EEPROM.h>
#include <stdarg.h>

#include "encoder.h"
#define CALIB_IMPL
#include "calibration.h"

calibration_data_t *_calibration_data_g;

#define blink(time, repetition) ({ \
	for (int i=0; i < repetition+2; i++) { \
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); \
		delay(time); \
	} \
})

bool calib_test(calibration_data_t* _data, int first_sensor) {
#ifdef _DEBUG
	if (!calibration_check_exists(&_data->header, &_example_meta_g)) { return false; }
#endif
	int *read_max = (int *)malloc(_data->header.sensor_num * sizeof(int));
	int *read_min = (int *)malloc(_data->header.sensor_num * sizeof(int));
	int *read_normal = (int *)malloc(_data->header.sensor_num * sizeof(int));
	blink(100, 30);

	for (int i=0; i < _data->header.sensor_num; i++) {
		read_normal[i] = analogRead(first_sensor+i);
		read_max[i] = read_normal[i];
		read_min[i] = read_normal[i];
	}

	int start_time = millis();
	while (millis() - start_time <= 6000) {
		serialf("Passed time: %d\n", millis() - start_time);
		delay(1000);
	}

	blink(100, 30);
	free(read_normal);
	free(read_max);
	free(read_min);
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

	calibration_write(0, _calibration_data_g);

#ifdef _DEBUG
	Serial.println("Data written to eeprom.");
#endif
}

void loop() {
#ifdef _DEBUG
	Serial.println("\nLoop begin");
#endif

	calibration_data_t *read_data = calibration_read(0);
	calib_test(_calibration_data_g, _first_sensor_pin_g);



#ifdef _DEBUG
	Serial.println("End.");
#endif
	free(read_data);
	delay(9000);
}

