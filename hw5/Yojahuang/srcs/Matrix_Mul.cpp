#include "Matrix_Mul.h"
#include "Matrix.h"

#include <mkl/mkl.h>
#include <mkl/mkl_lapack.h>
#include <mkl/mkl_lapacke.h>
#include <cstddef>
#include <stdexcept>
#include <cstring>

Matrix &multiply_naive(Matrix &lhs, const Matrix &rhs)
{
    if (lhs.ncol() != rhs.nrow())
    {
        throw std::invalid_argument("The shape of matrix is not align");
    }

    size_t r = lhs.nrow(), c = rhs.ncol(), n = lhs.ncol();

    Matrix *result = new Matrix(r, c);

    for (size_t i = 0; i < r; ++i)
    {
        for (size_t j = 0; j < c; ++j)
        {
            for (size_t k = 0; k < n; ++k)
            {
                (*result)(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }
    return (*result);
}

Matrix &multiply_mkl(Matrix &mat1, Matrix &mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::invalid_argument("The shape of matrix is not align");
    }
    Matrix *ret = new Matrix(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        mat1.m_row,
        mat2.m_col,
        mat1.m_col,
        1.0,
        mat1.m_buffer.data(),
        mat1.m_col,
        mat2.m_buffer.data(),
        mat2.m_col,
        0.0,
        ret->m_buffer.data(),
        ret->m_col);

    return *ret;
}

void loadBlock(Matrix &m, double *buf, size_t r, size_t c, size_t blocksize)
{
    std::fill(buf, buf + blocksize * blocksize, 0);
    size_t rowStart = r * blocksize;
    size_t colStart = c * blocksize;

    size_t row, col;

    size_t idx = 0, cnt = 0;

    for (row = rowStart; row < std::min(rowStart + blocksize, m.nrow()); ++row)
    {
        for (col = colStart; col < std::min(colStart + blocksize, m.ncol()); ++col)
        {
            buf[idx] = m(row, col);
            idx++;
        }
        cnt++;
        idx = cnt * blocksize;
    }
}

void mulBlock(double *lhsBuf, double *rhsBuf, size_t blocksize, double *resultBuf)
{
    std::fill(resultBuf, resultBuf + blocksize * blocksize, 0);
    for (size_t i = 0; i < blocksize; ++i)
    {
        for (size_t k = 0; k < blocksize; ++k)
        {
            for (size_t j = 0; j < blocksize; ++j)
            {
                resultBuf[i * blocksize + j] += lhsBuf[i * blocksize + k] * rhsBuf[k * blocksize + j];
            }
        }
    }
}

void saveBuf(double *resultBuf, Matrix *result, size_t blocksize, size_t r, size_t c)
{
    size_t rowStart = r * blocksize;
    size_t colStart = c * blocksize;

    size_t row, col;

    size_t idx = 0, cnt = 0;

    for (row = rowStart; row < std::min(rowStart + blocksize, (*result).nrow()); ++row)
    {
        for (col = colStart; col < std::min(colStart + blocksize, (*result).ncol()); ++col)
        {
            (*result)(row, col) += resultBuf[idx];
            idx++;
        }
        cnt++;
        idx = cnt * blocksize;
    }
}

Matrix &multiply_tile(Matrix &lhs, Matrix &rhs, size_t blocksize)
{
    if (lhs.ncol() != rhs.nrow())
    {
        throw std::invalid_argument("The shape of matrix is not align");
    }

    Matrix *result = new Matrix(lhs.nrow(), rhs.ncol());

    double lhsBuf[blocksize * blocksize];

    double rhsBuf[blocksize * blocksize];

    double resultBuf[blocksize * blocksize];

    size_t lhsRow = lhs.nrow() / blocksize + (lhs.nrow() % blocksize > 0);
    size_t lhsCol = lhs.ncol() / blocksize + (lhs.ncol() % blocksize > 0);

    size_t rhsCol = rhs.ncol() / blocksize + (rhs.ncol() % blocksize > 0);

    for (size_t i = 0; i < lhsRow; ++i)
    {
        for (size_t j = 0; j < rhsCol; ++j)
        {
            for (size_t k = 0; k < lhsCol; ++k)
            {
                loadBlock(lhs, lhsBuf, i, k, blocksize);
                loadBlock(rhs, rhsBuf, k, j, blocksize);
                mulBlock(lhsBuf, rhsBuf, blocksize, resultBuf);
                saveBuf(resultBuf, result, blocksize, i, j);
            }
        }
    }

    return *result;
}