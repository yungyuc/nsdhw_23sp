#include "Multiply.h"
#include "PythonMultiply.h"

PythonMatrix multiply_naive(const PythonMatrix &mat1, const PythonMatrix &mat2)
{
    return multiply_naive((const Matrix &)mat1, (const Matrix &)mat2);
}

PythonMatrix multiply_tile(const PythonMatrix &mat1, const PythonMatrix &mat2, size_t tsize)
{
    return multiply_tile((const Matrix &)mat1, (const Matrix &)mat2, tsize);
}

PythonMatrix multiply_mkl(const PythonMatrix &mat1, const PythonMatrix &mat2)
{
    return multiply_mkl((const Matrix &)mat1, (const Matrix &)mat2);
}