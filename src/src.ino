#define _ARDUINO

#include "calibration.h"
#include "calculator.h"

#include "main.h"


void setup() {
#ifdef _DEBUG
	Serial.begin(115200);
	Serial.println("Started.");
#endif
}


void loop() {
	struct encoder_init_parameters parameters = {_sensor_num_g,
												 _calibration_pin_g,
												 _eeprom_address_g,
												 _sensor_pins_g,
												 _example_meta_g
												};
	struct encoder_init_rv init_rv;

	while(!encoder_init(&init_rv, &parameters)) {
		serialdn("Encoder init failed.");
		wait(3000);
	} serialdn("Encoder init succeeded.");

	while(true) {
		if(!encoder_loop(&init_rv, &parameters)) {
			serialdn("Encoder loop failed.");
		} else { 
			serialdn("Encoder loop succeeded.");
		} wait(6000);
	}
}

