#pragma once
#include <iostream>
#include <cstddef>
#include <algorithm>
#include <mkl/mkl.h>
class Matrix {

public:
    Matrix() = default;
    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement];
    }

    ~Matrix()
    {
        delete[] m_buffer;
    }

    Matrix(const Matrix &matrix){
            m_nrow = matrix.m_nrow;
            m_ncol = matrix.m_ncol;
            m_buffer = new double[m_nrow * m_ncol](); 
            memcpy(m_buffer, matrix.m_buffer, m_nrow * m_ncol*sizeof(double));
    }

    Matrix& operator= (const Matrix& matrix) {
        m_buffer = matrix.m_buffer;
        return *this;
    }

    bool operator==(const Matrix &other) const
    {
        if (nrow() != other.nrow() || ncol() != other.ncol()) {
            return false;
        }

        for (size_t i = 0; i < m_nrow; i++) {
            for (size_t j = 0; j < m_ncol; j++) {
                if ((*this)(i, j) != other(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix &other) const
    {
        return !(*this == other);
    }
    
    // No bound check.
    double   operator() (size_t row, size_t col) const{
        return m_buffer[row*m_ncol + col];
    }
    
    double & operator() (size_t row, size_t col){
        return m_buffer[row*m_ncol + col];
    }

    void zero(){
        for(size_t r = 0; r < m_nrow; r++){
            for(size_t c = 0; c < m_ncol; c++){
                m_buffer[r*m_ncol + c] = 0.0;
            }
        }
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    double *buffer() const { return m_buffer; }
private:

    size_t m_nrow;
    size_t m_ncol;
    double * m_buffer;

};

Matrix multiply_naive(const Matrix& A, const Matrix& B) {
    size_t nrow_A = A.nrow();
    size_t ncol_A = A.ncol();
    size_t nrow_B = B.nrow();
    size_t ncol_B = B.ncol();

    if (ncol_A != nrow_B) {
        throw std::runtime_error("Matrix dimensions do not match for multiplication.");
    }

    Matrix result(nrow_A, ncol_B);
    result.zero();

    for (size_t i = 0; i < nrow_A; ++i) {
        for (size_t j = 0; j < ncol_B; ++j) {
            double sum = 0;
            for (size_t k = 0; k < ncol_A; ++k) {
                sum += A(i, k) * B(k, j);
            }
            result(i, j) = sum;
        }
    }

    return result;
}

Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_size) {
    size_t nrow_A = A.nrow();
    size_t ncol_A = A.ncol();
    size_t nrow_B = B.nrow();
    size_t ncol_B = B.ncol();

    if (ncol_A != nrow_B) {
        throw std::runtime_error("Matrix dimensions do not match for multiplication.");
    }

    Matrix result(nrow_A, ncol_B);
    result.zero();


    for (size_t ii = 0; ii < nrow_A; ii += tile_size) {
        for (size_t jj = 0; jj < ncol_B; jj += tile_size) {
            for (size_t kk = 0; kk < ncol_A; kk += tile_size) {
                for (size_t i = ii; i < std::min(ii + tile_size, nrow_A); ++i) {
                    for (size_t j = jj; j < std::min(jj + tile_size, ncol_B); ++j) {
                        double sum = 0;
                        for (size_t k = kk; k < std::min(kk + tile_size, ncol_A); ++k) {
                            sum += A(i, k) * B(k, j);
                        }
                        result(i, j) += sum;
                    }
                }
            }
        }
    }

    return result;
}

Matrix multiply_mkl(const Matrix &A, const Matrix &B)
{
    size_t nrow_A = A.nrow();
    size_t ncol_A = A.ncol();
    size_t nrow_B = B.nrow();
    size_t ncol_B = B.ncol();

    if (ncol_A != nrow_B) {
        throw std::runtime_error("Matrix dimensions do not match for multiplication.");
    }

    Matrix result(nrow_A, ncol_B);
    result.zero();

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        A.nrow(),
        B.ncol(),
        A.ncol(),
        1.0,
        A.buffer(),
        A.ncol(),
        B.buffer(),
        B.ncol(),
        0.0,
        result.buffer(),
        result.ncol());

    return result;
}
