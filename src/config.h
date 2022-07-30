#ifndef _CONFIG_H_INCLUDED
#define _CONFIG_H_INCLUDED



/* == General == */
const int _first_sensor_pin_g	= A0;
#define _sensor_num_g 3
#define _DEBUG



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
