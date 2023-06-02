#ifndef MATRIX_MUL_H_INCLUDED
#define MATRIX_MUL_H_INCLUDED

#include "Matrix.h"

Matrix &multiply_naive(Matrix &lhs, const Matrix &rhs);
Matrix &multiply_tile(Matrix &lhs, Matrix &rhs, size_t blocksize);
Matrix &multiply_mkl(Matrix &mat1, Matrix &mat2);

#endif