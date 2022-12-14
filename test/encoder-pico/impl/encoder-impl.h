/*
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci

SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef _ENCODER_IMPL_H_INCLUDED
#define _ENCODER_IMPL_H_INCLUDED



ftype* calc_sensor_angles(int sensor_num) {
	ftype *angles = (ftype *)malloc(sensor_num * sizeof(ftype));
	calc_sensor_angles_b(angles, sensor_num);
	return angles;
}

void calc_sensor_angles_b(ftype* buffer, int sensor_num) {
	for (int i = 0; i < sensor_num; i++)
		buffer[i] = i * 2 * M_PI / sensor_num;
}



point_t* calc_sensor_positions(int sensor_num) {
	point_t *positions = (point_t *)malloc(sensor_num * sizeof(point_t));
	calc_sensor_positions_b(positions, sensor_num);
	return positions;
}

void calc_sensor_positions_b(point_t* buffer, int sensor_num) {
	for (int i = 0; i < sensor_num; i++) {
		buffer[i].x = (ftype)cos((double)(i * 2 * M_PI / sensor_num));
		buffer[i].y = (ftype)sin((double)(i * 2 * M_PI / sensor_num));
	}
}


ftype* calc_test_distances(point_t test_point, const point_t *sensor_positions, int sensor_num) {
	ftype *distances = (ftype *)malloc(sensor_num * sizeof(ftype));
	calc_test_distances_b(distances, test_point, sensor_positions, sensor_num);
	return distances;
}

void calc_test_distances_b(ftype* buffer, point_t test_point, const point_t *sensor_positions, int sensor_num) {
	for (int i = 0; i < sensor_num; i++) {
		buffer[i] = point_distanceto_point(&test_point, &sensor_positions[i]);
	}
}



point_t find_target(struct ft_buffer *buffer, int sensor_num, circle_t* magnet_projection) {
#define circles (buffer->circles)
#define intersections (buffer->intersections)
#define inter_rv (buffer->_inter_rv)

#ifndef _skip_perfect_check
	/* bu noktayı iptal etme şansım var, testlere göre değerlendireceğim */
	int i; 
	if (!circle_intersect_circle_b(inter_rv, &circles[0], &circles[1])) { 
		/* serialdn("cicb error encoder-56");  */
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

	/* printf("Unperfect calculation.\n"); */
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
	for (int i = 0; i < sensor_num-1; i++) {
#endif
		/* sırayla tüm çemberli birbirleriyle kesiştir */
		for (int j = i+1; j < sensor_num; j++){
			/* çemberlerin kesişme noktalarını al */
			if (!circle_intersect_circle_b(inter_rv, &circles[i], &circles[j])) {
				/* serialdn("cicb error encoder-98");  */
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

#endif
