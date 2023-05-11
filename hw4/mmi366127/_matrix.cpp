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

template<typename T>
struct CustomAllocator {

    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;
    static size_type c_allocated, c_deallocated;

    CustomAllocator() = default;
    ~CustomAllocator() = default;
    template<class U>
    struct rebind {
        using other=CustomAllocator<U>;
    };

    pointer allocate(size_type n) {
        if(n > std::numeric_limits<size_type>::max() / sizeof(T)) {
            throw std::bad_alloc();
        }
        const size_type bytes = n * sizeof(T);
        pointer p = static_cast<pointer>(aligned_alloc(alligned, bytes));
        if(p) {
            c_allocated += bytes;
            return p;
        }
        else {
            throw std::bad_alloc();
        }
    }

    void deallocate(pointer p, size_type n) noexcept {
        free(p);
        
        const size_type bytes = n * sizeof(T);
        c_deallocated += bytes;
    }

    static size_type bytes() {
        return c_allocated - c_deallocated;
    }
    static size_type allocated() {
        return c_allocated;
    }
    static size_type deallocated() {
        return c_deallocated;
    }

};

template<typename T, typename U>
constexpr bool operator!=(const CustomAllocator<T> &,
  const CustomAllocator<U> &) noexcept;

template<typename T>
typename CustomAllocator<T>::size_type CustomAllocator<T>::c_allocated = 0;
template<typename T>
typename CustomAllocator<T>::size_type CustomAllocator<T>::c_deallocated = 0;

const double eps = 1e-9;

struct Matrix {
    // double *data;
    int nrow, ncol;
    std::vector<double, CustomAllocator<double>> data;
    Matrix(int r, int c) : nrow(r), ncol(c), data(r * c) {}
    Matrix(const Matrix &other)=delete;
    Matrix(Matrix&& other)=default;
    ~Matrix() {}
    void resize(int r, int c) {
        nrow = r, ncol = c;
        data.resize(r * c);
    }
    std::size_t size() const {
        return data.size();
    }
    Matrix &operator=(const Matrix &other)=delete;
    bool operator==(const Matrix &other) const {
        if(nrow != other.nrow) return false;
        if(ncol != other.ncol) return false;
        for(int i = 0; i < nrow; ++i) {
            for(int j = 0; j < ncol; ++j)
                if(fabs(data[i * ncol + j] - other.data[i * other.ncol + j]) > eps) return false;
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
        return data[idx[0] * ncol + idx[1]];
    }
    void setvalue(const std::vector<int> idx, double v) {
        assert(idx.size() == 2);
        assert(idx[0] < nrow);
        assert(idx[1] < ncol);
        data[idx[0] * ncol + idx[1]] = v;
    }
};

Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    assert(A.ncol == B.nrow);
    Matrix C(A.nrow, B.ncol);
    for(int i = 0; i < C.nrow; ++i) {
        for(int j = 0; j < C.ncol; ++j) {
            double temp = 0.0;
            for(int k = 0; k < A.ncol; ++k) {
                temp += A.data[i * A.ncol + k] * B.data[k * B.ncol + j];
            }
            C.data[i * C.ncol + j] = temp;
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
                            C.data[(ib + i) * C.ncol + (jb + j)] += A.data[(ib + i) * A.ncol + (kb + k)] * B.data[(kb + k) * B.ncol + (jb + j)];
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
    assert(A.ncol == B.nrow);
    Matrix C(A.nrow, B.ncol);
    myDgemm(A.data.data(), B.data.data(), &C.data[0], A.nrow, A.ncol, B.ncol);
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
    m.def("bytes", &CustomAllocator<double>::bytes);
    m.def("allocated", &CustomAllocator<double>::allocated);
    m.def("deallocated", &CustomAllocator<double>::deallocated);
}