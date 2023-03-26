import numpy as np
import pytest
import timeit
import unittest
from _matrix import Matrix, multiply_naive, multiply_tile, multiply_mkl


mat_size = 1000
tile_size = 16
repeat = 5

def gen_test_matrices(size):

    mat1 = Matrix(size, size)
    mat2 = Matrix(size, size)
    mat3 = Matrix(size, size)

    for i in range(size):
        for j in range(size):
            mat1[i, j] = i+j
            mat2[i, j] = i+j
            mat3[i, j] = 0

    return mat1, mat2, mat3

def assert_matrix_equal(mat1, mat2):

    assert mat1.nrow == mat2.nrow
    assert mat1.ncol == mat2.ncol

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            assert mat1[i, j] == mat2[i, j]


def test_matrix_build():

    mat1, mat2, _ = gen_test_matrices(mat_size)
    
    assert mat_size == mat1.nrow
    assert mat_size == mat1.ncol
    assert mat_size == mat2.nrow
    assert mat_size == mat2.ncol

    assert_matrix_equal(mat1, mat2)

def test_general():

    mat1, mat2, _ = gen_test_matrices(mat_size)

    naive = multiply_naive(mat1, mat2)
    tile = multiply_tile(mat1, mat2, tile_size)
    mkl = multiply_mkl(mat1, mat2)

    assert_matrix_equal(naive, tile)
    assert_matrix_equal(naive, mkl)
    assert_matrix_equal(mkl, tile)

def test_zero():

    mat1, _, mat2 = gen_test_matrices(mat_size)

    naive = multiply_naive(mat1, mat2)
    tile = multiply_tile(mat1, mat2, tile_size)
    mkl = multiply_mkl(mat1, mat2)

    assert_matrix_equal(naive, mat2)
    assert_matrix_equal(tile, mat2)
    assert_matrix_equal(mkl, mat2)

def test_tile():

    mat1, mat2, _ = gen_test_matrices(500)
    ns = dict(multiply_naive=multiply_naive, multiply_tile=multiply_tile, mat1=mat1, mat2=mat2)

    t_tile_0 = timeit.Timer("multiply_naive(mat1, mat2)", globals=ns)
    t_tile_16 = timeit.Timer("multiply_tile(mat1, mat2, 16)", globals=ns)
    t_tile_17 = timeit.Timer("multiply_tile(mat1, mat2, 17)", globals=ns)
    t_tile_19 = timeit.Timer("multiply_tile(mat1, mat2, 19)", globals=ns)

    t0 = min(t_tile_0.repeat(5, 1))
    t16 = min(t_tile_16.repeat(5, 1))
    t17 = min(t_tile_17.repeat(5, 1))
    t19 = min(t_tile_19.repeat(5, 1))

    assert t16/t0 <= 0.8
    assert t17/t0 <= 0.8
    assert t19/t0 <= 0.8
    

def test_benchmark():

    mat1, mat2, _ = gen_test_matrices(mat_size)
    ns = dict(multiply_naive=multiply_naive, multiply_tile=multiply_tile, multiply_mkl=multiply_mkl,
                mat1=mat1, mat2=mat2, tile_size=tile_size)

    naive = timeit.Timer('multiply_naive(mat1, mat2)', globals=ns)
    tile = timeit.Timer('multiply_tile(mat1, mat2, tile_size)', globals=ns)
    mkl = timeit.Timer('multiply_mkl(mat1, mat2)', globals=ns)

    with open('performance.txt', 'w') as outfile:
        naive_time = min(naive.repeat(repeat=repeat, number=1))
        tile_time = min(tile.repeat(repeat=repeat, number=1))
        mkl_time = min(mkl.repeat(repeat=repeat, number=1))
        tile_over_naive = naive_time/tile_time
        mkl_over_naive = naive_time/mkl_time

        outfile.write('Multiplication report of two 1000x1000 matrices (repeat {} times):\n'.format(repeat))
        outfile.write('Naive method: {} sec\n'.format(naive_time))
        outfile.write('Tile method(tile size=16): {} sec\n'.format(tile_time))
        outfile.write('mkl method: {} sec\n\n'.format(mkl_time))

        outfile.write('Tile speed-up over naive:{} x\n'.format(tile_over_naive))
        outfile.write('mkl speed-up over naive:{} x\n'.format(mkl_over_naive))

if __name__ == '__main__':
    test_benchmark()
