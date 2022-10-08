/*
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci

SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef _CONFIG_H_INCLUDED
#define _CONFIG_H_INCLUDED



/* == General == */
#define _sensor_num_g 3
#define _DEBUG
#define _ONEFILE

const int _sensor_pins_g[_sensor_num_g] = {0, 1, 2};


#ifdef _ONEFILE
#define CALC_IMPL
#define ANALYTICS_IMPL
#define CALIB_IMPL
#endif


/* == Calibration == */
#define _calibration_length_g 12
#define _calibration_pin_g 2
#define _version_g "0000000"
#define _NOSENSOR



/* == Circle Intersection == */
/* #define _tolerance_g 0.000000000000001 */
#define _tolerance_g 0.0001
#define _skip_perfect_check

/* #define _type_float */
#define _type_double
/* #define _type_long_double */


#endif
