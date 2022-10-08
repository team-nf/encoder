/*
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci

SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef __AVR

/* sleep() fonksiyonu için */
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#define _PC


/* ========================================== */

/* Keyboard interruptı yakalamak için */
#include <signal.h>
volatile sig_atomic_t _flag_g = 0;
void interrupt_flag_changer(int sig){
	_flag_g = 1;
}

/* ========================================== */

#include "main.h"


int main() {
	serialdn("Started.");
	signal(SIGINT, interrupt_flag_changer); 
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

/* 	while(true) { */
/* 		if(!encoder_loop(&init_rv, &parameters)) { */
/* 			serialdn("Encoder loop failed."); */
/* 		} else {  */
/* 			serialdn("Encoder loop succeeded."); */
/* 		}  */
/*  */
		/* Keyboard intterupt gelirse */
/* 		if (_flag_g) { */
/* 			encoder_free(&init_rv);  */
/* 			break; */
/* 		} */
/* 		wait(6000); */
/* 	} */

	if(!encoder_loop(&init_rv, &parameters)) {
		serialdn("Encoder loop failed.");
	} else { 
		serialdn("Encoder loop succeeded.");
	} 

	return 0;
}

#endif
