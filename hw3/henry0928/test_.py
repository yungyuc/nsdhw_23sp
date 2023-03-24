import _matrix
import math
import numpy as np
import pytest
import timeit


def make_matrices(size):
    mat1 = _matrix.Matrix(size, size)
    mat2 = _matrix.Matrix(size, size)
    mat3 = _matrix.Matrix(size, size)

    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt 
            mat2[it, jt] = it * size + jt
            mat3[it, jt] = 0
    return mat1, mat2, mat3

def test_basic():
    size = 100
    mat1, mat2, mat3 = make_matrices(size)

    assert size == mat1.nrow
    assert size == mat2.ncol
    assert size == mat3.nrow
    assert size == mat3.ncol

    assert mat1[0, 1] == 1
    assert mat1[1, 0] == 100 
    assert mat1[1, size-1] == 199
    assert mat1[size-1, size-1] == size*size -1 

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            assert mat1[i, j] == mat2[i, j]
            assert mat3[i, j] == 0

    assert _matrix.matrix_equal(mat1, mat2) == True 
    assert _matrix.matrix_equal(mat1, mat3) == False 

def test_zero():
    size = 100
    mat1, mat2, mat3= make_matrices(size)

    ret_naive = _matrix.multiply_naive(mat1, mat3)

    assert size == ret_naive.nrow
    assert size == ret_naive.ncol

    for i in range(ret_naive.nrow):
        for j in range(ret_naive.ncol):
            assert 0 == ret_naive[i, j]

def test_naive():
    mat1, mat2, mat3 = make_matrices(8)
    naive12 = _matrix.multiply_naive(mat1, mat2)
    naive23 = _matrix.multiply_naive(mat2, mat3)
    mkl12 = _matrix.multiply_mkl(mat1, mat2)
    mkl23 = _matrix.multiply_mkl(mat2, mat3)
    assert _matrix.matrix_equal(naive12, mkl12) == True
    assert _matrix.matrix_equal(naive23, mkl23) == True

def check_tile(mat1, mat2, tsize):
    ret_naive = _matrix.multiply_naive(mat1, mat2)
    ret_tile = _matrix.multiply_tile(mat1, mat2, tsize)
    ret_mkl = _matrix.multiply_mkl(mat1, mat2)
    assert _matrix.matrix_equal(ret_mkl, ret_tile) == True 
    assert _matrix.matrix_equal(ret_naive, ret_tile) == True
    ns = dict(_matrix=_matrix, mat1=mat1, mat2=mat2, tsize=tsize)
    t_naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', globals=ns)
    t_tile = timeit.Timer("_matrix.multiply_tile(mat1, mat2, tsize)", globals=ns)
    time_naive = min(t_naive.repeat(10, 1))
    time_tile = min(t_tile.repeat(10, 1))
    ratio = time_tile / time_naive
    return ratio

def test_tile():
    mat1, mat2, mat3 = make_matrices(1000)
    ratio16 = check_tile(mat1, mat2, 16)
    print("tile 16 ratio:", ratio16)
    assert ratio16 <= 0.8
    ratio18 = check_tile(mat1, mat2, 18)
    print("tile 18 ratio:", ratio18)
    assert ratio18 <= 0.8
    ratio20 = check_tile(mat1, mat2, 20)
    print("tile 20 ratio:", ratio20)
    assert ratio20 <= 0.8
    ratio32 = check_tile(mat1, mat2, 32)
    print("tile 32 ratio:", ratio32)
    assert ratio32 <= 0.8