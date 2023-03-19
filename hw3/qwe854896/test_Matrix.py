import random

from _matrix import Matrix


MAX_SIZE = 10


def test_custom_constructor():
    nrow, ncol = random.sample(range(1, MAX_SIZE), 2)
    mat = Matrix(nrow, ncol)


def test_getitem():
    nrow, ncol = random.sample(range(1, MAX_SIZE), 2)
    mat = Matrix(nrow, ncol)
    for i in range(nrow):
        for j in range(ncol):
            _ = mat[i, j]


def test_setitem():
    nrow, ncol = random.sample(range(1, MAX_SIZE), 2)
    mat = Matrix(nrow, ncol)
    for i in range(nrow):
        for j in range(ncol):
            mat[i, j] = i * j
            assert mat[i, j] == i * j


def test_getter():
    nrow, ncol = random.sample(range(1, MAX_SIZE), 2)
    mat = Matrix(nrow, ncol)

    assert nrow == mat.nrow
    assert ncol == mat.ncol


def test_equal_comparator():
    nrow, ncol = random.sample(range(1, MAX_SIZE), 2)
    mat1 = Matrix(nrow, ncol)
    mat2 = Matrix(nrow, ncol)

    for i in range(nrow):
        for j in range(ncol):
            mat1[i, j] = i * j
            mat2[i, j] = i * j

    assert mat1 == mat2


def test_inequal_comparator():
    nrow, ncol = random.sample(range(1, MAX_SIZE), 2)
    mat1 = Matrix(nrow, ncol)
    mat2 = Matrix(nrow, ncol)

    for i in range(nrow):
        for j in range(ncol):
            mat1[i, j] = i + j
            mat2[i, j] = i - j

    assert mat1 != mat2


def test_copy_constructor():
    nrow, ncol = random.sample(range(1, MAX_SIZE), 2)
    mat1 = Matrix(nrow, ncol)
    mat2 = mat1
    assert mat1 == mat2
