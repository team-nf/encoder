#ifndef _CINTER_INCLUDED
#define _CINTER_INCLUDED

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "config.h"


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

typedef enum {false=0, true=1} bool;

#define _tolerance_g 0.0001
/* b iki kere çağırılacak :-/ */
#define check_wt(a, b) (b - _tolerance_g < a && a < b + _tolerance_g)
/* neyse */
#define check_wts(a, b) {(\
		typeof(a) _a = a; typeof(b) _b = b; \
		b - _tolerance_g < a && a < b + _tolerance_g \
	)}


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


ftype point_distanceto_point(const point_t* self, const point_t* other);
ftype point_distanceto_line(const point_t* self, const line_t* line);
ftype point_distanceto_circle(const point_t* self, const circle_t* circle);
bool point_ison_circle(const point_t* self, const circle_t* circle);
bool point_ison_line(const point_t* self, const line_t* line);

point_t line_calc_pointfx(const line_t* self, ftype x);
point_t line_calc_pointfy(const line_t* self, ftype y);
point_t line_intersect_line(const line_t* self, const line_t* other);
ftype line_distanceto_circle(const line_t* line, const circle_t* circle);

point_t* circle_intersect_circle(const circle_t* self, const circle_t* line);
point_t* circle_intersect_line(const circle_t* self, const line_t* line);
ftype circle_distanceto_circle(const circle_t* self, const circle_t* other);

ftype* find_roots(ftype a, ftype b, ftype c);


#ifdef CINTER_IMPL
#include "impl/cinter-impl.h"
#endif

#endif
