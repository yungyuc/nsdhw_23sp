#ifndef PYTHON_MULTIPLY_H_INCLUDED
#define PYTHON_MULTIPLY_H_INCLUDED

#include "PythonMatrix.h"

PythonMatrix multiply_naive(const PythonMatrix &mat1, const PythonMatrix &mat2);
PythonMatrix multiply_tile(const PythonMatrix &mat1, const PythonMatrix &mat2);
PythonMatrix multiply_mkl(const PythonMatrix &mat1, const PythonMatrix &mat2);

#endif