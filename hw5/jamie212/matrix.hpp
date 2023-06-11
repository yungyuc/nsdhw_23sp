#pragma once
#include <iostream>
#include <chrono>
#include <mkl.h>
#include <bits/stdc++.h>


using namespace std;
using namespace chrono;
namespace py = pybind11;

# define RANDMAX 10

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_data = new double[nelement];
    }

    ~Matrix()
    {
        delete[] m_data;
    }
    
    Matrix(const Matrix &matrix){
            m_nrow = matrix.m_nrow;
            m_ncol = matrix.m_ncol;
            m_data = new double[m_nrow * m_ncol](); 
            memcpy(m_data, matrix.m_data, m_nrow * m_ncol*sizeof(double));
    }

    bool operator == (Matrix const &matrix) const {
            for (size_t i = 0; i < m_nrow; i++) 
                for (size_t j = 0; j < m_ncol; j++)
                    if (m_data[i*m_ncol + j] != matrix(i, j)) 
                        return false;
            return true;
        }

    Matrix& operator= (const Matrix& matrix) {
        m_data = matrix.m_data;
        return *this;
    }

    double& operator()(size_t row, size_t col)
    {
        return m_data[row * m_ncol + col];
    }

    double operator()(size_t row, size_t col) const
    {
        return m_data[row * m_ncol + col];
    }

    double * get_data() const { 
        return m_data; 
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

private:
    size_t m_nrow;
    size_t m_ncol;
    double* m_data;
};

Matrix multiply_naive(Matrix const & A, Matrix const & B)
{
    assert(A.ncol() == B.nrow());

    size_t n = A.nrow();
    size_t m = A.ncol();
    size_t p = B.ncol();

    Matrix C(n, p);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < p; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < m; k++) {
                sum += A(i, k) * B(k, j);
            }
            C(i, j) = sum;
        }
    }
    return C;
}


Matrix multiply_tile(Matrix& A, Matrix& B, size_t tile_size) {
    assert(A.ncol() == B.nrow());

    size_t n = A.nrow();
    size_t m = A.ncol();
    size_t p = B.ncol();

    Matrix C(n, p);

    for (size_t i = 0; i < C.nrow(); ++i) {
        for (size_t j = 0; j < C.ncol(); ++j) {
            C(i, j) = 0.0;
        }
    }

    for (size_t i = 0; i < n; i += tile_size) {
        for (size_t j = 0; j < p; j += tile_size) {
            for (size_t k = 0; k < m; k += tile_size) {
                for (size_t ii = i; ii < std::min(i + tile_size, n); ++ii) {
                    for (size_t jj = j; jj < std::min(j + tile_size, p); ++jj) {
                        double sum = 0.0;
                        for (size_t kk = k; kk < std::min(k + tile_size, m); ++kk) {
                            sum += A(ii, kk) * B(kk, jj);
                        }
                        C(ii, jj) += sum;
                    }
                }
            }
        }
    }
    return C;
}

Matrix multiply_mkl(const Matrix& A, const Matrix& B) {
    assert(A.ncol() == B.nrow());

    size_t n = A.nrow();
    size_t m = A.ncol();
    size_t p = B.ncol();

    Matrix E(n, p);

    const double alpha = 1.0;
    const double beta = 0.0;
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, p, m, alpha, A.get_data(), m, B.get_data(), p, beta, E.get_data(), p);
    return E;
}


