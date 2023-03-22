#ifndef MATRIX_H
#define MATRIX_H

#include <bits/stdc++.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

class Matrix
{
public:
    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = (double *) calloc(nelement, sizeof(double));
    }

    ~Matrix() { free(m_buffer); }

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

private:
    size_t m_nrow;
    size_t m_ncol;
    double *m_buffer;
};

#endif  // MATRIX_H
