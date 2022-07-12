/* tunapro1234 */
#define ENCODER_IMPL
#include "encoder.h" 
#define CALIB_IMPL
#include "calibration.h"

/* datanın tutulacağı yer */
const sensor_data_t *_calibration_data_g;


void setup() {
	Serial.begin(115200);
	Serial.println("Started.");

	pinMode(_calibration_pin_g, INPUT);
	_calibration_data_g = (sensor_data_t *)malloc(_sensor_num_g * sizeof(sensor_data_t));
	
	sensor_data_t sensor_datas[3] = { {0,1,2}, {3,4,5}, {5,6,7} };

	/* for (int i=0; i < 3; i++) {
		sensor_datas[i]._min	= 1;
		sensor_datas[i]._normal = 2;
		sensor_datas[i]._max	= 3;
	} */
	//memcpy(_calibration_data_g, sensor_datas, 3 * sizeof(sensot_data_t));

	/* bu olaya el at */
	calibration_data_t temp_data = { _example_meta_g, sensor_datas[0], sensor_datas[1], sensor_datas[2] };
	calibration_write(&temp_data);
}

void loop() {
	if (calibration_check_eeprom()) {
		calibration_read(_calibration_data_g);
		for (int i=0; i < 3; i++) {
			Serial.print(_calibration_data_g[i]._min);
			Serial.print(_calibration_data_g[i]._normal);
			Serial.print(_calibration_data_g[i]._max);
			Serial.print(" ");
		} Serial.print("\n");
	} else {
		Serial.println("Calibration check failed.");
	}
	
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
}


