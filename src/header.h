#ifndef _HEADER_H_INCLUDED
#define _HEADER_H_INCLUDED

#include "config.h"

#include <stdlib.h>
#include <string.h>



/* typedef enum { false=0, true=1 } bool; */

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
		write_pin(LED_BUILTIN, !read_pin(LED_BUILTIN)); \
		wait(time); \
	} \
})


/* release için derleme hızı artsın diye */
#ifdef _DEBUG
#define dbg(a) a

#ifdef _ARDUINO
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
#define seriald(msg)  Serial.print(msg)
#define serialdn(msg) Serial.println(msg)
#endif

#ifdef _PICO
#define serialf(...) printf(__VA_ARGS__)
#define serialfn(...) printf(__VA_ARGS__); printf("\n")
#define seriald(msg)  printf(msg)
#define serialdn(msg) printf(msg); printf("\n")
#endif

#endif



#ifndef _DEBUG
#define serialf(...)
#define serialfn(...)

#define seriald(msg)
#define serialdn(msg)

#define dbg(a)
#endif
#endif

