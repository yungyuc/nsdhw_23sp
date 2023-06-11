#ifndef _MATRIX_INCLUDED_
#define _MATRIX_INCLUDED_
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include <limits>
#include <vector>
#include <cmath>

const int alligned = 64;
const double eps = 1e-9;


extern "C" {
extern void dgemm_(const char* transa,
                   const char* transb,
                   const int* m,
                   const int* n,
                   const int* k,
                   const double* alpha,
                   const double* a,
                   const int* lda,
                   const double* b,
                   const int* ldb,
                   const double* beta,
                   double* c,
                   const int* ldc);
}


struct Matrix {
    double *data;
    size_t _nrow, _ncol;
    Matrix() = default;
    Matrix(size_t r, size_t c) : _nrow(r), _ncol(c) {
        data = (double*)aligned_alloc(alligned, sizeof(double) * r * c);
        memset(data, 0, sizeof(double) * r * c);
    }
    Matrix(const Matrix &other) {
        _nrow = other.nrow();
        _ncol = other.ncol();
        data = (double*)aligned_alloc(alligned, sizeof(double) * nrow() * ncol());
        memcpy(data, other.data, sizeof(double) * nrow() * ncol());
    }
    ~Matrix() {
        free(data);
    }
    void resize(int r, int c) {
        // Becareful, calling resize will free the data
        free(data);
        _nrow = r, _ncol = c;
        data = (double*)aligned_alloc(alligned, sizeof(double) * r * c);
    }
    int size() const {
        return nrow() * ncol();
    }
    Matrix &operator=(const Matrix &other) {
        if(this == &other) 
            return *this;
        if (nrow() != other.nrow() || ncol() != other.ncol()) {
            resize(other.nrow(), other.ncol());
        }
        memcpy(data, other.data, sizeof(double) * nrow() * ncol());
        return *this;
    }
    bool operator==(const Matrix &other) const {
        if(nrow() != other.nrow()) return false;
        if(ncol() != other.ncol()) return false;
        for(size_t i = 0; i < nrow(); ++i) {
            for(size_t j = 0; j < ncol(); ++j)
                if(fabs(data[i * ncol() + j] - other.data[i * other.ncol() + j]) > eps) return false;
        }
        return true;
    }
    bool operator!=(const Matrix &other) const {
        return !(*this == other);
    }
    double &operator()(const size_t r, const size_t c) {
        assert(0 <= r && r < nrow());
        assert(0 <= c && c < ncol());
        return data[r * ncol() + c];
    }
    double operator()(const size_t r, const size_t c) const {
        assert(0 <= r && r < nrow());
        assert(0 <= c && c < ncol());
        return data[r * ncol() + c];
    }
    size_t nrow() const { return _nrow; }
    size_t ncol() const { return _ncol; }
};


Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    for(size_t i = 0; i < C.nrow(); ++i) {
        for(size_t j = 0; j < C.ncol(); ++j) {
            double temp = 0.0;
            for(size_t k = 0; k < A.ncol(); ++k) {
                temp += A.data[i * A.ncol() + k] * B.data[k * B.ncol() + j];
            }
            C.data[i * C.ncol() + j] = temp;
        }
    }
    return C;
}

Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t BLOCK_SIZE) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    for(size_t i = 0; i < A.nrow(); i += BLOCK_SIZE) {
        for(size_t j = 0; j < B.ncol(); j += BLOCK_SIZE) {
            for(size_t k = 0; k < A.ncol(); k += BLOCK_SIZE) {
                // multiply in block
                for(size_t ib = 0; ib < BLOCK_SIZE; ++ib) {
                    if(ib + i >= A.nrow()) break;
                    for(size_t jb = 0; jb < BLOCK_SIZE; ++jb) {
                        if(jb + j >= B.ncol()) break;
                        for(size_t kb = 0; kb < BLOCK_SIZE; ++kb) {
                            if(kb + k >= A.ncol()) break;
                            C.data[(ib + i) * C.ncol() + (jb + j)] += A.data[(ib + i) * A.ncol() + (kb + k)] * B.data[(kb + k) * B.ncol() + (jb + j)];
                        }
                    }
                }
            }
        }
    }
    return C;
}

void myDgemm(const double* a, const double* b, double* c, const int m, const int k, const int n) {
    // fix column-major problem
    char transa = 'N', transb = 'N';
    double alpha = 1.0, beta = 0.;
    dgemm_(&transb, &transa, &n, &m, &k, &alpha, b, &n, a, &k, &beta, c, &n);
}

Matrix multiply_mkl(const Matrix &A, const Matrix &B) {
    assert(A.ncol() == B.nrow());
    Matrix C(A.nrow(), B.ncol());
    myDgemm(A.data, B.data, C.data, A.nrow(), A.ncol(), B.ncol());
    return C;
}

#endif