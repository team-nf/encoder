/*
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci

SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef _CALC_H_INCLUDED
#define _CALC_H_INCLUDED

#include "analytics.h"



#define calc_sensor_angles_m(sensor_num) ({ \
	ftype *angles = (ftype *)malloc(sensor_num * sizeof(ftype)); \
	calc_sensor_angles_bm(angles, sensor_num); \
	angles })
ftype* calc_sensor_angles(int sensor_num);
/*  */
#define calc_sensor_angles_bm(buffer, sensor_num) ({ \
	for (int i = 0; i < sensor_num; i++)		\
		buffer[i] = i * 2 * M_PI / sensor_num;	\
	})
void calc_sensor_angles_b(ftype* buffer, int sensor_num);



#define calc_sensor_positions_m(sensor_num) ({ \
	point_t *positions = (point_t *)malloc(sensor_num * sizeof(point_t)); \
	calc_sensor_positions(positions, sensor_num); \
	positions })
point_t* calc_sensor_positions(int sensor_num);
/*  */
#define calc_sensor_positions_bm(buffer, sensor_num) ({ \
	for (int i = 0; i < sensor_num; i++) { \
		buffer[i].x = (ftype)cos((double)(i * 2 * M_PI / sensor_num)); \
		buffer[i].y = (ftype)sin((double)(i * 2 * M_PI / sensor_num)); \
	} }) 
void calc_sensor_positions_b(point_t* buffer, int sensor_num);

void calc_sensor_positions_bs(void* buffer, int step, int sensor_num);


#define calc_test_distances_m(test_point, sensor_positions, sensor_num) ({ \
	ftype *distances = (ftype *)malloc(sensor_num * sizeof(ftype)); \
	calc_test_distances_bm(distances, sensor_positions, sensor_num); \
	distances })
ftype* calc_test_distances(point_t test_point, const point_t *sensor_positions, int sensor_num);

#define calc_test_distances_bm(buffer, test_point, sensor_positions, sensor_num) ({ \
	for (int i = 0; i < sensor_num; i++) { \
		buffer[i] = point_distanceto_point_m(&test_point, &sensor_positions[i]); \
	} })
void calc_test_distances_b
	(ftype* buffer, point_t test_point, const point_t *sensor_positions, int sensor_num);

void calc_test_distances_bs
	(void* buffer, int buffer_step, point_t test_point, 
	 const void *sensor_positions, int positions_step, int sensor_num);


/* find_target tarafından kullanılacak değişkenlerin tutulduğu yer */
/* her seferinde tekrar malloc yapmakla zaman harcamamak */ 
/* için program başında sadece bir kere malloc yapılıyor */
struct ft_buffer {
	point_t _inter_rv[2];
}; 

point_t find_target(struct ft_buffer *buffer, circle_t *circles, int sensor_num, circle_t* magnet_projection);


#define to_radian(degree) ((double)degree / 180 * M_PI)
#define to_degree(radian) ((double)radian / M_PI * 180)


#ifdef CALC_IMPL

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

void calc_sensor_positions_bs(void* buffer, int step, int sensor_num) {
	for (int i = 0; i < sensor_num; i++) {
		((point_t *)(buffer+i*step))->x = (ftype)cos((double)(i * 2 * M_PI / sensor_num));
		((point_t *)(buffer+i*step))->y = (ftype)sin((double)(i * 2 * M_PI / sensor_num));
	}
}


ftype* calc_test_distances(point_t test_point, const point_t *sensor_positions, int sensor_num) {
	ftype *distances = (ftype *)malloc(sensor_num * sizeof(ftype));
	calc_test_distances_b(distances, test_point, sensor_positions, sensor_num);
	return distances;
}

void calc_test_distances_b
	(ftype* buffer, point_t test_point, const point_t *sensor_positions, int sensor_num)
{
	for (int i = 0; i < sensor_num; i++) {
		*(buffer+i) = point_distanceto_point(&test_point, &sensor_positions[i]);
	}
}

void calc_test_distances_bs
	(void* buffer, int buffer_step, point_t test_point, 
	 const void *sensor_positions, int positions_step, int sensor_num)
{
	for (int i = 0; i < sensor_num; i++) {
		*(ftype *)(buffer+i*buffer_step) = 
			point_distanceto_point(&test_point, (point_t *)(sensor_positions+i*positions_step));
	}
}


