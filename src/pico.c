/* arduino compiler direkt bu kodu derlemeye çalışıyor o yüzden bu satırı ekledim */
#ifndef __AVR

#include "pico/stdlib.h"
#include "stdio.h"

#define _PICO


/* ========================================== */
#include "header.h"

#define _eeprom_address_g 0
#define _calibration_start_g "calibeg"
#define _version_skip_key_g "9999999"

typedef struct calibration_meta_t {
	char calibration_start[8];
	char version[8];
	int sensor_num;
} calibration_meta_t;

typedef struct calibration_data_t {
	calibration_meta_t header;
	circle_t magnet_projection;
	sensor_data_t sensor_datas[_sensor_num_g];
} calibration_data_t;

const calibration_meta_t _example_meta_g = { _calibration_start_g, _version_g, _sensor_num_g };
/* ========================================== */

#include "main.h"


int main() {
	stdio_init_all();
	printf("Started.");

	struct encoder_init_parameters parameters = {.sensor_num = _sensor_num_g,
												 .calibration_pin = _calibration_pin_g,
												 .eeprom_address = _eeprom_address_g,
												 .first_sensor_pin = _first_sensor_pin_g,
												 .example_meta = _example_meta_g
												};
	struct encoder_init_rv init_rv;

	while(!encoder_init(&init_rv, &parameters)) { serialdn("Encoder init failed."); wait(1000); }
	while(true) {
		if(!encoder_loop(&init_rv, &parameters)) {
			serialdn("Encoder loop failed.");
		}
	}
	return 0;
}

#endif
