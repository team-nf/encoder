#include <stdio.h>

#define CINTER_IMPL
#include "cinter.h"

/* point_t line_calc_pointfx(line_t* self, ftype x); */
/* point_t line_calc_pointfy(line_t* self, ftype y); */

/* point_t* circle_intersect_circle(const circle_t* self, const circle_t* line); */
/* point_t* circle_intersect_line(const circle_t* self, const line_t* line); */


int main() {
	point_t point0 = {0, 0}, point1 = {1, 1}, point2 = {4, 5}, point3 = {1, 0};
	printf("distance (5): %f\n", point_find_distance(&point1, &point2));
	
	circle_t circle0 = {point0, 1}, circle1 = {point1, 1}, circle2 = {point2, 5};
	printf("is on circle (0, 1): %d, %d\n", 
			point_is_on_circle(&point0, &circle0),
			point_is_on_circle(&point3, &circle0));

	line_t line0 = {2, 0}, line1 = {-0.75, 1.875};
	printf("is on line (0, 1): %d, %d\n", 
			point_is_on_line(&point1, &line0),
			point_is_on_line(&point0, &line0));

	ftype *roots = find_roots(1, 0, -25);
	printf("roots (-5, 5): %f, %f\n", roots[0], roots[1]);

	point_t *rv = circle_intersect_line(&circle1, &line1);
	printf("circle intersect line: (%f, %f), (%f, %f)\n", rv[0].x, rv[0].y, rv[1].x, rv[1].y);
	free(rv);

	rv = circle_intersect_circle(&circle1, &circle2);
	printf("circle intersect circle: (%f, %f), (%f, %f)\n", rv[0].x, rv[0].y, rv[1].x, rv[1].y);
	free(rv);

	return 0;
}
