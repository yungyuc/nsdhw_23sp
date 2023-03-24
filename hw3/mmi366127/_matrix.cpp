#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include <vector>
#include <cmath>

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

void memcpy_matrix(double *dst, const double *src, int nrow, int ncol, int pad_dst, int pad_src) {
    for(int i = 0; i < nrow; ++i) {
        for(int j = 0; j < ncol; ++j) {
            dst[i * pad_dst + j] = src[i * pad_src + j];
        }
    }
}

const double eps = 1e-9;

struct Matrix {
    double *data;
    int nrow, ncol, pad;
    Matrix(int r, int c) {
        nrow = r, ncol = c;
        pad = get_padding(c);
        // using memory padding to have better locality
        data = (double*)aligned_alloc(64, sizeof(double) * r * pad);
        memset(data, 0, sizeof(double) * r * pad);
    }
    Matrix(const Matrix &other) {
        nrow = other.nrow;
        ncol = other.ncol;
        pad = get_padding(ncol);
        // using memory padding to have better locality
        data = (double*)aligned_alloc(64, sizeof(double) * nrow * pad);
        memcpy_matrix(data, other.data, nrow, ncol, pad, other.pad);
    }
    ~Matrix() {
        free(data);
    }
    int get_padding(int x) {
        return x;
        return ((x - 1) / 16 + 1) * 16;
    }
    void resize(int r, int c) {
        // Becareful, calling resize will free the data
        free(data);
        nrow = r, ncol = c;
        pad = get_padding(c);
        data = (double*)aligned_alloc(64, sizeof(double) * r * pad);
    }
    int size() const {
        return nrow * ncol;
    }
    Matrix &operator=(const Matrix &other) {
        if(this == &other) 
            return *this;
        if (nrow != other.nrow || ncol != other.ncol) {
            resize(other.nrow, other.ncol);
        }
        memcpy_matrix(data, other.data, nrow, ncol, pad, other.pad);
    }
    bool operator==(const Matrix &other) const {
        if(nrow != other.nrow) return false;
        if(ncol != other.ncol) return false;
        for(int i = 0; i < nrow; ++i) {
            for(int j = 0; j < ncol; ++j)
                if(fabs(data[i * pad + j] - other.data[i * other.pad + j]) > eps) return false;
        }
        return true;
    }
    bool __eq__(const Matrix &other) {
        return *this == other;
    }
    double getvalue(const std::vector<int> idx) const {
        assert(idx.size() == 2);
        assert(idx[0] < nrow);
        assert(idx[1] < ncol);
        return data[idx[0] * pad + idx[1]];
    }
    void setvalue(const std::vector<int> idx, double v) {
        assert(idx.size() == 2);
        assert(idx[0] < nrow);
        assert(idx[1] < ncol);
        data[idx[0] * pad + idx[1]] = v;
    }
};

Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    assert(A.ncol == B.nrow);
    Matrix C(A.nrow, B.ncol);
    for(int i = 0; i < C.nrow; ++i) {
        for(int j = 0; j < C.ncol; ++j) {
            double temp = 0.0;
            for(int k = 0; k < A.ncol; ++k) {
                temp += A.data[i * A.pad + k] * B.data[k * B.pad + j];
            }
            C.data[i * C.pad + j] = temp;
        }
    }
    return C;
}

Matrix multiply_tile(const Matrix &A, const Matrix &B, int BLOCK_SIZE) {
    assert(A.ncol == B.nrow);
    Matrix C(A.nrow, B.ncol);
    for(int i = 0; i < A.nrow; i += BLOCK_SIZE) {
        for(int j = 0; j < B.ncol; j += BLOCK_SIZE) {
            for(int k = 0; k < A.ncol; k += BLOCK_SIZE) {
                // multiply in block
                for(int ib = 0; ib < BLOCK_SIZE; ++ib) {
                    if(ib + i >= A.nrow) break;
                    for(int jb = 0; jb < BLOCK_SIZE; ++jb) {
                        if(jb + j >= B.ncol) break;
                        for(int kb = 0; kb < BLOCK_SIZE; ++kb) {
                            if(kb + k >= A.ncol) break;
                            C.data[(ib + i) * C.pad + (jb + j)] += A.data[(ib + i) * A.pad + (kb + k)] * B.data[(kb + k) * B.pad + (jb + j)];
                        }
                    }
                }
            }
        }
    }
    return C;
}

void myDgemm(double* a, double* b, double* c, int m, int k, int n) {
    // fix column-major problem
    char transa = 'N', transb = 'N';
    double alpha = 1.0, beta = 0.;
    dgemm_(&transb, &transa, &n, &m, &k, &alpha, b, &n, a, &k, &beta, c, &n);
}

Matrix multiply_mkl(const Matrix &A, const Matrix &B) {
    assert(A.ncol == B.nrow);
    Matrix C(A.nrow, B.ncol);
    double *temp_a, *temp_b, *temp_c;
    temp_a = (double*)aligned_alloc(64, sizeof(double) * A.size());
    temp_b = (double*)aligned_alloc(64, sizeof(double) * B.size());
    temp_c = (double*)aligned_alloc(64, sizeof(double) * C.size());
    memset(temp_c, 0, sizeof(double) * C.size());
    memcpy_matrix(temp_a, A.data, A.nrow, A.ncol, A.ncol, A.pad);
    memcpy_matrix(temp_b, B.data, B.nrow, B.ncol, B.ncol, B.pad);
    myDgemm(temp_a, temp_b, temp_c, A.nrow, A.ncol, B.ncol);
    memcpy_matrix(C.data, temp_c, C.nrow, C.ncol, C.pad, C.ncol);
    free(temp_a); free(temp_b); free(temp_c); 
    return C;
}

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "tool for matrix multiplication"; 
    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<int, int>())
        .def("size", &Matrix::size)
        .def("__eq__", &Matrix::__eq__)
        .def("__getitem__", &Matrix::getvalue)
        .def("__setitem__", &Matrix::setvalue)
        .def_readonly("nrow", &Matrix::nrow)
        .def_readonly("ncol", &Matrix::ncol);
    m.def("multiply_naive", &multiply_naive, "naive matrix multiply method.");
    m.def("multiply_tile", &multiply_tile, " matrix multiply method with tiling.");
    m.def("multiply_mkl", &multiply_mkl, "matrix multiply method with BLAS.");
}