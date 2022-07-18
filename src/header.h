#ifndef _HEADER_H_INCLUDED
#define _HEADER_H_INCLUDED

/* typedef enum bool { false=0, true=1 }; */

enum encoder_mode_g {
	em_analog,
	em_digital,
	em_analog_digital,
	em_count
};


typedef struct sensor_data_t {
	int _min, _normal, _max;
} sensor_data_t;

#include "config.h"

/* release için derleme hızı artsın diye */
#ifdef _DEBUG
#define serialf(...) ({ \
	int _size = snprintf(NULL, 0, __VA_ARGS__) + 1; \
	char* _str = (char *)malloc(_size * sizeof(char)); \
	snprintf(_str, _size, __VA_ARGS__); \
	Serial.print(_str); \
	free(_str); \
})
#endif


#define blink(time, repetition) ({ \
	for (int i=0; i < repetition+2; i++) { \
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); \
		delay(time); \
	} \
})

#endif


/* config file include */
/* #ifndef _CONFIG_H_INCLUDED */
/* #define _CONFIG_H_INCLUDED */
/* #include "config.h" */
/* #endif */
