/*
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci

SPDX-License-Identifier: GPL-3.0-or-later
*/

/* arduino compiler direkt bu kodu derlemeye çalışıyor o yüzden bu satırı ekledim */
#ifndef __AVR

#include "pico/stdlib.h"
#include "stdio.h"

#define _PICO


#include "main.h"


int main() {
	dbg(stdio_init_all());
	serialdn("Started.");

	struct encoder_init_parameters parameters = {.sensor_num = _sensor_num_g,
												 .calibration_pin = _calibration_pin_g,
												 .eeprom_address = _eeprom_address_g,
												 .sensor_pins = _sensor_pins_g,
												 .example_meta = _example_meta_g
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
	return 0;
}

#endif
