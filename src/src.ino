#define _ARDUINO

#include "calibration.h"
#include "calculator.h"

#include "main.h"


void setup() {
	Serial.begin(115200);
	Serial.println("Started.");
}


void loop() {
	struct encoder_init_parameters parameters = {_sensor_num_g,
												 _calibration_pin_g,
												 _eeprom_address_g,
												 _first_sensor_pin_g,
												 _example_meta_g
												};
	struct encoder_init_rv init_rv;

	while(!encoder_init(&init_rv, &parameters)) { Serial.println("Encoder init failed."); delay(1000); }
	while(true) {
		if(!encoder_loop(&init_rv, &parameters)) {
			Serial.println("Encoder loop failed.");
		}
	}
}

