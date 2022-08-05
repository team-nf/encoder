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

bool point_ison_circle(const point_t* self, const circle_t* circle) {
	return check_wt(point_distanceto_point(&circle->center, self), circle->radius);
}

bool point_ison_line(const point_t* self, const line_t* line) {
	return check_wt(line->m * self->x + line->n, self->y);
}

point_t points_find_center(const point_t* points, int point_num) {
	point_t sum = {0, 0};
	for (int i = 0; i < point_num; i++) {
		sum.x += points[i].x;
		sum.y += points[i].y;
	}
	sum.x /= point_num;
	sum.y /= point_num;
	return sum;	
}



point_t line_calc_pointfx(const line_t* self, ftype x) {
	point_t rv = {x, self->m * x + self->n}; return rv;
}

point_t line_calc_pointfy(const line_t* self, ftype y) {
	point_t rv = {(y - self->n) / self->m, y}; return rv;
}

point_t line_intersect_line(const line_t* self, const line_t* other) { 
	return line_calc_pointfx(self, (ftype)(other->n-self->n)/(self->m-other->m));
}

ftype line_distanceto_circle(const line_t* self, const circle_t* circle) {
	return point_distanceto_line(&circle->center, self) - circle->radius;
}


point_t* circle_intersect_circle(const circle_t* self, const circle_t* circle) {
	point_t *buffer = (point_t *)malloc(2 * sizeof(point_t));
	if (circle_intersect_circle_b(buffer, self, circle))
		return buffer;
	return NULL;
}

bool circle_intersect_circle_b(point_t* buffer, const circle_t* self, const circle_t* circle) {
/* iki çember max 2 noktada kesişebilir */
	if (check_wt(self->radius, 0)) {
		if (point_ison_circle_m(&self->center, circle)) {
			memcpy(&buffer[0], &self->center, sizeof(point_t));
			memcpy(&buffer[1], &self->center, sizeof(point_t));
			return true;
		} return false;
	} if (check_wt(circle->radius, 0)) {
		if (point_ison_circle_m(&circle->center, self)) {
			memcpy(&buffer[0], &self->center, sizeof(point_t));
			memcpy(&buffer[1], &self->center, sizeof(point_t));
			return true;
		} return false;
	}

	/* karmaşık matematiksel işlem, burada anlatmaya çalışmayacağım.
	 * Belki dokümantasyonda anlatırım */
	line_t new_line = {(self->center.x - circle->center.x) / (circle->center.y - self->center.y),
		(pow(self->radius, 2) + pow(circle->center.x, 2) + pow(circle->center.y, 2)
		- pow(circle->radius, 2) - pow(self->center.x, 2) - pow(self->center.y, 2)) /
		(2*(circle->center.y - self->center.y))
	}; return circle_intersect_line_b(buffer, self, &new_line);
}

point_t* circle_intersect_line(const circle_t* self, const line_t* line) {
	point_t *buffer = (point_t *)malloc(2 * sizeof(point_t));
	if (circle_intersect_line_b(buffer, self, line))
		return buffer;
	return NULL;
}

bool circle_intersect_line_b(point_t* buffer, const circle_t* self, const line_t* line) {
	if (check_wt(self->radius, 0)) {
		if (point_ison_line_m(&self->center, line)) {
			memcpy(&buffer[0], &self->center, sizeof(point_t));
			memcpy(&buffer[1], &self->center, sizeof(point_t));
			return true;
		} return false;
	} 
	ftype roots[2];
	bool rv = find_roots_b( roots,
							pow(line->m, 2) + 1,
							2*(line->m*(line->n-self->center.y)-self->center.x),
							pow(line->n-self->center.y, 2)+pow(self->center.x, 2)-pow(self->radius, 2));
	if (!rv) { return false; }

	/* tek kök olduğunda tekrar hesaplamama gerek yoktu evet ama 
	 * fonksiyonun yaptığı işlem pahalı bir işlem değil */
	buffer[0] = line_calc_pointfx(line, roots[0]); 
	buffer[1] = line_calc_pointfx(line, roots[1]);
	return true;
}

ftype circle_distanceto_circle(const circle_t* self, const circle_t* other) {
	return point_distanceto_point(&self->center, &other->center) - self->radius - other->radius;
}


ftype* find_roots(ftype a, ftype b, ftype c) {
	ftype *buffer = (ftype *)malloc(2 * sizeof(ftype));
	if (!find_roots_b(buffer, a, b, c)) return NULL;
	return buffer;
}

bool find_roots_b(ftype* buffer, ftype a, ftype b, ftype c) {
	ftype delta = pow(b, 2) - 4*a*c;
	if (delta < 0) return false;
	/* deltanın 0 olma durumunu kontrol etmeli miyiz emin değilim *
	 * sqrt fonksiyonunun ne kadar pahalı olduğunu bilmiyorum     */
	if (delta == 0) { buffer[0] = -b/(2*a); buffer[1] = -b/(2*a); }
	else { buffer[0] = (-b-sqrt(delta))/(2*a); buffer[1] = (-b+sqrt(delta))/(2*a); }
	return true;
}



#endif
