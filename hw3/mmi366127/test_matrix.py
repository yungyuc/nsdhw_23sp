import random, sys
import numpy as np
from math import fabs
import timeit

# The python module that wraps the matrix code.
import _matrix


def test_entry():

    # test the __getitem__ and __setitem__
    l, r = 1, 1024
    n = random.randint(l, r)
    m = random.randint(l, r)    
    mat = _matrix.Matrix(n, m)
    
    repeat = 10000
    for _ in range(repeat):
        
        i = random.randint(0, n - 1)
        j = random.randint(0, m - 1)

        value = random.random()
        mat[i, j] = value

        assert mat[i, j] == value

def random_matrix(n, m):
    mat_numpy = np.random.rand(n, m).astype(np.double)
    mat = _matrix.Matrix(n, m)
    for i in range(n):
        for j in range(m):
            mat[i, j] = mat_numpy[i, j]

    return mat, mat_numpy

def two_random_matrix(l = 1, r = 1024):
    # Generate two matrices with random dimension and value
    n = random.randint(l, r)
    m = random.randint(l, r)
    k = random.randint(l, r)

    A, A_numpy = random_matrix(n, m)
    B, B_numpy = random_matrix(m, k)

    return A, B, A_numpy, B_numpy

def test_equal():

    # Test if the __eq__ works
    repeat = 10
    l, r = 1, 1024
    for _ in range(repeat):
        
        n = random.randint(l, r)
        m = random.randint(l, r)

        A = _matrix.Matrix(n, m)
        B = _matrix.Matrix(n, m)

        for i in range(n):
            for j in range(m):
                value = random.random()
                A[i, j] = value
                B[i, j] = value
        
        assert A == B

def test_attribute():
    l, r = 1, 1024
    n = random.randint(l, r)
    m = random.randint(l, r)
    mat = _matrix.Matrix(n, m)    
    assert mat.nrow == n
    assert mat.ncol == m

eps = 1e-9

def test_naive():
    repeat = 5
    for _ in range(repeat):
        A, B, A_numpy, B_numpy = two_random_matrix()
        C_naive = _matrix.multiply_naive(A, B)
        C_numpy = np.matmul(A_numpy, B_numpy)

        for i in range(C_naive.nrow):
            for j in range(C_naive.ncol):
                assert fabs(C_numpy[i, j] - C_naive[i, j]) < eps
        
def test_tile():
    repeat = 5
    for BLOCK_SIZE in [4, 8, 16, 32, 87]:
        for _ in range(repeat):
            A, B, A_numpy, B_numpy = two_random_matrix()
            C_tile = _matrix.multiply_tile(A, B, BLOCK_SIZE)
            C_numpy = np.matmul(A_numpy, B_numpy)
        
            for i in range(C_tile.nrow):
                for j in range(C_tile.ncol):
                    assert fabs(C_numpy[i, j] - C_tile[i, j]) < eps

def test_mkl():
    repeat = 5
    for _ in range(repeat):
        A, B, A_numpy, B_numpy = two_random_matrix()
        C_mkl = _matrix.multiply_mkl(A, B)
        C_numpy = np.matmul(A_numpy, B_numpy)
        
        for i in range(C_mkl.nrow):
            for j in range(C_mkl.ncol):
                assert fabs(C_numpy[i, j] - C_mkl[i, j]) < eps

class Writer:

    def __init__(self, streams):

        self.streams = streams

    def write(self, msg):

        for stream in self.streams:

            stream.write(msg)

def test_performacne():
    setup = '''
import numpy as np
import random 

import _matrix

n, m, k = 987, 1022, 978

mat1 = _matrix.Matrix(n, m)
mat2 = _matrix.Matrix(m, k)

for i in range(n):
    for j in range(m):
        mat1[i, j] = random.random()

for i in range(m):
    for j in range(k):
        mat2[i, j] = random.random()

'''

    naive = timeit.Timer('_matrix.multiply_naive(mat1, mat2)', setup=setup)
    mkl = timeit.Timer('_matrix.multiply_mkl(mat1, mat2)', setup=setup)

    tile4 = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 4)', setup=setup)
    tile8 = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 8)', setup=setup)
    tile16 = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 16)', setup=setup)
    tile32 = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 32)', setup=setup)
    tile64 = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 64)', setup=setup)
    tile128 = timeit.Timer('_matrix.multiply_tile(mat1, mat2, 128)', setup=setup)

    repeat = 5

    with open('performance.txt', 'w') as f:

        w = Writer([sys.stdout, f])

        naive_sec = min(naive.repeat(repeat=repeat, number=1))
        w.write(f'multiply_naive takes {naive_sec} seconds\n')

        mkl_sec = min(mkl.repeat(repeat=repeat, number=1))
        w.write(f'multiply_mkl takes {mkl_sec} seconds\n')

        tile4_sec = min(tile4.repeat(repeat=repeat, number=1))
        w.write(f'multiply_tile with block size = 4 takes {tile4_sec} seconds\n')
        
        tile8_sec = min(tile8.repeat(repeat=repeat, number=1))
        w.write(f'multiply_tile with block size = 8 takes {tile8_sec} seconds\n')

        tile16_sec = min(tile16.repeat(repeat=repeat, number=1))
        w.write(f'multiply_tile with block size = 16 takes {tile16_sec} seconds\n')

        tile32_sec = min(tile32.repeat(repeat=repeat, number=1))
        w.write(f'multiply_tile with block size = 32 takes {tile32_sec} seconds\n')

        tile64_sec = min(tile64.repeat(repeat=repeat, number=1))
        w.write(f'multiply_tile with block size = 64 takes {tile64_sec} seconds\n')

        tile128_sec = min(tile64.repeat(repeat=repeat, number=1))
        w.write(f'multiply_tile with block size = 128 takes {tile128_sec} seconds\n')