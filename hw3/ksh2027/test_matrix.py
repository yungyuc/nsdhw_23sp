import pytest
import _matrix
import numpy as np

def build_matrix(row_size, col_size):
    mat_test = _matrix.Matrix(row_size, col_size)
    mat_rdm = np.random.rand(row_size, col_size)
    
    for it in range(row_size):
        for jt in range(col_size):
            mat_test[it, jt] = mat_rdm[it, jt]
            
    return mat_test, mat_rdm

def test_mat_class():
    row_size = 85
    col_size = 47
    A_test, A_rdm = build_matrix(row_size, col_size)
    B_test = _matrix.Matrix(row_size, col_size)
    for it in range(row_size):
        for jt in range(col_size):
            B_test[it, jt] = A_test[it, jt]
    C_test, C_rdm = build_matrix(10, 10)
    
    assert A_test.nrow == row_size
    assert A_test.ncol == col_size
    assert A_test == B_test
    assert A_test != C_test
    
def test_multiply_naive():
    row_size = 85
    col_size = 47
    A_test, A_rdm = build_matrix(row_size, col_size)
    B_test, B_rdm = build_matrix(col_size, row_size)
    C_test, C_rdm = build_matrix(row_size, col_size)
    
    try:
        _matrix.multiply_naive(A_test, C_test)
    except IndexError as msg:
        assert str(msg) == "the number of first matrix column differs from that of second matrix row"
    
    expected_rst = np.dot(A_rdm, B_rdm)
    test_rst = _matrix.multiply_naive(A_test, B_test)
    for it in range(row_size):
        for jt in range(col_size):
            assert abs(test_rst[it, jt] - expected_rst[it, jt]) < 1e-4
            
def test_multiply_tile():
    row_size = 85
    col_size = 47
    A_test, A_rdm = build_matrix(row_size, col_size)
    B_test, B_rdm = build_matrix(col_size, row_size)
    C_test, C_rdm = build_matrix(row_size, col_size)
    
    try:
        _matrix.multiply_tile(A_test, C_test, 10)
    except IndexError as msg:
        assert str(msg) == "the number of first matrix column differs from that of second matrix row"
    
    expected_rst = np.dot(A_rdm, B_rdm)
    test_rst = _matrix.multiply_tile(A_test, B_test, 10)
    for it in range(row_size):
        for jt in range(col_size):
            assert abs(test_rst[it, jt] - expected_rst[it, jt]) < 1e-4
        