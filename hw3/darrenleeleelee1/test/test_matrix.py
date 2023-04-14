import random
import _matrix

def test_zero_matrix():
    test_matrix = _matrix.Matrix(0, 0)
    assert test_matrix.nrow == 0 and test_matrix.ncol == 0

def test_matrix_creation():
    min_matrix_size = 1
    max_matrix_size = 10000
    nrow = random.randint(min_matrix_size, max_matrix_size)
    ncol = random.randint(min_matrix_size, max_matrix_size)
    test_matrix = _matrix.Matrix(nrow, ncol)
    
    assert test_matrix.nrow == nrow and test_matrix.ncol == ncol
        
def test_matrixGetter():
    min_matrix_size = 1
    max_matrix_size = 100
    nrow = random.randint(min_matrix_size, max_matrix_size)
    ncol = random.randint(min_matrix_size, max_matrix_size)
    test_matrix = _matrix.Matrix(nrow, ncol)
    
    r_id = random.randint(0, nrow-1)
    c_id = random.randint(0, ncol-1)
    try:
        _ = test_matrix[r_id, c_id]
        assert True
    except:
        assert False
    
def test_matrixSetter():
    min_matrix_size = 1
    max_matrix_size = 100
    nrow = random.randint(min_matrix_size, max_matrix_size)
    ncol = random.randint(min_matrix_size, max_matrix_size)
    test_matrix = _matrix.Matrix(nrow, ncol)
    
    for r in range(nrow):
        for c in range(ncol):
            test_matrix[r, c] = r * nrow + c
            assert test_matrix[r, c] == r * nrow + c

