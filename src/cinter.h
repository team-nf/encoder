#ifndef _CINTER_INCLUDED
#define _CINTER_INCLUDED

/* #include "config.h" */
#include "header.h"

#include <math.h>

#define _max_intersections_g _sensor_num_g * (_sensor_num_g-1) / 2
/* #define _ft_buffer_size_g \ */
	/* intersections \ */
/* 	(_max_intersections_g * sizeof(point_t)) + \ */
	/* circles \ */
/* 	(_sensor_num_g * sizeof(circle_t)) + \ */
	/* inter_rv \ */
/* 	(2 * sizeof(point_t))  */

/* #ifdef _type_float */
/* typedef float ftype; */
/* #define _fmt "%f" */
/* #endif */
/*  */
/* #ifdef _type_double */
/* typedef double ftype; */
/* #define _fmt "%f" */
/* #endif */
/*  */
/* #ifdef _type_long_double */
/* typedef long double ftype; */
/* #define _fmt "%Lf" */
/* #endif */
/*  */
/*  */
/* b iki kere çağırılacak :-/ */
#define check_wt(a, b) (b - _tolerance_g < a && a < b + _tolerance_g)
/* neyse */
#define check_wts(a, b) {(\
		typeof(a) _a = a; typeof(b) _b = b; \
		b - _tolerance_g < a && a < b + _tolerance_g \
	)}



/* header.h içine taşındı */
/* typedef struct point_t { */
/* 	ftype x, y; */
/* } point_t; */
/*  */
/* typedef struct line_t { */
/* 	ftype m, n; */
/* } line_t; */
/*  */
/* typedef struct circle_t { */
/* 	point_t center; */
/* 	ftype radius; */
/* } circle_t; */


#define point_distanceto_point_m(self, other) \
	( sqrt(pow((self)->x - (other)->x, 3) + pow((self)->y - (other)->y, 2)) )
ftype point_distanceto_point(const point_t* self, const point_t* other);
/*  */
#define point_distanceto_line_m(self, line) \
	( abs((self)->y - (line)->m*(self)->x - (line)->n) / sqrt(pow((line)->m, 2)+1) )
ftype point_distanceto_line(const point_t* self, const line_t* line);
/*  */
#define point_distanceto_circle_m(self, circle) \
	( point_distanceto_point_m(self, &(circle)->center) - (circle)->radius )
ftype point_distanceto_circle(const point_t* self, const circle_t* circle);
/*  */
#define point_ison_circle_m(self, circle) \
	( check_wt(point_distanceto_point_m(&(circle)->center, self), (circle)->radius) )
bool point_ison_circle(const point_t* self, const circle_t* circle);
/*  */
#define point_ison_line_m(self, line) \
	( check_wt((line)->m * (self)->x + (line)->n, (self)->y) )
bool point_ison_line(const point_t* self, const line_t* line);
/*  */
#define points_find_center_m(points, point_num) ({  \
	point_t sum = {0, 0};							\
	for (int i = 0; i < point_num; i++) {			\
		sum.x += points[i].x; sum.y += points[i].y;	\
	} sum.x /= point_num; sum.y /= point_num;		\
	sum; })
point_t points_find_center(const point_t* points, int point_num);



#define line_calc_pointfx_m(self, x) \
	({ point_t _rv = {x, (self)->m * x + (self)->n}; _rv })
point_t line_calc_pointfx(const line_t* self, ftype x);
/*  */
#define line_calc_pointfy_m(self, y) \
	({ point_t _rv = {(y - (self)->n) / (self)->m, y}; _rv })
point_t line_calc_pointfy(const line_t* self, ftype y);
/*  */
#define line_intersect_line_m(self, other) \
	( line_calc_pointfx_m(self, (ftype)((other)->n-(self)->n)/((self)->m-(other)->m)) )
point_t line_intersect_line(const line_t* self, const line_t* other);
/*  */
#define line_distanceto_circle_m(line, circle) \
	( point_distanceto_line_m(&(circle)->center, self) - (circle)->radius )
ftype line_distanceto_circle(const line_t* self, const circle_t* circle);



#define circle_distanceto_circle_m(self, other) \
	( point_distanceto_point_m(&(self)->center, &(other)->center) - (self)->radius - (other)->radius )
ftype circle_distanceto_circle(const circle_t* self, const circle_t* other);
/*  */
point_t* circle_intersect_circle(const circle_t* self, const circle_t* line);
bool circle_intersect_circle_b(point_t* buffer, const circle_t* self, const circle_t* line);
/*  */
point_t* circle_intersect_line(const circle_t* self, const line_t* line);
bool circle_intersect_line_b(point_t* buffer, const circle_t* self, const line_t* line);



ftype* find_roots(ftype a, ftype b, ftype c);
bool find_roots_b(ftype* buffer, ftype a, ftype b, ftype c);


#ifdef CINTER_IMPL
#include "impl/cinter-impl.h"
#endif

#endif
