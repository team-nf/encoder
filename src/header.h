/*
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci

SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef _HEADER_H_INCLUDED
#define _HEADER_H_INCLUDED

#include "config.h"

#include <stdlib.h>
#include <string.h>



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


/* == MCU Specific == */
/* millis ve analogRead için pico versiyonları bulunmalı */
#ifdef _ARDUINO
#define start_serial_connection() Serial.begin(115200)
#define wait(ms) delay(ms)
#define set_pin(pin, mode) pinMode(pin, mode)
#define read_pin(pin) digitalRead(pin)
#define write_pin(pin, state) digitalWrite(pin, state)
#endif


#ifdef _PICO
#define start_serial_connection() stdio_init_all()
#define wait(ms) sleep_ms(ms)
#define set_pin(pin, mode) ({ gpio_init(pin); gpio_set_dir(pin, mode); })
#define read_pin(pin) gpio_get(pin)
#define write_pin(pin, state) gpio_put(pin, state)
/* BURAYA BAK */
#define LED_BUILTIN PICO_DEFAULT_LED_PIN
#define OUTPUT GPIO_OUT
#define INPUT GPIO_IN
#endif


#ifdef _PC
typedef enum { false=0, true=1 } bool;
typedef char byte;
#define start_serial_connection() 
#define wait(ms) sleep(ms/1000);

#define set_pin(...) printf("set_pin("#__VA_ARGS__");\n")
#define read_pin(...) printf("read_pin("#__VA_ARGS__");\n")
#define write_pin(...) printf("write_pin("#__VA_ARGS__");\n")

#define malloc(size) ({ printf("malloc("#size"); -> %lu bytes.\n", size); malloc(size); })
#define free(size)	 ({ printf("free("#size");\n"); free(size); })

#define LED_BUILTIN 13
#define OUTPUT 0
#define INPUT 1
#define HIGH 1
#define LOW 0
#endif



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
#define serialfn(...) ({ printf(__VA_ARGS__); printf("\n"); })
#define seriald(msg)  printf(msg)
#define serialdn(msg) ({ printf(msg); printf("\n"); })
#endif

#ifdef _PC
#define serialf(...) printf(__VA_ARGS__)
#define serialfn(...) ({ printf(__VA_ARGS__); printf("\n"); })
#define seriald(msg)  printf(msg)
#define serialdn(msg) ({ printf(msg); printf("\n"); })
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

