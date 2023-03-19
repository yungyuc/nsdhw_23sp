#include "Multiply.h"
#include "PythonMultiply.h"

PythonMatrix multiply_naive(const PythonMatrix &mat1, const PythonMatrix &mat2)
{
    return Multiply::multiply_naive(mat1, mat2);
}

PythonMatrix multiply_tile(const PythonMatrix &mat1, const PythonMatrix &mat2, size_t tsize)
{
    return Multiply::multiply_tile(mat1, mat2, tsize);
}

PythonMatrix multiply_mkl(const PythonMatrix &mat1, const PythonMatrix &mat2)
{
    return Multiply::multiply_mkl(mat1, mat2);
}