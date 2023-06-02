#include "Matrix.h"
#include <vector>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

py::array_t<double> Matrix::array()
{
    return py::array_t<double>({nrow(), ncol()},
        {sizeof(double) * ncol(), sizeof(double)},
        m_buffer.data(), py::cast(this));
}

Matrix::Matrix() {

}

Matrix::Matrix(size_t c, size_t r)
{
    m_col = c;
    m_row = r;
    m_buffer.resize(c * r);
    std::fill(m_buffer.begin(), m_buffer.end(), 0);
}

Matrix::~Matrix()
{
    m_col = 0;
    m_row = 0;
    m_buffer.clear();
}

const size_t & Matrix::nrow() const
{
    return m_row;
}

const size_t & Matrix::ncol() const
{
    return m_col;
}

double Matrix::operator()(size_t r, size_t c) const
{
    return m_buffer[r * m_col + c];
}

double & Matrix::operator()(size_t r, size_t c)
{
    return m_buffer[r * m_col + c];
}

double Matrix::operator()(size_t idx) const
{
    return m_buffer[idx];
}

double & Matrix::operator()(size_t idx)
{
    return m_buffer[idx];
}

bool Matrix::operator==(const Matrix &rhs) const
{
    if (m_col != rhs.ncol())
        return false;
    if (m_row != rhs.nrow())
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

Matrix & Matrix::operator=(const Matrix &rhs)
{
if (m_row != rhs.nrow() && m_col != rhs.ncol())
    {
        throw std::invalid_argument("Please don't tried to change the shape of the matrix");
    }
    size_t n = m_row * m_col;
    for (size_t i = 0; i < n; i++)
    {
        m_buffer[i] = rhs(i);
    }
    return *this;
}