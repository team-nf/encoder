"""
This file is part of ncoder.
Copyright (C) 2022 Tuna Gül, Yüşa Furkan Erenci
"""
# SPDX-License-Identifier: GPL-3.0-or-later

from math import *

_sensor_num_g = 3

class sensor_data_t:
	def __init__(self, _min, _normal, _max):
		self._normal = _normal
		self._min = _min
		self._max = _max
	

class position_t:
	def __init__(self, x, y):
		self.x = x
		self.y = y

	def __repr__(self): return f"({self.x:.6f}, {self.y:.6f})"


def _calc_sensor_angles(sensor_num, radians=True):
	if radians: return [(2*pi / sensor_num) * i for i in range(sensor_num)]
	else: return [(360 / sensor_num) * i for i in range(sensor_num)]

def calc_sensor_positions(sensor_num):
	angles = _calc_sensor_angles(sensor_num)
	return [position_t(cos(angles[i]), sin(angles[i])) for i in range(sensor_num)]



def _calc_samex_sensors2(sensor_num):
	angles = _calc_sensor_angles(sensor_num, radians=False)
	
	tolerance = 0.000000000001
	i = 1
	samex = []
	while i < sensor_num:
		j = sensor_num - i
		while j > i:
			# print(f"i: {i}, j: {j}, angle: {angles[i]}, rangle: {angles[j]}")
			if 360 - tolerance < angles[i] + angles[j] < 360 + tolerance: 
				samex.append((i, j))
				break
			j = j - 1
		i=i+1
	return samex


def _calc_samex_sensors(sensor_num):
	angles = _calc_sensor_angles(sensor_num, radians=False)
	rangles = [360 - angle for angle in angles]

	samex = []
	for i, angle in enumerate(angles):
		if angle == 180: continue
		if i in [j[1] for j in samex]: continue
		if angle in rangles:
			samex.append((i, rangles.index(angle)))
	return samex

def _calc_intersec_y(s1_dist, s2_dist, s1_posy, s2_posy):
	# çemberlerin merkezlerinin apsisi aynı olmalı
	return (s1_dist**2 - s2_dist**2 + s2_posy**2 - s1_posy**2) / (2 * (s2_posy-s1_posy))

def calc_intersec_ys(distance_to_sensors, sensor_positions, samex):
	if len(sensor_positions) != len(distance_to_sensors): return Exception("Internal Failure")
	sensor_num, dists, positions = len(sensor_positions), distance_to_sensors, sensor_positions

	return [_calc_intersec_y(dists[i], dists[j], \
			positions[i].y, positions[j].y) for i, j in samex]

def _calc_intersec_x(target_y, s1_dist, s2_dist, s1_pos, s2_pos):
	return (-s1_pos.x**2 + s2_pos.x**2 + (target_y - s2_pos.y)**2 - (target_y - s1_pos.y)**2 + s1_dist**2 - s2_dist**2) / (2 * (s2_pos.x - s1_pos.x))

def calc_intersec_xs(target_ys, distance_to_sensors, sensor_positions, samex):
	#	 0 ve 1. sensörün apsisleri her zaman farklı olmak zorunda 
	# olduğundan direkt o ikisini kullanarak hesaplama yapabiliriz
	return [_calc_intersec_x(y, distance_to_sensors[0], distance_to_sensors[1], \
			sensor_positions[0], sensor_positions[1]) for y in target_ys]


# bu fonksiyonun nasıl geliştirileceği testlerde belirlenecek
def _calc_distance_to_sensor(sensor_reading):
	return NotImplemented


def calc_distance_to_sensors(sensor_readings):
	return [_calc_distance_to_sensor(i) for i in sensor_readings]


def calc_distance_to_sensors_test(sensor_positions, target_pos):
	return [sqrt((spos.x - target_pos.x)**2 + (spos.y - target_pos.y)**2)
			for spos in sensor_positions]


def calc_target_pos(distances, positions, samex):
	target_ys = calc_intersec_ys(distances, positions, samex)
	target_xs = calc_intersec_xs(target_ys, distances, positions, samex)
	
	return position_t(sum(target_xs) / len(target_xs), sum(target_ys) / len(target_ys))


def main():
	positions = calc_sensor_positions(_sensor_num_g)
	samex = _calc_samex_sensors2(_sensor_num_g)

	tolerance = 0.0000000000001
	for angle in range(360):
		test_angle = radians(angle)
		test_target = position_t(cos(test_angle), sin(test_angle))
		distances = calc_distance_to_sensors_test(positions, test_target)

		calculated = calc_target_pos(distances, positions, samex)
		if not test_target.x - tolerance < calculated.x < test_target.x + tolerance: print("error, ", angle)
		elif not test_target.y - tolerance < calculated.y < test_target.y + tolerance: print("error, ", angle)
		# print("created: ", test_target.x, test_target.y)
		# print("calculated: ", calculated.x, calculated.y)


if __name__ == "__main__":
	print("Started.")
	main()
	print("Done.")

