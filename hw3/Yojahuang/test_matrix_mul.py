# The python module that wraps the matrix code.
import _matrix
import pytest

def test_matrix_row_and_col():
    row = 1
    col = 2
    mat = _matrix.Matrix(row, col)

    assert mat.nrow == row
    assert mat.ncol == col

def test_equal_and_not_equal():
    mat1 = _matrix.Matrix(1, 1) 
    mat2 = _matrix.Matrix(1, 1) 
    mat3 = _matrix.Matrix(1, 1) 
    mat1[0, 0] = 1
    mat2[0, 0] = 1
    mat3[0, 0] = 2
    assert mat1 == mat2
    assert mat1 != mat3

def test_matrix_mul():
    size = 500
    mat1 = _matrix.Matrix(size,size)
    mat2 = _matrix.Matrix(size,size)

    for i in range(size):
        for j in range(size):
            mat1[i, j] = 2 * (i ** 2) + j + 0.5
            mat2[i, j] = 2 * (j ** 2) + i + 0.5

    ret_naive = _matrix.multiply_naive(mat1, mat2)
    ret_tile = _matrix.multiply_tile(mat1, mat2, 16)
    ret_mkl = _matrix.multiply_mkl(mat1, mat2)

    assert ret_naive == ret_tile
    assert ret_naive == ret_mkl

