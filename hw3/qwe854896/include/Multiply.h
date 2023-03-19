#ifndef MULTIPLY_H_INCLUDED
#define MULTIPLY_H_INCLUDED

#include "Matrix.h"

namespace Multiply
{
    Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2);
    Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2);
    Matrix multiply_mkl(const Matrix &mat1, const Matrix &mat2);
}

#endif