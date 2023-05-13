#ifndef MATRIX_H
#define MATRIX_H

#include <bits/stdc++.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "alloc.hpp"

MyAllocator<double> alloc;

class Matrix
{
    friend Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
    friend Matrix multiply_tile(Matrix const &mat1,
                                Matrix const &mat2,
                                size_t const tsize);
    friend Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2);
    friend bool operator==(Matrix const &mat1, Matrix const &mat2);

public:
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol), m_buffer(alloc)
    {
        reset_buffer(nrow, ncol);
    }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    void reset_buffer(size_t nrow, size_t ncol)
    {
        m_buffer.reserve(nrow * ncol);
        m_nrow = nrow;
        m_ncol = ncol;
    }
    double operator()(size_t row, size_t col) const
    {
        return m_buffer[row * m_ncol + col];
    }
    double &operator()(size_t row, size_t col)
    {
        return m_buffer[row * m_ncol + col];
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    std::vector<double, MyAllocator<double>> m_buffer;
};

#endif  // MATRIX_H
