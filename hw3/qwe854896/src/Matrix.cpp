#include <cstring>
#include "Matrix.h"

Matrix::Matrix(const size_t &nrow, const size_t &ncol)
    : m_nrow(nrow), m_ncol(ncol)
{
    size_t nelement = nrow * ncol;
    m_buffer = new double[nelement];
    for (size_t i = 0; i < nelement; ++i)
        m_buffer[i] = 0.0;
}

Matrix::Matrix(const Matrix &mat)
    : Matrix(mat.m_nrow, mat.m_ncol)
{
    size_t nelement = m_nrow * m_ncol;
    m_buffer = new double[nelement];
    memcpy(m_buffer, mat.m_buffer, sizeof(double) * nelement);
}

Matrix::~Matrix()
{
    delete[] m_buffer;
    m_buffer = nullptr;
}

// No bound check.
double Matrix::operator()(const size_t &row, const size_t &col) const
{
    return m_buffer[row * m_ncol + col];
}
double &Matrix::operator()(const size_t &row, const size_t &col)
{
    return m_buffer[row * m_ncol + col];
}

size_t Matrix::nrow() const { return m_nrow; }
size_t Matrix::ncol() const { return m_ncol; }

bool Matrix::operator==(const Matrix &other) const
{
    if (nrow() != other.nrow() || ncol() != other.ncol())
        return false;
    for (size_t i = 0; i < nrow(); ++i)
        for (size_t j = 0; j < ncol(); ++j)
            if (operator()(i, j) != other(i, j))
                return false;
    return true;
}
bool Matrix::operator!=(const Matrix &other) const
{
    return !(*this == other);
}
