#include <mkl/mkl.h>
#include <mkl/mkl_lapack.h>
#include <mkl/mkl_lapacke.h>

#include "stdexcept"
#include "Multiply.h"

void check_matrix(const Matrix &mat1, const Matrix &mat2)
{
    if (mat1.ncol() != mat2.nrow())
        throw std::invalid_argument("Matrix: the number of columns in the first matrix must be equal to the number of rows in the second matrix");
}

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2)
{
    check_matrix(mat1, mat2);

    Matrix rst(mat1.nrow(), mat2.ncol());

    for (size_t i = 0; i < mat1.nrow(); ++i)
    {
        for (size_t j = 0; j < mat2.ncol(); ++j)
        {
            double v = 0;
            for (size_t k = 0; k < mat1.ncol(); ++k)
                v += mat1(i, k) * mat2(k, j);
            rst(i, j) = v;
        }
    }

    return rst;
}

Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, size_t tsize)
{
    check_matrix(mat1, mat2);

    size_t nrow_1 = mat1.nrow();
    size_t nrow_2 = mat2.nrow();
    size_t ncol_1 = mat1.ncol();
    size_t ncol_2 = mat2.ncol();

    // Get tsize * ceil(size / tsize)
    // which is the smallest multiple of tsize than size
    auto fix = [&](size_t size)
    {
        return (size + tsize - 1) / tsize * tsize;
    };

    size_t nrow_1_fix = fix(nrow_1);
    size_t nrow_2_fix = fix(nrow_2);
    size_t ncol_1_fix = fix(ncol_1);
    size_t ncol_2_fix = fix(ncol_2);

    // Set dest with src
    // they are align by (0, 0) with size (nrow, ncol)
    auto set = [&](Matrix &dest, const Matrix &src, size_t nrow, size_t ncol)
    {
        for (size_t i = 0; i < nrow; ++i)
            for (size_t j = 0; j < ncol; ++j)
                dest(i, j) = src(i, j);
    };

    Matrix mat1_fix(nrow_1_fix, ncol_1_fix);
    Matrix mat2_fix(nrow_2_fix, ncol_2_fix);
    Matrix rst_fix(nrow_1_fix, ncol_2_fix);

    set(mat1_fix, mat1, nrow_1, ncol_1);
    set(mat2_fix, mat2, nrow_2, ncol_2);

    const size_t buffer_size = tsize * tsize;
    double *block_1 = new double[buffer_size];
    double *block_2 = new double[buffer_size];
    double *block_rst = new double[buffer_size];
    double *block_sum = new double[buffer_size];

    // Set a block to all zeros
    auto clear = [&](double *arr)
    {
        for (size_t i = 0; i < buffer_size; ++i)
            arr[i] = 0.0;
    };

    // Add src block to dest block
    auto add = [&](double *dest, double *src)
    {
        for (size_t i = 0; i < buffer_size; ++i)
            dest[i] += src[i];
    };

    // Load mat to arr with offset
    // if transpose is set, let arr store mat^T
    auto load = [&](double *arr, const Matrix &mat, size_t r_offset, size_t c_offset, bool transpose)
    {
        if (transpose)
        {
            for (size_t j = 0, jo = c_offset; j < buffer_size; j += tsize, ++jo)
                for (size_t i = 0, io = r_offset; i < tsize; ++i, ++io)
                    arr[j + i] = mat(io, jo);
        }
        else
        {
            for (size_t i = 0, io = r_offset; i < buffer_size; i += tsize, ++io)
                for (size_t j = 0, jo = c_offset; j < tsize; ++j, ++jo)
                    arr[i + j] = mat(io, jo);
        }
    };

    // Inverse operation of load
    auto save = [&](const double *arr, Matrix &mat, size_t r_offset, size_t c_offset)
    {
        for (size_t i = 0, io = r_offset; i < buffer_size; i += tsize, ++io)
            for (size_t j = 0, jo = c_offset; j < tsize; ++j, ++jo)
                mat(io, jo) = arr[i + j];
    };

    // naive multiply
    // for block
    auto multiply = [&](double *rst, const double *b1, const double *b2)
    {
        for (size_t iBase = 0, i = 0; iBase < buffer_size; iBase += tsize, ++i)
        {
            for (size_t jBase = 0, j = 0; jBase < buffer_size; jBase += tsize, ++j)
            {
                double v = 0;
                for (size_t k = 0; k < tsize; ++k)
                    v += b1[iBase + k] * b2[jBase + k];
                rst[iBase + j] = v;
            }
        }
    };

    for (size_t i = 0; i < nrow_1_fix; i += tsize)
    {
        for (size_t j = 0; j < ncol_2_fix; j += tsize)
        {
            clear(block_sum);
            for (size_t k = 0; k < ncol_1_fix; k += tsize)
            {
                load(block_1, mat1_fix, i, k, false);
                load(block_2, mat2_fix, k, j, true);
                multiply(block_rst, block_1, block_2);
                add(block_sum, block_rst);
            }
            save(block_sum, rst_fix, i, j);
        }
    }

    delete[] block_1;
    delete[] block_2;
    delete[] block_sum;
    delete[] block_rst;

    Matrix rst(nrow_1, ncol_2);
    set(rst, rst_fix, nrow_1, ncol_2);

    return rst;
}

Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2)
{
    check_matrix(mat1, mat2);

    Matrix rst(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        mat1.nrow(),
        mat2.ncol(),
        mat1.ncol(),
        1.0,
        mat1.buffer().data(),
        mat1.ncol(),
        mat2.buffer().data(),
        mat2.ncol(),
        0.0,
        rst.buffer().data(),
        rst.ncol());

    return rst;
}