point_t find_target(struct ft_buffer *buffer, circle_t *circles, int sensor_num, circle_t* magnet_projection) {
#define intersections (buffer->intersections)
#define inter_rv (buffer->_inter_rv)

#ifndef _skip_perfect_check
	/* bu noktayı iptal etme şansım var, testlere göre değerlendireceğim */
	int i; 
	if (!circle_intersect_circle_b(inter_rv, &circles[0], &circles[1])) { 
		serialdn("cicb error encoder-56"); 
	}
	/*	eğer yukarıdaki satırdan gelen kesişim noktalarından  */
	/* herhangi biri tüm çemberler üzerinde varsa fazla uğraşmadan döndür */
	for (i = 2; i < sensor_num; i++) {
		if (!point_ison_circle(&inter_rv[0], &circles[i])) break;	
	} if (i == sensor_num) return inter_rv[0];

	/* eğer üstteki döngü bir tur bile ilerlediyse bu döngüyü yapmamız tamamen anlamsız oluyor */
	/* sebebini burada açıklamam ne kadar doğru olur bilmiyorum kısa bir açıklama değil çünkü */

	/* ufak bir açıklama yapmaya karar verdim */
	/* 13 satır yukarıda inter_rv içine ilk iki çemberin iki kesişim noktasını yazıyoruz */
	/* yukarıdaki döngü iki kesişim noktasından ilkinin tüm çemberlerin üzerinde olup */
	/* olmadığını kontrol ediyor. Hemen aşağıdaki döngü de ikinci kesişim noktası için */
	/* kontrolleri yapıyor. */
	/* Şimdi eğer ilk kesişim noktası tüm çemberler üzerinde değilse ama 3 veya daha fazla */
	/* çember üzerindeyse, ikinci kesişim noktasını tekrar kontrol etmeye gerek kalmıyor */
	/* çünkü, ikinci kesişim noktasının, ilk kesişim noktasının üzerinde bulunduğu çemberlerin */
	/* üzerinde bulunmayacağını biliyoruz. Matematiksel olarak söylediğim cümlenin doğruluğunu */
	/* kanıtlamayacağım ama 3 çemberin aynı anda 2 noktada kesişememesinden bi şeyler bulunabilir */
	/* (çemberler aynı olmadığı sürece tabii). */
	if (i == 2) {	
		/* biraz düşün belki hatırlarsın (gelecekteki bana not) */
		for (; i < sensor_num; i++) {
			if (!point_ison_circle(&inter_rv[1], &circles[i])) break;	
		} if (i == sensor_num) return inter_rv[1];
	}
#endif

	/* serialdn("Unperfect calculation.\n"); */
	/* === Eğer elimizdeki değerler mükemmel bir nokta oluşturmuyorsa === */
	/*				(ki yüksek ihtimalle oluşturmayacak)				  */
	
	/* normalde bizim magnet projectionımıza yakın olan noktaların hepsini bir arrayde */ 
	/* topluyorduk ama yeni güncellemeyle her noktanın kaydedlmesini kaldırdım ve hafıza */ 
	/* kullanımı (intersection_len * 8) byte azaldı (intersection_len de c(sensor_num, 2) */
	/* üzerinden hesaplanıyor. Artık projectiona yakın olan nokta hesaplandıktan sonra */
	/* noktanın kordinatlarını kaydetmek yerine noktaların kordinatlarını aşağıdaki değişkenlerde */ 
	/* topluyoruz. Sonrasında toplam kesişim sayısına bölerek ortalamasını alıyoruz */
	ftype sumx = 0, sumy = 0;
	
#ifndef _skip_perfect_check
	/* eğer perfect check açıksa zaten kesin olarak ilk iki çemberi kesiştirdik  */
	/* elde ettiğimiz iki kesişim noktasından bizim magnet_projectionımıza daha yakın olanı suma ekle */
	if (point_distanceto_circle(&inter_rv[0], magnet_projection) > 
			point_distanceto_circle(&inter_rv[1], magnet_projection)) 
	{
		sumx += inter_rv[1].x;
		sumy += inter_rv[1].y;
	} else {
		sumx += inter_rv[0].x;
		sumy += inter_rv[0].y;
	}

	for (i = 1; i < sensor_num; i++) {
#endif

#ifdef _skip_perfect_check
	for (int i = 0; i < sensor_num-1; i++) {
#endif
		/* sırayla tüm çemberli birbirleriyle kesiştir */
		for (int j = i+1; j < sensor_num; j++){
			/* çemberlerin kesişme noktalarını al */
			if (!circle_intersect_circle_b(inter_rv, &circles[i], &circles[j])) {
				serialdn("cicb error encoder-98"); 
			}

			/* bizim mıknatısın yoluna daha yakın olanı suma ekle */
			if (point_distanceto_circle(&inter_rv[0], magnet_projection) > 
					point_distanceto_circle(&inter_rv[1], magnet_projection))
			{
				sumx += inter_rv[1].x;
				sumy += inter_rv[1].y;
			} else {
				sumx += inter_rv[0].x;
				sumy += inter_rv[0].y;
			}
		}
	}

	point_t result = { sumx/_max_intersections_g, sumy/_max_intersections_g };
	return result;
#undef intersections
#undef inter_rv
}

#endif
#endif
