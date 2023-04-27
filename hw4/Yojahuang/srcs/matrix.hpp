#ifndef MATRIX_H
#define MATRIX_H

#include <limits>
#include <cstddef>
#include <vector>
#include "counter_and_allocator.hpp"

class Matrix
{
public:
    Matrix()
    {
    }
    Matrix(size_t c, size_t r)
    {
        m_col = c;
        m_row = r;
    }

    ~Matrix()
    {
        m_col = 0;
        m_row = 0;
        m_buffer.clear();
    }

    const size_t &n_row() const
    {
        return m_row;
    }

    const size_t &n_col() const
    {
        return m_col;
    }

    double operator()(size_t r, size_t c) const
    {
        return m_buffer[r * m_col + c];
    }

    double &operator()(size_t r, size_t c)
    {
        return m_buffer[r * m_col + c];
    }

    bool operator==(const Matrix &rhs) const
    {
        if (m_col != rhs.n_col())
            return false;
        if (m_row != rhs.n_row())
            return false;

        for (size_t i = 0; i < m_row; ++i)
        {
            for (size_t j = 0; j < m_col; ++j)
            {
                if (m_buffer[i * m_col + j] != rhs(i, j))
                    return false;
            }
        }
        return true;
    }

    friend Matrix multiply_mkl(Matrix const &, Matrix const &);

private:
    size_t m_col, m_row;
    std::vector<double, CustomAllocator<double>> m_buffer;
}; /* end class Line */

#endif