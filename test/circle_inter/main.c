#include <stdio.h>

#define CINTER_IMPL
#include "cinter.h"


int main() {
	point_t point0 = {0, 0}, point1 = {1, 1}, point2 = {4, 5}, point3 = {1, 0};
	printf("distance (5): %Lf\n", point_distanceto_point(&point1, &point2));
	
	circle_t circle0 = {point0, 1}, circle1 = {point1, 1}, circle2 = {point2, 5};
	printf("is on circle (0, 1): %d, %d\n", 
			point_is_on_circle(&point0, &circle0),
			point_is_on_circle(&point3, &circle0));

	line_t line0 = {2, 0}, line1 = {-0.75, 1.875};
	printf("is on line (0, 1): %d, %d\n", 
			point_is_on_line(&point1, &line0),
			point_is_on_line(&point0, &line0));

	ftype *roots = find_roots(1, 0, -25);
	printf("roots (-5, 5): %.20Lf, %.20Lf\n", roots[0], roots[1]);

	point_t *rv = circle_intersect_line(&circle1, &line1);
	printf("circle intersect line:   (%.20Lf, %.20Lf), (%.20Lf, %.20Lf)\n", rv[0].x, rv[0].y, rv[1].x, rv[1].y);
	free(rv);

	rv = circle_intersect_circle(&circle1, &circle2);
	printf("circle intersect circle: (%.20Lf, %.20Lf), (%.20Lf, %.20Lf)\n", rv[0].x, rv[0].y, rv[1].x, rv[1].y);
	free(rv);

	return 0;
}
