#include "stdexcept"
#include "Multiply.h"

Matrix Multiply::multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.ncol() != mat2.nrow())
        throw std::invalid_argument("Matrix: the number of columns in the first matrix must be equal to the number of rows in the second matrix");

    Matrix rst(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); ++i)
        for (size_t j = 0; j < mat2.ncol(); ++j)
            for (size_t k = 0; k < mat1.ncol(); ++k)
                rst(i, j) += mat1(i, k) * mat2(k, j);

    return rst;
}

Matrix Multiply::multiply_tile(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.ncol() != mat2.nrow())
        throw std::invalid_argument("Matrix: the number of columns in the first matrix must be equal to the number of rows in the second matrix");

    Matrix rst(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); ++i)
        for (size_t j = 0; j < mat2.ncol(); ++j)
            for (size_t k = 0; k < mat1.ncol(); ++k)
                rst(i, j) += mat1(i, k) * mat2(k, j);

    return rst;
}

Matrix Multiply::multiply_mkl(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.ncol() != mat2.nrow())
        throw std::invalid_argument("Matrix: the number of columns in the first matrix must be equal to the number of rows in the second matrix");

    Matrix rst(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); ++i)
        for (size_t j = 0; j < mat2.ncol(); ++j)
            for (size_t k = 0; k < mat1.ncol(); ++k)
                rst(i, j) += mat1(i, k) * mat2(k, j);

    return rst;
}