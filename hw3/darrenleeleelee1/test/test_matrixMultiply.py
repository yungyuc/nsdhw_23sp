import random
import numpy as np
import _matrix

    
def test_matrix_multiply_naive():
    min_matrix_size = 1
    max_matrix_size = 100
    
    nrow_A = random.randint(min_matrix_size, max_matrix_size)
    ncol_A = random.randint(min_matrix_size, max_matrix_size)

    np_matrix_A = np.random.rand(nrow_A, ncol_A)

    test_matrix_A = _matrix.Matrix(nrow_A, ncol_A)
    for r in range(nrow_A):
        for c in range(ncol_A):
            test_matrix_A[r, c] = np_matrix_A[r, c]
            
    nrow_B = ncol_A
    ncol_B = random.randint(min_matrix_size, max_matrix_size)

    np_matrix_B = np.random.rand(nrow_B, ncol_B)

    test_matrix_B = _matrix.Matrix(nrow_B, ncol_B)
    for r in range(nrow_B):
        for c in range(ncol_B):
            test_matrix_B[r, c] = np_matrix_B[r, c]

    np_matrix_result = np.dot(np_matrix_A, np_matrix_B)
    
    navie_matrix_result = _matrix.Matrix(nrow_A, ncol_B)
    navie_matrix_result = _matrix.multiply_naive(test_matrix_A, test_matrix_B)
        
    for r in range(nrow_A):
        for c in range(ncol_B):
            assert np.abs(np_matrix_result[r, c] - navie_matrix_result[r, c]) < 1e-6

def test_matrixmultiply_tile():
    min_matrix_size = 1
    max_matrix_size = 100
    
    nrow_A = random.randint(min_matrix_size, max_matrix_size)
    ncol_A = random.randint(min_matrix_size, max_matrix_size)

    np_matrix_A = np.random.rand(nrow_A, ncol_A)

    test_matrix_A = _matrix.Matrix(nrow_A, ncol_A)
    for r in range(nrow_A):
        for c in range(ncol_A):
            test_matrix_A[r, c] = np_matrix_A[r, c]
            
    nrow_B = ncol_A
    ncol_B = random.randint(min_matrix_size, max_matrix_size)

    np_matrix_B = np.random.rand(nrow_B, ncol_B)

    test_matrix_B = _matrix.Matrix(nrow_B, ncol_B)
    for r in range(nrow_B):
        for c in range(ncol_B):
            test_matrix_B[r, c] = np_matrix_B[r, c]

    np_matrix_result = np.dot(np_matrix_A, np_matrix_B)
    
    tile_matrix_result = _matrix.Matrix(nrow_A, ncol_B)
    tile_matrix_result = _matrix.multiply_tile(test_matrix_A, test_matrix_B, 64)
        
    for r in range(nrow_A):
        for c in range(ncol_B):
            assert np.abs(np_matrix_result[r, c] - tile_matrix_result[r, c]) < 1e-6
