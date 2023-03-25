import pytest
import random
import numpy as np
# import _matrix
from _matrix import Matrix, multiply_naive, multiply_tile, generateValue

def get_np_matrix(n, m, mat):
    mat_np = np.zeros((n, m))
    for i in range(n):
        for j in range(m):
            mat_np[i, j] = mat[i, j]
    return mat_np

def test_naive():
    n_size = 4
    m_size = 4
    A = Matrix(n_size, m_size)
    B = Matrix(n_size, m_size)
    generateValue(A)
    generateValue(B)
    A_np = get_np_matrix(n_size, m_size, A)
    B_np = get_np_matrix(n_size, m_size, B)
    naive_ans = multiply_naive(A, B)
    naive_ans_np = np.matmul(A_np, B_np)
    for i in range(n_size):
        for j in range(m_size):
            assert abs(naive_ans[i, j] - naive_ans_np[i, j]) < 1e-6

def test_tile():
    tile_size = 2
    n_size = 4
    m_size = 4
    C = Matrix(n_size, m_size)
    D = Matrix(n_size, m_size)
    generateValue(C)
    generateValue(D)
    C_np = get_np_matrix(n_size, m_size, C)
    D_np = get_np_matrix(n_size, m_size, D)
    tile_ans = multiply_tile(C, D, tile_size)
    tile_ans_np = np.matmul(C_np, D_np)
    for i in range(n_size):
        for j in range(m_size):
            assert abs(tile_ans[i, j] - tile_ans_np[i, j]) < 1e-6

# test_naive()
    # C_np = np.matmul(A_np, B_np)
    # for i in range(n_size):
    #     for j in range(m_size):
    #         assert abs(C_np[i, j] - C_np[i, j]) < 1e-4
    # result_naive = multiply_naive(A, B)
    # result_tile = multiply_tile(A, B, tile_size)

    # assert matrices_equal(result_naive, result_tile)
    # assert matrices_equal(result_naive, A @ B)
    # assert matrices_equal(result_tile, A @ B)