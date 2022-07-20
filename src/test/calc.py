from math import *

_sensor_num_g = 5

class sensor_data_t:
	def __init__(self, _min, _normal, _max):
		self._normal = _normal
		self._min = _min
		self._max = _max
	
	def __str__(self): return f"({self._min}, {self._normal}, {self._max})"
	def __repr__(self): return f"({self._min}, {self._normal}, {self._max})"


class position_t:
	def __init__(self, x, y):
		self.x = x
		self.y = y
	
	def __str__(self): return f"({self.x:.2f}, {self.y:.2f})"
	def __repr__(self): return f"({self.x:.2f}, {self.y:.2f})"



def _calc_sensor_angles(sensor_num, radians=True):
	if radians: return [(2*pi / sensor_num) * i for i in range(sensor_num)]
	else: return [(360 / sensor_num) * i for i in range(sensor_num)]


def calc_sensor_positions(sensor_num):
	angles = _calc_sensor_angles(sensor_num)
	return [position_t(cos(angles[i]), sin(angles[i])) for i in range(sensor_num)]



def calc_intersec_ys(sensor_positions, distance_to_sensors): 
	if len(sensor_positions) != len(distance_to_sensors): return Exception("Internal Failure")
	sensor_num, dists, positions = len(sensor_positions), distance_to_sensors, sensor_positions

	return [_calc_circle_intersec_y(dists[i], dists[j], positions[i].y, positions[j].y)
			for i, j in _calc_samex_sensors(sensor_num)]

# 	ys = []
# 	samex = _calc_samex_sensors(sensor_num)
# 	for i, j in samex:
# 		ys.append(_calc_circle_intersec_y(dists[i], dists[j], positions[i].y, positions[j].y))
#	return ys


def _calc_circle_intersec_y(c1_dist, c2_dist, c1_posy, c2_posy):
	# çemberlerin merkezlerinin apsisi aynı olmalı
	return (c1_dist**2 - c2_dist**2 + c2_posy**2 - c1_posy**2) / (2 * (c2_posy-c1_posy))


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


def __polygon_samex_test(_start=3, _end=1000):
	# işte bu
	for sensor_num in range(3, 1000):
		angles = [(360 / sensor_num) * i for i in range(sensor_num)]
		rangles = [360 - angle for angle in angles]

		saved_indexes = []
		for i, angle in enumerate(angles):
			if angle == 180: continue
			if angle in rangles:
				saved_indexes.append(i)
		if len(saved_indexes) == 0: print(f"ERROR at sensor {sensor_num}")
		if sensor_num % 100 == 0: print(f"at sensor: {sensor_num}")


# bu fonksiyonun nasıl geliştirileceği testlerde belirlenecek
def _calc_distance_to_sensor(sensor_reading):
	return NotImplemented


# bu fonksiyonun nasıl geliştirileceği testlerde belirlenecek
def calc_distance_to_sensors(sensor_readings):
	return [_calc_distance_to_sensor(i) for i in sensor_readings]


def calc_distance_to_sensors_test(sensor_positions, target_pos):
	return [sqrt((spos.x - target_pos.x)**2 + (spos.y - target_pos.y)**2)
			for spos in sensor_positions]
		


def main():
	test_target = position_t(1, 1)
	positions = calc_sensor_positions(_sensor_num_g)
	distances = calc_distance_to_sensors_test(positions, test_target)
	print(calc_intersec_ys(positions, distances))

if __name__ == "__main__":
	main()
