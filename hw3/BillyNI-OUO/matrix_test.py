import pytest
import math
import _matrix

size = 10
def make_matrices(size):

    mat1 = _matrix.Matrix(size,size)
    mat2 = _matrix.Matrix(size,size)
    mat3 = _matrix.Matrix(size,size)
    print(mat1.ncol)
    for it in range(size):
        for jt in range(size):
            mat1[it, jt] = it * size + jt + 1
            mat2[it, jt] = it * size + jt + 1
            mat3[it, jt] = 0

    return mat1, mat2, mat3

def test_multiply_naive():
    mat1, mat2, mat3 = make_matrices(10)
    
    assert mat1[0,1] == 2
    assert size+2 == mat1[1,1]
    assert size*2 == mat1[1,size-1]
    assert size*size == mat1[size-1,size-1]

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            assert 0 != mat1[i,j]
            assert mat1[i,j] == mat2[i,j]
            assert 0 == mat3[i,j]
    assert mat1 == mat2
    assert mat1 is not mat2
    
def test_multiple_tile():
    mat1, mat2, mat3 = make_matrices(10)
    
    assert mat1[0,1] == 2
    assert size+2 == mat1[1,1]
    assert size*2 == mat1[1,size-1]
    assert size*size == mat1[size-1,size-1]

    for i in range(mat1.nrow):
        for j in range(mat1.ncol):
            assert 0 != mat1[i,j]
            assert mat1[i,j] == mat2[i,j]
            assert 0 == mat3[i,j]
    assert mat1 == mat2
    assert mat1 is not mat2
# def test_zero_vector():
#     assert math.isclose(_vector.cal_angle_in_python(1, 10, 1, 10), 0)

# def test_right_vector():
#     assert math.isclose(_vector.cal_angle_in_python(1, 0, 0, 1), 90*math.pi/180)

# def test_other_vector():
#     assert math.isclose(_vector.cal_angle_in_python(0, 1, 1, 1), 45*math.pi/180)

