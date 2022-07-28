#include <stdio.h>

#define CINTER_IMPL
#include "cinter.h"


void test() {
	point_t point0 = {0, 0}, point1 = {1, 1}, point2 = {4, 5}, point3 = {1, 0};
	line_t line0 = {2, 0}, line1 = {-0.75, 1.875}, line2 = {1, 1}, line3 = {2, 0};
	circle_t circle0 = {point0, 1}, circle1 = {point1, 1}, circle2 = {point2, 5};

	printf("point distanceto point      (5): %Lf\n", point_distanceto_point(&point1, &point2));
	printf("point distanceto line    (~0.7): %Lf\n", point_distanceto_line(&point0, &line2));
	printf("point distanceto circle  (~0.4): %Lf\n", point_distanceto_circle(&point1, &circle0));
	printf("line distanceto circle     (<0): %Lf\n", line_distanceto_circle(&line2, &circle1));
	printf("circle distanceto circle   (<0): %Lf\n", circle_distanceto_circle(&circle0, &circle1));

	printf("is on circle             (0, 1): %d, %d\n", 
			point_is_on_circle(&point0, &circle0),
			point_is_on_circle(&point3, &circle0));

	printf("is on line               (0, 1): %d, %d\n", 
			point_is_on_line(&point1, &line0),
			point_is_on_line(&point0, &line0));

	ftype *roots = find_roots(1, 0, -25);
	printf("roots                   (-5, 5): %Lf, %Lf\n", roots[0], roots[1]);

	point_t rv0 = line_intersect_line(&line2, &line3);
	printf("line intersect line      (1, 2): (%Lf, %Lf)\n", rv0.x, rv0.y);

	point_t *rv = circle_intersect_line(&circle1, &line1);
	printf("circle intersect line          : (%.20Lf, %.20Lf), (%.20Lf, %.20Lf)\n", \
			rv[0].x, rv[0].y, rv[1].x, rv[1].y);
	free(rv);

	rv = circle_intersect_circle(&circle1, &circle2);
	printf("circle intersect circle        : (%.20Lf, %.20Lf), (%.20Lf, %.20Lf)\n", \
			rv[0].x, rv[0].y, rv[1].x, rv[1].y);
	free(rv);
}

int main() {
	test();
	return 0;
}
