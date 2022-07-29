#include <stdio.h>

#define CINTER_IMPL
#include "cinter.h"

#define _sensor_num_g 3


ftype* calc_sensor_angles(int sensor_num) {
	ftype *angles = (ftype *)malloc(sensor_num * sizeof(ftype));
	for (int i = 0; i < sensor_num; i++)
		angles[i] = i * M_PI / sensor_num;
	return angles;
}

point_t* calc_sensor_positions(int sensor_num) {
	point_t *positions = (point_t *)malloc(sensor_num * sizeof(point_t));
	for (int i = 0; i < sensor_num; i++) {
		positions[i].x = (ftype)cos((double)(i * 2 * M_PI / sensor_num));
		positions[i].y = (ftype)sin((double)(i * 2 * M_PI / sensor_num));
	} return positions;
}

ftype* calc_test_distances(point_t test_point, const point_t *sensor_positions, int sensor_num) {
	ftype *distances = (ftype *)malloc(sensor_num * sizeof(ftype));
	for (int i = 0; i < sensor_num; i++) {
		distances[i] = point_distanceto_point(&test_point, &sensor_positions[i]);
	} return distances;
}

point_t find_target(const ftype* distances, const point_t* positions, int sensor_num) {
	point_t magnet_center = {0, 0};
	circle_t magnet_projection = {magnet_center, 0.5};
	/* her seferinde bunlar için yeniden malloc çağırmak doğru değil buna bi çözüm buluruz */
	circle_t *circles = (circle_t *)malloc(sensor_num * sizeof(circle_t));
	for (int i = 0; i < sensor_num; i++) {
		circles[i].center = positions[i];
		circles[i].radius = distances[i];
	}


#ifndef _skip_perfect_check
	/* bu noktayı iptal etme şansım var, testlere göre değerlendireceğim */
	int i; point_t *inter_rv = circle_intersect_circle(&circles[0], &circles[1]);
	/*	eğer yukarıdaki satırdan gelen kesişim noktalarından  */
	/* herhangi biri tüm çemberler üzerinde varsa fazla uğraşmadan döndür */
	for (i = 2; i < sensor_num; i++) {
		if (!point_ison_circle(&inter_rv[0], &circles[i])) break;	
	} if (i == sensor_num) return inter_rv[0];

	/* eğer üstteki döngü bir tur bile ilerlediyse bu döngüyü yapmamız tamamen anlamsız oluyor */
	/* sebebini burada açıklamam ne kadar doğru olur bilmiyorum kısa bir açıklama değil çünkü */
	if (i == 2) {	
		/* biraz düşün belki hatırlarsın (gelecekteki bana not) */
		for (; i < sensor_num; i++) {
			if (!point_ison_circle(&inter_rv[1], &circles[i])) break;	
		} if (i == sensor_num) return inter_rv[1];
	}
	/* ENDREGION */
#endif

	
	
	/* === Eğer elimizdeki değerler mükemmel bir nokta oluşturmuyorsa === */
	/*				(ki yüksek ihtimalle oluşturmayacak)				  */
	
	/* normalde maksimum kesişme sayısı aşağıdakinin 2 katı ama her kesişmenin *
	 * noktaya yakın olan kısmını alacağız o yüzden 2 ile çarpmamız gerekmiyor */
	ftype max_intersections = sensor_num * (sensor_num-1) / 2;
	point_t *intersections = (point_t *)malloc(max_intersections * sizeof(point_t));
	int intersections_len = 0;
	
#ifndef _skip_perfect_check
	intersections[0] = inter_rv[0];
	intersections[1] = inter_rv[1];
	free(inter_rv);
	for (i = 2; i < sensor_num; i++) {
#endif

#ifdef _skip_perfect_check
	point_t *inter_rv;
	for (i = 0; i < sensor_num; i++) {
#endif
		/* sırayla tüm çemberli birbirleriyle kesiştir */
		for (int j = i+1; j < sensor_num; i++){
			/* çemberlerin kesişme noktalarını al */
			inter_rv = circle_intersect_circle(&circles[i], &circles[j]);
			if (inter_rv == NULL) { assert("null error"); }

			/* bizim mıknatısın yoluna daha yakın olanı kaydet */
			if (point_distanceto_circle(&inter_rv[0], &magnet_projection) > 
					point_distanceto_circle(&inter_rv[1], &magnet_projection))
				intersections[intersections_len++] = inter_rv[0];
			else intersections[intersections_len++] = inter_rv[1];
			free(inter_rv);
		}
	}

	/* toplanan kesişim noktalarının ortalamasını al */
	point_t sum = {0, 0};
	for (i = 0; i < intersections_len; i++) {
		sum.x += intersections[i].x;
		sum.y += intersections[i].y;
	}
	sum.x /= intersections_len; 
	sum.y /= intersections_len; 
	return sum;
}

#define to_radian(degree) ((double)degree / 180 * M_PI)
#define to_degree(radian) ((double)radian / M_PI * 180)

int main() {
	/* For once */
	printf("Started.\n");
	point_t *positions = calc_sensor_positions(_sensor_num_g);
	if (positions == NULL) { assert("calc_sensor_positions error\n"); }

	double magnet_radius = 0.5;
	/* For each loop */
	for (int angle=0; angle < 360; angle++) {
		/* int angle = 30; */
		/* point_t test_point = {1, 1}; */
		point_t test_point = { magnet_radius * cos(to_radian(angle)), magnet_radius * sin(to_radian(angle)) };
		ftype *distances = calc_test_distances(test_point, positions, _sensor_num_g);
		if (distances == NULL) { assert("calc_test_distances error\n"); }
		
		point_t found_target = find_target(distances, positions, _sensor_num_g);
/* 		printf("test  target: ("_fmt", "_fmt")\n", test_point.x, test_point.y);  */
/* 		printf("found target: ("_fmt", "_fmt")\n", found_target.x, found_target.y); */
		if (!check_wt(test_point.x, found_target.x)) { 
			printf("x error at angle: %d; test: "_fmt", found: "_fmt"\n", 
					angle, test_point.x, found_target.x); 
		} if (!check_wt(test_point.y, found_target.y)) { 
			printf("y error at angle: %d; test: "_fmt", found: "_fmt"\n", 
					angle, test_point.y, found_target.y); 
		}

		free(distances);
	} free(positions);
	printf("Done.\n");
	return 0;
}
