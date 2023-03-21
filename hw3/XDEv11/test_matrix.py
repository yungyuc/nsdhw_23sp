import random
import time

import _matrix

def test_matrix():
	f = open("performance.txt", "a")

	size = [50, 500, 1000, 2000];
	for sz in size:
		s = '\nsz: ' + str(sz);
		print(s); f.write(s + '\n');

		mat1 = _matrix.Matrix(sz, sz);
		for i in range(0, sz):
			for j in range(0, sz):
				mat1[i, j] = random.random();

		mat2 = _matrix.Matrix(sz, sz);
		for i in range(0, sz):
			for j in range(0, sz):
				mat1[i, j] = random.random();

		s = 'multiply_mkl: ';
		print(s, end=''); f.write(s);
		start = time.time();
		golden = _matrix.multiply_mkl(mat1, mat2);
		end = time.time();
		s = str(end - start);
		print(s); f.write(s + '\n');

		s = 'multiply_naive: ';
		print(s, end=''); f.write(s);
		start = time.time()
		mat3 = _matrix.multiply_naive(mat1, mat2);
		end = time.time();
		s = str(end - start);
		print(s); f.write(s + '\n');
		assert mat3 == golden;

		s = 'multiply_naive_better_order: ';
		print(s, end=''); f.write(s);
		start = time.time()
		mat3 = _matrix.multiply_naive_better_order(mat1, mat2);
		end = time.time();
		s = str(end - start);
		print(s); f.write(s + '\n');
		assert mat3 == golden;

		s = 'multiply_tile: ';
		print(s, end=''); f.write(s);
		start = time.time()
		mat3 = _matrix.multiply_tile(mat1, mat2, 32);
		end = time.time()
		s = str(end - start);
		print(s); f.write(s + '\n');
		assert mat3 == golden;

		s = 'multiply_tile_better_order: ';
		print(s, end=''); f.write(s);
		start = time.time()
		mat3 = _matrix.multiply_tile_better_order(mat1, mat2, 32);
		end = time.time()
		s = str(end - start);
		print(s); f.write(s + '\n');
		assert mat3 == golden;

	f.close()

if __name__ == '__main__':
	test_matrix();
	
