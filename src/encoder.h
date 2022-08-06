#ifndef _ENCODER_H_INCLUDED
#define _ENCODER_H_INCLUDED

#include "cinter.h"



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


#define calc_test_distances_m(test_point, sensor_positions, sensor_num) ({ \
	ftype *distances = (ftype *)malloc(sensor_num * sizeof(ftype)); \
	calc_test_distances_bm(distances, sensor_positions, sensor_num); \
	distances })
ftype* calc_test_distances(point_t test_point, const point_t *sensor_positions, int sensor_num);

#define calc_test_distances_bm(buffer, test_point, sensor_positions, sensor_num) ({ \
	for (int i = 0; i < sensor_num; i++) { \
		buffer[i] = point_distanceto_point_m(&test_point, &sensor_positions[i]); \
	} })
void calc_test_distances_b(ftype* buffer, point_t test_point, const point_t *sensor_positions, int sensor_num);


struct ft_buffer {
	point_t _inter_rv[2], intersections[_max_intersections_g];
	circle_t circles[_sensor_num_g];
}; 

point_t find_target(struct ft_buffer *buffer, int sensor_num, circle_t* magnet_projection);


#define to_radian(degree) ((double)degree / 180 * M_PI)
#define to_degree(radian) ((double)radian / M_PI * 180)


#ifdef ENCODER_IMPL
#include "impl/encoder-impl.h"
#endif

#endif
