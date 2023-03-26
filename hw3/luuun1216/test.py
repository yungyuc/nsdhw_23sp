import pytest
import random
import numpy as np
from _matrix import Matrix, multiply_naive, multiply_tile

def get_np_matrix(n, m, mat):
    mat_np = np.zeros((n, m))
    for i in range(n):
        for j in range(m):
            mat_np[i, j] = mat[i, j]
    return mat_np

def test_naive():
    n_size = 1000
    m_size = 1000
    matrix_A = Matrix(n_size, m_size)
    matrix_B = Matrix(n_size, m_size)
    A_np = get_np_matrix(n_size, m_size, matrix_A)
    B_np = get_np_matrix(n_size, m_size, matrix_B)
    naive_ans = multiply_naive(matrix_A, matrix_B)
    naive_ans_np = np.matmul(A_np, B_np)
    for i in range(n_size):
        for j in range(m_size):
            assert abs(naive_ans[i, j] - naive_ans_np[i, j]) < 1e-6

def test_tile():
    tile_size = 100
    n_size = 1000
    m_size = 1000
    matrix_C = Matrix(n_size, m_size)
    matrix_D = Matrix(n_size, m_size)
    C_np = get_np_matrix(n_size, m_size, matrix_C)
    D_np = get_np_matrix(n_size, m_size, matrix_D)
    tile_ans = multiply_tile(matrix_C, matrix_D, tile_size)
    tile_ans_np = np.matmul(C_np, D_np)
    for i in range(n_size):
        for j in range(m_size):
            assert abs(tile_ans[i, j] - tile_ans_np[i, j]) < 1e-6