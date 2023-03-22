import _matrix
import pytest
import math

class Test_Matrix():
    def make_matrices(self, size):

        mat1 = _matrix.Matrix(size,size)
        mat2 = _matrix.Matrix(size,size)
        mat3 = _matrix.Matrix(size,size)

        for it in range(size):
            for jt in range(size):
                mat1[it, jt] = it * size + jt + 1
                mat2[it, jt] = it * size + jt + 1
                mat3[it, jt] = 0

        return mat1, mat2, mat3

    def test_basic(self):

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)

        assert size == mat1.nrow
        assert size == mat1.ncol
        assert size == mat2.nrow
        assert size == mat2.ncol
        assert size == mat3.nrow
        assert size == mat3.ncol

        assert 2 == mat1[0,1]
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
