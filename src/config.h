#ifndef _CONFIG_H_INCLUDED
#define _CONFIG_H_INCLUDED



/* == General == */
#ifdef _ARDUINO
const int _first_sensor_pin_g = A0;
#else
const int _first_sensor_pin_g = 0;
#endif

#define _sensor_num_g 3
#define _DEBUG
#define _ONEFILE


#ifdef _ONEFILE
#define CALC_IMPL
#define ANALYTICS_IMPL
#define CALIB_IMPL
#endif

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
#define start_serial_connection() 
#define wait(ms) sleep_ms(ms)
#define set_pin(pin, mode) gpio_init(pin); gpio_set_dir(pin, mode)
#define read_pin(pin) gpio_get(pin)
#define write_pin(pin, state) gpio_put(pin, state)
/* BURAYA BAK */
#define OUTPUT GPIO_OUT
#define INPUT GPIO_IN
#define LED_BUILTIN PICO_DEFAULT_LED_PIN
#endif



/* == Calibration == */
#define _calibration_length_g 12
#define _calibration_pin_g 2
#define _version_g "0000000"
#define _NOSENSOR



/* == Circle Intersection == */
#define _tolerance_g 0.0001
#define _skip_perfect_check

/* #define _type_float */
#define _type_double
/* #define _type_long_double */



#endif
