import random

from _matrix import Matrix

MIN_SIZE = 1
MAX_SIZE = 10


def random_matrix(nrow=0, ncol=0, sz_min=MIN_SIZE, sz_max=MAX_SIZE):
    nrow = nrow if nrow != 0 else random.randint(sz_min, sz_max)
    ncol = ncol if ncol != 0 else random.randint(sz_min, sz_max)
    return Matrix(nrow, ncol), nrow, ncol


def test_matrix_creation():
    try:
        _, _, _ = random_matrix()
        assert True
    except:
        assert False


def test_empty_matrix():
    try:
        _, _, _ = random_matrix(sz_min=0, sz_max=0)
        assert True
    except:
        assert False


def test_scalar_matrix():
    try:
        _, _, _ = random_matrix(sz_min=1, sz_max=1)
        assert True
    except:
        assert False


def test_getitem():
    mat, nrow, ncol = random_matrix()
    for i in range(nrow):
        for j in range(ncol):
            _ = mat[i, j]


def test_setitem():
    mat, nrow, ncol = random_matrix()
    for i in range(nrow):
        for j in range(ncol):
            mat[i, j] = i * j
            assert mat[i, j] == i * j


def test_getter():
    mat, nrow, ncol = random_matrix()
    assert nrow == mat.nrow
    assert ncol == mat.ncol


def test_equal_comparator():
    mat1, nrow, ncol = random_matrix()
    mat2, nrow, ncol = random_matrix(nrow=nrow, ncol=ncol)

    for i in range(nrow):
        for j in range(ncol):
            mat1[i, j] = i * j
            mat2[i, j] = i * j

    assert mat1 == mat2


def test_inequal_comparator():
    mat1, nrow, ncol = random_matrix(sz_min=2)
    mat2, nrow, ncol = random_matrix(nrow=nrow, ncol=ncol)

    for i in range(nrow):
        for j in range(ncol):
            mat1[i, j] = i + j
            mat2[i, j] = i - j

    assert mat1 != mat2


def test_copy_constructor():
    mat1, _, _ = random_matrix()
    mat2 = Matrix(mat1)
    assert mat1 == mat2
