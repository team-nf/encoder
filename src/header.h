#ifndef _HEADER_H_INCLUDED
#define _HEADER_H_INCLUDED

#include "config.h"

#include <stdlib.h>
#include <string.h>



/* typedef enum bool { false=0, true=1 }; */

#ifdef _type_float
typedef float ftype;
#define _fmt "%f"
#endif

#ifdef _type_double
typedef double ftype;
#define _fmt "%f"
#endif

#ifdef _type_long_double
typedef long double ftype;
#define _fmt "%Lf"
#endif



typedef struct sensor_data_t {
	int _min, _normal, _max;
} sensor_data_t;

typedef struct point_t {
	ftype x, y;
} point_t;

typedef struct line_t {
	ftype m, n;
} line_t;

typedef struct circle_t {
	point_t center;
	ftype radius;
} circle_t;



#define blink(time, repetition) ({ \
	for (int i=0; i < repetition+2; i++) { \
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); \
		delay(time); \
	} \
})


/* release için derleme hızı artsın diye */
#ifdef _DEBUG
#define serialf(...) ({ \
	int _size = snprintf(NULL, 0, __VA_ARGS__) + 1; \
	char* _str = (char *)malloc(_size * sizeof(char)); \
	snprintf(_str, _size, __VA_ARGS__); \
	Serial.print(_str); \
	free(_str); \
})

#define serialfn(...) ({ \
	int _size = snprintf(NULL, 0, __VA_ARGS__) + 1; \
	char* _str = (char *)malloc(_size * sizeof(char)); \
	snprintf(_str, _size, __VA_ARGS__); \
	Serial.println(_str); \
	free(_str); \
})

#define serialdn(...) Serial.print(__VA_ARGS__)
#define seriald(...)  Serial.print(__VA_ARGS__)

#define dbg(a) a
#endif



#ifndef _DEBUG
#define serialdn(...)
#define seriald(...)

#define serialfn(...)
#define serialf(...)
#define dbg(a)
#endif
#endif


/* config file include */
/* #ifndef _CONFIG_H_INCLUDED */
/* #define _CONFIG_H_INCLUDED */
/* #include "config.h" */
/* #endif */
