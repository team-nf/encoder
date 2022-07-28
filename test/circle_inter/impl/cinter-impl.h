#ifndef _CINTER_IMPL_INCLUDED
#define _CINTER_IMPL_INCLUDED

/* bu tarz kısa fonksiyonları direkt macro yapmak daha mantıklı olabilir */
ftype point_distanceto_point(const point_t* self, const point_t* other) {
	return sqrt(pow(self->x - other->x, 2) + pow(self->y - other->y, 2));
}

ftype point_distanceto_line(const point_t* self, const line_t* line) {
	return abs(self->y - line->m*self->x - line->n) / sqrt(pow(line->m, 2)+1);
}

ftype point_distanceto_circle(const point_t* self, const circle_t* circle) {
	return point_distanceto_point(self, &circle->center) - circle->radius;
}

bool point_is_on_circle(const point_t* self, const circle_t* circle) {
	return check_wt(circle->radius, point_distanceto_point(&circle->center, self));
}

bool point_is_on_line(const point_t* self, const line_t* line) {
	return check_wt(self->y, line->m * self->x + line->n);
}



point_t line_calc_pointfx(const line_t* self, ftype x) {
	point_t rv = {x, self->m * x + self->n}; return rv;
}

point_t line_calc_pointfy(const line_t* self, ftype y) {
	point_t rv = {(y - self->n) / self->m, y}; return rv;
}

point_t line_intersect_line(line_t* self, line_t* other) { 
	return line_calc_pointfx(self, (other->n-self->n)/(self->m-self->m));
}

ftype line_distanceto_circle(const line_t* line, const circle_t* circle) {
	return point_distanceto_line(&circle->center, line) - circle->radius;
}


point_t* circle_intersect_circle(const circle_t* self, const circle_t* circle) {
/* iki çember max 2 noktada kesişebilir */
	if (check_wt(self->radius, 0)) {
		if (point_is_on_circle(&self->center, circle)) {
			point_t *rv = (point_t *)malloc(2 * sizeof(point_t));
			memcpy(&rv[0], &self->center, sizeof(point_t));
			memcpy(&rv[1], &self->center, sizeof(point_t));
			return rv;
		} return NULL;
	} if (check_wt(circle->radius, 0)) {
		if (point_is_on_circle(&circle->center, self)) {
			point_t *rv = (point_t *)malloc(2 * sizeof(point_t));
			memcpy(&rv[0], &self->center, sizeof(point_t));
			memcpy(&rv[1], &self->center, sizeof(point_t));
			return rv;
		} return NULL;
	}

	/* karmaşık matematiksel işlem, burada anlatmaya çalışmayacağım.
	 * Belki dokümantasyonda anlatırım */
	line_t new_line = {(self->center.x - circle->center.x) / (circle->center.y - self->center.y),
		(pow(self->radius, 2) + pow(circle->center.x, 2) + pow(circle->center.y, 2)
		- pow(circle->radius, 2) - pow(self->center.x, 2) - pow(self->center.y, 2)) /
		(2*(circle->center.y - self->center.y))
	}; return circle_intersect_line(self, &new_line);
}

point_t* circle_intersect_line(const circle_t* self, const line_t* line) {
	if (check_wt(self->radius, 0)) {
		if (point_is_on_line(&self->center, line)) {
			point_t *rv = (point_t *)malloc(2 * sizeof(point_t));
			memcpy(&rv, &self->center, sizeof(point_t));
			return rv;
		} return NULL;
	} 
	ftype *roots = find_roots(pow(line->m, 2) + 1,
							  2*(line->m*(line->n-self->center.y)-self->center.x),
							 pow(line->n-self->center.y, 2)+pow(self->center.x, 2)-pow(self->radius, 2));
	if (roots == NULL) return NULL;
	point_t *rv = (point_t *)malloc(2 * sizeof(point_t));
	/* tek kök olduğunda tekrar hesaplamama gerek yoktu evet ama 
	 * fonksiyonun yaptığı işlem pahalı bir işlem değil */
	rv[0] = line_calc_pointfx(line, roots[0]); rv[1] = line_calc_pointfx(line, roots[1]);
	free(roots); return rv;
}

ftype circle_distanceto_circle(const circle_t* self, const circle_t* other) {
	return point_distanceto_point(&self->center, &other->center) - self->radius - other->radius;
}



ftype* find_roots(ftype a, ftype b, ftype c) {
	ftype delta = pow(b, 2) - 4*a*c;
	if (delta < 0) return NULL;
	ftype *rv = (ftype *)malloc(2 * sizeof(ftype));
	/* deltanın 0 olma durumunu kontrol etmeli miyiz emin değilim *
	 * sqrt fonksiyonunun ne kadar pahalı olduğunu bilmiyorum     */
	if (delta == 0) { rv[0] = -b/(2*a); rv[1] = -b/(2*a); }
	else { rv[0] = (-b-sqrt(delta))/(2*a); rv[1] = (-b+sqrt(delta))/(2*a); }
	return rv;
}



#endif
