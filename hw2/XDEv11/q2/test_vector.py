import _vector

import math

def test_angle():
	def dot_product(v1, v2):
		return v1[0] * v2[0] + v1[1] * v2[1];

	inputs = [((1, 0), (1, 0)), ((1, 0), (0, 1)), ((-1, -0.1), (-2, 1.5))];
	for v1, v2 in inputs:
		a = _vector.angle(v1, v2);
		print(v1, v2, a);
		assert a \
			== math.acos(dot_product(v1, v2) / (math.sqrt(dot_product(v1, v1)) * math.sqrt(dot_product(v2, v2))));

if __name__ == '__main__':
	print(_vector.angle((1, 2), (2, 1)));
	test_angle();
	
