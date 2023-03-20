import random

from _matrix import Matrix
from _matrix import multiply_naive, multiply_tile, multiply_mkl

MIN_SIZE = 1
MAX_SIZE = 10


def random_matrix(nrow=0, ncol=0, sz_min=MIN_SIZE, sz_max=MAX_SIZE):
    nrow = nrow if nrow != 0 else random.randint(sz_min, sz_max)
    ncol = ncol if ncol != 0 else random.randint(sz_min, sz_max)
    return Matrix(nrow, ncol), nrow, ncol


def test_multiply_naive():
    mat1, _, ncol = random_matrix()
    mat2, _, _ = random_matrix(nrow=ncol)
    try:
        _ = multiply_naive(mat1, mat2)
        assert True
    except:
        assert False


def test_multiply_naive_empty():
    mat1 = Matrix(0, 0)
    mat2 = Matrix(0, 0)

    try:
        _ = multiply_naive(mat1, mat2)
        assert True
    except:
        assert False


def test_multiply_naive_scalar():
    mat1 = Matrix(1, 1)
    mat2 = Matrix(1, 1)

    a, b = random.choices(range(MAX_SIZE), k=2)
    mat1[0, 0], mat2[0, 0] = a, b

    mat = multiply_naive(mat1, mat2)
    assert mat[0, 0] == a * b


def test_multiply_naive_special():
    m, n, p = 2, 2, 2
    mat1 = Matrix(m, n)
    mat2 = Matrix(n, p)
    mat1[0, 0], mat1[0, 1], mat1[1, 0], mat1[1, 1] = 0, 1, 2, 3
    mat2[0, 0], mat2[0, 1], mat2[1, 0], mat2[1, 1] = 4, 5, 6, 7

    mat = multiply_naive(mat1, mat2)
    assert mat.nrow == 2
    assert mat.ncol == 2
    assert mat[0, 0] == 6 and mat[0,
                                  1] == 7 and mat[1, 0] == 26 and mat[1, 1] == 31


def test_multiply_tile():
    mat1, _, ncol = random_matrix()
    mat2, _, _ = random_matrix(nrow=ncol)
    tsize = 16
    try:
        _ = multiply_tile(mat1, mat2, tsize)
        assert True
    except:
        assert False


def test_multiply_tile_special():
    size = 100
    mat1 = Matrix(size, size)
    mat2 = Matrix(size, size)

    for i in range(size):
        for j in range(size):
            mat1[i, j] = i * size + j + 1
            mat2[i, j] = i * size + j + 1

    tsize = 16
    mat_naive = multiply_naive(mat1, mat2)
    mat_tile = multiply_tile(mat1, mat2, tsize)
    assert mat_tile == mat_naive


def test_multiply_mkl():
    mat1, _, ncol = random_matrix()
    mat2, _, _ = random_matrix(nrow=ncol)
    try:
        _ = multiply_mkl(mat1, mat2)
        assert True
    except:
        assert False


def test_multiply_mkl_special():
    size = 30
    mat1 = Matrix(size, size)
    mat2 = Matrix(size, size)

    for i in range(size):
        for j in range(size):
            mat1[i, j] = i * size + j + 1
            mat2[i, j] = i * size + j + 1

    mat_naive = multiply_naive(mat1, mat2)
    mat_mkl = multiply_mkl(mat1, mat2)
    assert mat_mkl == mat_naive
