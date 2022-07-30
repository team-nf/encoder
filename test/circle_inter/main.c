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

struct ft_buffer {
	point_t _inter_rv[2];
	point_t intersections[_max_intersections_g];
	circle_t circles[_sensor_num_g];
};

point_t find_target(struct ft_buffer *buffer, int sensor_num, circle_t* magnet_projection) {
#define circles (buffer->circles)
#define intersections (buffer->intersections)
#define inter_rv (buffer->_inter_rv)

#ifndef _skip_perfect_check
	/* bu noktayı iptal etme şansım var, testlere göre değerlendireceğim */
	int i; 
	if (circle_intersect_circle_b(inter_rv, &circles[0], &circles[1])) { 
		assert("cicb error\n"); 
	}
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
#endif

	printf("Unperfect calculation.\n");
	/* === Eğer elimizdeki değerler mükemmel bir nokta oluşturmuyorsa === */
	/*				(ki yüksek ihtimalle oluşturmayacak)				  */
	
	/* normalde maksimum kesişme sayısı aşağıdakinin 2 katı ama her kesişmenin *
	 * noktaya yakın olan kısmını alacağız o yüzden 2 ile çarpmamız gerekmiyor */
	int intersections_len = 0;
	
#ifndef _skip_perfect_check
	if (point_distanceto_circle(&inter_rv[0], magnet_projection) > 
			point_distanceto_circle(&inter_rv[1], magnet_projection)) {
		intersections[0] = inter_rv[1];
	} else { intersections[0] = inter_rv[0]; }
	for (i = 1; i < sensor_num; i++) {
#endif

#ifdef _skip_perfect_check
	printf("Skipped perfect check.\n");
	for (int i = 0; i < sensor_num-1; i++) {
#endif
		/* sırayla tüm çemberli birbirleriyle kesiştir */
		for (int j = i+1; j < sensor_num; j++){
			/* çemberlerin kesişme noktalarını al */
			if (circle_intersect_circle_b(inter_rv, &circles[i], &circles[j])) {
				assert("cicb error\n"); 
			}

			/* bizim mıknatısın yoluna daha yakın olanı kaydet */
			if (point_distanceto_circle(&inter_rv[0], magnet_projection) > 
					point_distanceto_circle(&inter_rv[1], magnet_projection))
				intersections[intersections_len++] = inter_rv[1];
			else intersections[intersections_len++] = inter_rv[0];
		}
	}

	/* toplanan kesişim noktalarının ortalamasını al */
	return points_find_center(intersections, intersections_len);
#undef circles
#undef intersections
#undef inter_rv
}

#define to_radian(degree) ((double)degree / 180 * M_PI)
#define to_degree(radian) ((double)radian / M_PI * 180)

int main() {
	/* For once */
	printf("Started.\n");
	point_t *positions = calc_sensor_positions(_sensor_num_g);
	if (positions == NULL) { assert("calc_sensor_positions error\n"); }

	point_t magnet_center = {0, 0};
	double magnet_radius = 0.5;
	circle_t _projection = {magnet_center, magnet_radius};
	circle_t *magnet_projection = &_projection;
	struct ft_buffer *ft_buffer = (struct ft_buffer *)malloc(sizeof(struct ft_buffer));

	/* For each loop */
	{
		int angle = 30;
		/* point_t test_point = {1, 1}; */
		point_t test_point = { magnet_radius * cos(to_radian(angle)), magnet_radius * sin(to_radian(angle)) };
		ftype *distances = calc_test_distances(test_point, positions, _sensor_num_g);
		if (distances == NULL) { assert("calc_test_distances error\n"); }

		/* create noise */
		for (int i=0; i < _sensor_num_g; i++) {
			distances[i] = distances[i] + 0.0001;
		}
		
		/* initialize circles */
		for (int i = 0; i < _sensor_num_g; i++) {
			ft_buffer->circles[i].center = positions[i];
			ft_buffer->circles[i].radius = distances[i];
		}

		if (magnet_projection == NULL) {
			/* eğer mıknatısın yolu hakkında herhangi bir bilgimiz  */
			/* yoksa en içte olan kesişim noktalarını almak için */
			point_t _center = {0, 0};
			circle_t _projection = {_center, 0};
			magnet_projection = &_projection;
		}

		point_t found_target = find_target(ft_buffer, _sensor_num_g, magnet_projection);


		printf("test  target: ("_fmt", "_fmt"), angle: %f\n", 
				test_point.x, test_point.y, to_degree(atan(test_point.y/test_point.x)));
		printf("found target: ("_fmt", "_fmt"), angle: %f\n", 
				found_target.x, found_target.y, to_degree(atan(found_target.y/found_target.x)));
	/* 	if (!check_wt(test_point.x, found_target.x)) {  */
	/* 		printf("x error at angle: %d; test: "_fmt", found: "_fmt"\n",  */
	/* 				angle, test_point.x, found_target.x);  */
	/* 	} else if (!check_wt(test_point.y, found_target.y)) {  */
	/* 		printf("y error at angle: %d; test: "_fmt", found: "_fmt"\n",  */
	/* 				angle, test_point.y, found_target.y);  */
	/* 	} else { printf("%d degrees ok.\n", angle); } */


		free(distances);
	}free(positions);
	printf("Done.\n");
	return 0;
}
