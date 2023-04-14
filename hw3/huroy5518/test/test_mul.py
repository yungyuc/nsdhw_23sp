from _matrix import *
import pytest
import random

def gen_matrix(r, c):
    A = Matrix(r, c)
    for i in range(r):
        for j in range(c):
            A[i, j] = random.randint(0, 10000)
    return A

def test_multiply_naive():
    row1 = random.randint(50, 100)
    col1 = random.randint(50, 100)
    col2 = random.randint(50, 100)

    ans = [[0 for i in range(col2)] for i in range(row1)]

    A = gen_matrix(row1, col1)
    B = gen_matrix(col1, col2)
    for i in range(row1):
        for j in range(col2):
            for k in range(col1):
                ans[i][j] += A[i, k] * B[k, j]
    C = multiply_naive(A, B)

    for i in range(row1):
        for j in range(col2):
            assert C[i, j] == ans[i][j]

def test_multiply_tile():
    row1 = random.randint(50, 100)
    col1 = random.randint(50, 100)
    col2 = random.randint(50, 100)

    ans = [[0 for i in range(col2)] for i in range(row1)]

    A = gen_matrix(row1, col1)
    B = gen_matrix(col1, col2)
    for i in range(row1):
        for j in range(col2):
            for k in range(col1):
                ans[i][j] += A[i, k] * B[k, j]
    C = multiply_tile(A, B, 64)
    print(C)
    for i in range(row1):
        for j in range(col2):
            assert C[i, j] == ans[i][j]