import pytest
import _matrix
import numpy as np
import timeit

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
            
def test_performance():
    size = 1000
    tile_size = 16
    repeat = 5
    A, A_rdm = build_matrix(size, size)
    B, B_rdm = build_matrix(size, size)
    ns = dict(multiply_naive = _matrix.multiply_naive, multiply_tile = _matrix.multiply_tile, multiply_mkl = _matrix.multiply_mkl, A = A, B = B, tile_size = tile_size)
    
    naive = timeit.Timer('multiply_naive(A, B)', globals=ns)
    tile = timeit.Timer('multiply_tile(A, B, tile_size)', globals=ns)
    mkl = timeit.Timer('multiply_mkl(A, B)', globals=ns)
    
    with open('performance.txt', 'w') as fout:
        print('\n')
        
        fout.write(f'Start multiply_naive (repeat={repeat}), take min = ')
        naivesec = minsec = min(naive.repeat(repeat=repeat, number=1))
        fout.write(f'{minsec} seconds\n')
        print(f'Start multiply_naive (repeat={repeat}), take min = {minsec} seconds')

        fout.write(f'Start multiply_tile (repeat={repeat}), take min = ')
        tilesec = minsec = min(tile.repeat(repeat=repeat, number=1))
        fout.write(f'{minsec} seconds\n')
        print(f'Start multiply_tile (repeat={repeat}), take min = {minsec} seconds')
        
        fout.write(f'Start multiply_mkl (repeat={repeat}), take min = ')
        mklsec = minsec = min(mkl.repeat(repeat=repeat, number=1))
        fout.write(f'{minsec} seconds\n')
        print(f'Start multiply_mkl (repeat={repeat}), take min = {minsec} seconds')

        fout.write('tile speed-up over naive: %g x\n' % (naivesec/tilesec))
        fout.write('MKL speed-up over naive: %g x\n' % (naivesec/mklsec))
        print('tile speed-up over naive: %g x' % (naivesec/tilesec))
        print('MKL speed-up over naive: %g x' % (naivesec/mklsec))
        
        print('\n')

    
        