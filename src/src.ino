#define _ARDUINO
#include <EEPROM.h>

/* calibration.h dosyası headerı includelıyor */
#define CALIB_IMPL
#include "calibration.h"

/* cinter.h, encoder.h dosyasında includelanıyor */
#define CINTER_IMPL
/* #include "cinter.h" */

#define ENCODER_IMPL
#include "encoder.h"


#include "main.h"


void setup() {
	while(!encoder_init());
}


void loop() {
	encoder_loop();
}

