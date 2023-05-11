#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <cstddef>
#include <vector>

#include "CustomAllocator.h"

class Matrix
{
public:
    Matrix(const Matrix &mat);
    Matrix(const size_t &nrow, const size_t &ncol);
    ~Matrix();

    double operator()(const size_t &row, const size_t &col) const;
    double &operator()(const size_t &row, const size_t &col);

    size_t nrow() const;
    size_t ncol() const;
    std::vector<double, CustomAllocator<double>> &buffer();
    const std::vector<double, CustomAllocator<double>> &buffer() const;

    bool operator==(const Matrix &other) const;
    bool operator!=(const Matrix &other) const;

private:
    size_t m_nrow;
    size_t m_ncol;
    std::vector<double, CustomAllocator<double>> m_buffer;
};

#endif