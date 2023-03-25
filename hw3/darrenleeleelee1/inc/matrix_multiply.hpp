#pragma once
#include <iostream>
#include <mkl/mkl.h>
#include <mkl/mkl_lapack.h>
#include <mkl/mkl_lapacke.h>
#include "matrix.hpp"
namespace MatrixMultiply{
    Matrix multiply_naive(const Matrix& A, const Matrix& B);
    Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_size = 64);
    Matrix multiply_mkl(const Matrix& A, const Matrix& B);
}

Matrix MatrixMultiply::multiply_naive(const Matrix& A, const Matrix& B) {
    size_t nrow_A = A.nrow();
    size_t ncol_A = A.ncol();
    size_t nrow_B = B.nrow();
    size_t ncol_B = B.ncol();

    if (ncol_A != nrow_B) {
        throw std::runtime_error("Matrix dimensions do not match for multiplication.");
    }

    Matrix result(nrow_A, ncol_B);
    result.zero();

    for (size_t i = 0; i < nrow_A; ++i) {
        for (size_t j = 0; j < ncol_B; ++j) {
            double sum = 0;
            for (size_t k = 0; k < ncol_A; ++k) {
                sum += A(i, k) * B(k, j);
            }
            result(i, j) = sum;
        }
    }

    return result;
}

Matrix MatrixMultiply::multiply_tile(const Matrix& A, const Matrix& B, size_t tile_size) {
    size_t nrow_A = A.nrow();
    size_t ncol_A = A.ncol();
    size_t nrow_B = B.nrow();
    size_t ncol_B = B.ncol();

    if (ncol_A != nrow_B) {
        throw std::runtime_error("Matrix dimensions do not match for multiplication.");
    }

    Matrix result(nrow_A, ncol_B);
    result.zero();


    for (size_t ii = 0; ii < nrow_A; ii += tile_size) {
        for (size_t jj = 0; jj < ncol_B; jj += tile_size) {
            for (size_t kk = 0; kk < ncol_A; kk += tile_size) {
                for (size_t i = ii; i < std::min(ii + tile_size, nrow_A); ++i) {
                    for (size_t j = jj; j < std::min(jj + tile_size, ncol_B); ++j) {
                        double sum = 0;
                        for (size_t k = kk; k < std::min(kk + tile_size, ncol_A); ++k) {
                            sum += A(i, k) * B(k, j);
                        }
                        result(i, j) += sum;
                    }
                }
            }
        }
    }

    return result;
}

Matrix MatrixMultiply::multiply_mkl(const Matrix &A, const Matrix &B)
{
    size_t nrow_A = A.nrow();
    size_t ncol_A = A.ncol();
    size_t nrow_B = B.nrow();
    size_t ncol_B = B.ncol();

    if (ncol_A != nrow_B) {
        throw std::runtime_error("Matrix dimensions do not match for multiplication.");
    }

    Matrix result(nrow_A, ncol_B);
    result.zero();

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        A.nrow(),
        B.ncol(),
        A.ncol(),
        1.0,
        A.buffer(),
        A.ncol(),
        B.buffer(),
        B.ncol(),
        0.0,
        result.buffer(),
        result.ncol());

    return result;
}
