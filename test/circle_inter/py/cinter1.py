from math import *

_sensor_num_g = 3
tolerance = 0.0000000000001


class point_t:
	def __init__(self, x, y):
		self.x = x
		self.y = y

	def find_distance(self, point):
		return sqrt((self.x - point.x)**2 + (self.y - point.y)**2)

	def is_on_circle(self, circle):
		return check_wt(circle.radius, circle.center.find_distance(self))

	def is_on_line(self, line):
		return check_wt(self.y, line.m * self.x + line.n)

	def __repr__(self): 
		return f"({self.x:.3f}, {self.y:.3f})"

class line_t:
	def __init__(self, m, n):
		self.m = m
		self.n = n
	

class circle_t:
	def __init__(self, center: point_t, radius):
		self.center = center
		self.radius = radius
	
	def intersect(self, obj):
		if type(obj) == line_t: return self.intersect_line(obj)
		elif type(obj) == circle_t: return self.intersect_circle(obj)
		else: raise Exception(f"No intersection formula found for type {type(obj)} with type {type(self)}")

	def intersect_line(self, line: line_t):
		if self.radius == 0: return [self.center] if self.center.is_on_line(line) else []

		a = line.m**2 + 1
		b = 2 * (line.m * (line.n - self.center.y) - self.center.x)
		c = (line.n - self.center.y)**2 + self.center.x**2 - self.radius**2
		return [point_t(x, line.m*x+line.n) for x in find_roots(a, b, c)]

	def intersect_circle(self, circle):
		if self.radius == 0: return [self.center] if self.center.is_on_circle(circle) else []
		if circle.radius == 0: return [circle.center] if circle.center.is_on_circle(self) else []

		m = (self.center.x - circle.center.x) / (circle.center.y - self.center.y)
		n = (self.radius**2 + circle.center.x**2 + circle.center.y**2 \
			-circle.radius**2 - self.center.x**2 - self.center.y**2) /\
			(2*(circle.center.y - self.center.y))
		return self.intersect_line(line_t(m, n))

def check_wt(a, b):
	return a - tolerance < b < a + tolerance

def find_roots(a, b, c):
	delta = b**2 - 4*a*c
	if delta < 0: return []
	elif delta == 0: return [-b / 2*a]
	elif delta > 0: 
		return [(-b - sqrt(delta)) / (2*a), (-b + sqrt(delta)) / (2*a)]

def _calc_sensor_angles(sensor_num, radians=True):
	if radians: return [(2*pi / sensor_num) * i for i in range(sensor_num)]
	else: return [(360 / sensor_num) * i for i in range(sensor_num)]

def calc_sensor_positions(sensor_num):
	angles = _calc_sensor_angles(sensor_num)
	return [point_t(cos(angles[i]), sin(angles[i])) for i in range(sensor_num)]

def calc_target(distances, positions):
	circles = [circle_t(position, distance) for position, distance in zip(positions, distances)]
	inters = circles[0].intersect(circles[1])
	for point in inters:
		if point.is_on_circle(circles[2]):
			return point

def calc_distance_to_sensors_test(sensor_positions, target_pos):
	return [spos.find_distance(target_pos) for spos in sensor_positions]

def main():
	# For once
	positions = calc_sensor_positions(_sensor_num_g)

	# For each loop 
	#test_angle = radians(angle)
	#test_target = point_t(cos(test_angle), sin(test_angle))
	test_target = point_t(1, 1)
	distances = calc_distance_to_sensors_test(positions, test_target)

	target = calc_target(distances, positions)
	errors = []
	try:
		if   not check_wt(target.x, test_target.x): errors.append(angle)
		elif not check_wt(target.y, test_target.y): errors.append(angle)
	except:
		errors.append(angle)

	print(f"Done with {len(errors)} errors.")
	if len(errors) > 0: print(errors)

if __name__ == "__main__":
	main()
