import random

from _matrix import Matrix
from _matrix import multiply_naive, multiply_tile, multiply_mkl


MAX_SIZE = 10


def test_multiply_naive():
    m, n, p = random.sample(range(1, MAX_SIZE), 3)
    mat1 = Matrix(m, n)
    mat2 = Matrix(n, p)
    mat = multiply_naive(mat1, mat2)


def test_multiply_naive_special():
    m, n, p = 2, 2, 2
    mat1 = Matrix(m, n)
    mat2 = Matrix(n, p)
    mat1[0, 0], mat1[0, 1], mat1[1, 0], mat1[1, 1] = 0, 1, 2, 3
    mat2[0, 0], mat2[0, 1], mat2[1, 0], mat2[1, 1] = 4, 5, 6, 7

    mat = multiply_naive(mat1, mat2)
    assert mat.nrow == 2
    assert mat.ncol == 2
    assert mat[0, 0] == 6 and mat[0, 1] == 7 and mat[1, 0] == 26 and mat[1, 1] == 31


def test_multiply_tile():
    m, n, p = random.sample(range(1, MAX_SIZE), 3)
    mat1 = Matrix(m, n)
    mat2 = Matrix(n, p)
    tsize = 16
    mat = multiply_tile(mat1, mat2, tsize)


def test_multiply_mkl():
    m, n, p = random.sample(range(1, MAX_SIZE), 3)
    mat1 = Matrix(m, n)
    mat2 = Matrix(n, p)
    mat = multiply_mkl(mat1, mat2)
