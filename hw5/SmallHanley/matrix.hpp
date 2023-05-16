#pragma once
#include <bits/stdc++.h>
#include <mkl.h>

class Matrix
{
public:
    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement];
        memset(m_buffer, 0, nelement * sizeof(double));
    }

    Matrix(const Matrix &m) : m_nrow(m.nrow()), m_ncol(m.ncol())
    {
        size_t nelement = m_nrow * m_ncol;
        m_buffer = new double[nelement];
        memcpy(m_buffer, m.buffer(), nelement * sizeof(double));
    }

    ~Matrix() { delete[] m_buffer; }

    // No bound check.
    double operator()(size_t row, size_t col) const
    {
        return m_buffer[row * m_ncol + col];
    }
    double &operator()(size_t row, size_t col)
    {
        return m_buffer[row * m_ncol + col];
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

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    double *buffer() const { return m_buffer; }

private:
    size_t m_nrow;
    size_t m_ncol;
    double *m_buffer;
};

Matrix multiply_naive(Matrix &a, Matrix &b)
{
    if (a.ncol() != b.nrow()) {
        exit(1);
    }

    Matrix c(a.nrow(), b.ncol());
    for (size_t i = 0; i < a.nrow(); i++) {
        for (size_t j = 0; j < b.ncol(); j++) {
            for (size_t k = 0; k < a.ncol(); k++) {
                c(i, j) += a(i, k) * b(k, j);
            }
        }
    }
    return c;
}

Matrix multiply_tile(Matrix const &a, Matrix const &b, size_t const tile_size)
{
    if (a.ncol() != b.nrow()) {
        exit(1);
    }

    Matrix c(a.nrow(), b.ncol());
    for (size_t i = 0; i < a.nrow(); i += tile_size) {
        for (size_t k = 0; k < a.ncol(); k += tile_size) {
            for (size_t j = 0; j < b.ncol(); j += tile_size) {
                for (size_t ti = i; ti < std::min(i + tile_size, a.nrow());
                     ti++) {
                    for (size_t tk = k; tk < std::min(k + tile_size, a.ncol());
                         tk++) {
                        for (size_t tj = j;
                             tj < std::min(j + tile_size, b.ncol()); tj++) {
                            c(ti, tj) += a(ti, tk) * b(tk, tj);
                        }
                    }
                }
            }
        }
    }
    return c;
}

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{
    if (mat1.ncol() != mat2.nrow()) {
        exit(1);
    }

    mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(CblasRowMajor /* const CBLAS_LAYOUT Layout */
                ,
                CblasNoTrans /* const CBLAS_TRANSPOSE transa */
                ,
                CblasNoTrans /* const CBLAS_TRANSPOSE transb */
                ,
                mat1.nrow() /* const MKL_INT m */
                ,
                mat2.ncol() /* const MKL_INT n */
                ,
                mat1.ncol() /* const MKL_INT k */
                ,
                1.0 /* const double alpha */
                ,
                mat1.buffer() /* const double *a */
                ,
                mat1.ncol() /* const MKL_INT lda */
                ,
                mat2.buffer() /* const double *b */
                ,
                mat2.ncol() /* const MKL_INT ldb */
                ,
                0.0 /* const double beta */
                ,
                ret.buffer() /* double * c */
                ,
                ret.ncol() /* const MKL_INT ldc */
    );

    return ret;
}
