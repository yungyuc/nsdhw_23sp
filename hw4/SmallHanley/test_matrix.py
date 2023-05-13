import _matrix
import pytest
import timeit
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

        assert 0 == _matrix.bytes()

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)
        assert 3*8*size*size == _matrix.bytes()
        base_alloc = _matrix.allocated()
        base_dealloc = _matrix.deallocated()

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

        assert 3*8*size*size == _matrix.bytes()
        assert base_alloc == _matrix.allocated()
        assert base_dealloc ==_matrix.deallocated()

    def test_match_naive_mkl(self):

        assert 0 == _matrix.bytes()

        size = 100
        mat1, mat2, *_ = self.make_matrices(size)
        assert 3*8*size*size == _matrix.bytes()
        base_alloc = _matrix.allocated()
        base_dealloc = _matrix.deallocated()

        ret_naive = _matrix.multiply_naive(mat1, mat2)
        ret_mkl = _matrix.multiply_mkl(mat1, mat2)

        assert size == ret_naive.nrow
        assert size == ret_naive.ncol
        assert size == ret_mkl.nrow
        assert size == ret_mkl.ncol

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                assert mat1[i,j] is not ret_mkl[i,j]
                assert ret_naive[i,j] == ret_mkl[i,j]

        assert 5*8*size*size == _matrix.bytes()
        assert base_alloc + 2*8*size*size == _matrix.allocated()
        assert base_dealloc == _matrix.deallocated()

    def test_zero(self):

        assert 0 == _matrix.bytes()

        size = 200
        mat1, mat2, mat3, *_ = self.make_matrices(size)
        assert 3*8*size*size == _matrix.bytes()
        base_alloc = _matrix.allocated()
        base_dealloc = _matrix.deallocated()

        ret_naive = _matrix.multiply_naive(mat1, mat3)
        ret_mkl = _matrix.multiply_mkl(mat1, mat3)

        assert size == ret_naive.nrow
        assert size == ret_naive.ncol
        assert size == ret_mkl.nrow
        assert size == ret_mkl.ncol

        for i in range(ret_naive.nrow):
            for j in range(ret_naive.ncol):
                assert 0 == ret_naive[i, j]
                assert 0 == ret_mkl[i, j]

        assert 5*8*size*size == _matrix.bytes()
        assert base_alloc+2*8*size*size == _matrix.allocated()
        assert base_dealloc == _matrix.deallocated()

    def test_memory(self):

        assert 0 == _matrix.bytes()
        base_alloc = _matrix.allocated()
        base_dealloc = _matrix.deallocated()

        size = 100
        mat1, mat2, mat3, *_ = self.make_matrices(size)
        assert 3*8*size*size == _matrix.bytes()
        # New allocation.
        assert base_alloc+3*8*size*size == _matrix.allocated()
        # No deallocation.
        assert base_dealloc == _matrix.deallocated()
        mat1 = mat2 = mat3 = None
        # Matrices are deallocated.
        assert 0 == _matrix.bytes()
        assert base_dealloc+3*8*size*size == _matrix.deallocated()
        assert base_alloc+3*8*size*size == _matrix.allocated()
