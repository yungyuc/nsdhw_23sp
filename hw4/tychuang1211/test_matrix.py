import timeit

import numpy as np
import pytest

import _matrix

@pytest.fixture(scope="session", params=[32, 512], ids=["32_matrix", "512_matrix"])
def generate_matrix(request):
	size = request.param
	mat1 = _matrix.Matrix(size, size)
	mat2 = _matrix.Matrix(size, size)

	for it in range(size):
		for jt in range(size):
			mat1[it, jt] = it * size + jt + 1
			mat2[it, jt] = it * size + jt + 1

	return mat1, mat2, size

def test_naive_multiplication(generate_matrix):
	mat1, mat2, size = generate_matrix
	naive = _matrix.multiply_naive(mat1, mat2)
	mkl = _matrix.multiply_mkl(mat1, mat2)
	for i in range(size):
		for j in range(size):
			assert naive[i, j] == pytest.approx(mkl[i, j])

@pytest.mark.parametrize("tsize", [8, 16, 32])
def test_tile_multiplication(generate_matrix, tsize):
	mat1, mat2, size = generate_matrix
	tile = _matrix.multiply_tile(mat1, mat2, tsize)
	mkl = _matrix.multiply_mkl(mat1, mat2)
	for i in range(size):
		for j in range(size):
			assert tile[i, j] == pytest.approx(mkl[i, j])

def test_perfromance():
	setup = '''
import _matrix

size = 1000

mat1 = _matrix.Matrix(size,size)
mat2 = _matrix.Matrix(size,size)

for it in range(size):
    for jt in range(size):
        mat1[it, jt] = it * size + jt + 1
        mat2[it, jt] = it * size + jt + 1
'''
	naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', setup=setup)
	mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', setup=setup)

	repeat = 5
	
	with open('performance.txt', 'w') as fobj:

		w = fobj
		
		w.write(f'Start multiply_naive (repeat={repeat}), take min = ')
		naivesec = minsec = min(naive.repeat(repeat=repeat, number=1))
		w.write(f'{minsec} seconds\n')

		tsizes = [16, 32]
		for tsize in tsizes:
			tile = timeit.Timer(f'_matrix.multiply_tile(mat1, mat2, {tsize})', setup=setup)
			w.write(f'Start multiply_tile (tsize={tsize}, repeat={repeat}), take min = ')
			tilesec = minsec = min(tile.repeat(repeat=repeat, number=1))
			w.write(f'{tilesec} seconds\n')
			w.write('Tilesize %d speed-up over naive: %g x\n' % (tsize, naivesec/tilesec))

		w.write(f'Start multiply_mkl (repeat={repeat}), take min = ')
		mklsec = minsec = min(mkl.repeat(repeat=repeat, number=1))
		w.write(f'{minsec} seconds\n')

		w.write('MKL speed-up over naive: %g x\n' % (naivesec/mklsec))