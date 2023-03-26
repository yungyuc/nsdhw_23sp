#include "_matrix.hpp"
#include <cstddef>
#include <algorithm>

Matrix multiply_naive(Matrix const & matrix1, Matrix const & matrix2){
    Matrix ret(matrix1.nrow(), matrix2.ncol());
    for (size_t row = 0; row < ret.nrow(); row++){
        for (size_t column = 0; column < ret.ncol(); column++){
            double value = 0;
            for (size_t k = 0; k < matrix1.ncol(); k++){
                value += matrix1(row, k) * matrix2(k, column);
            }
            ret(row, column) = value;
        }
    }
    return ret;
}

Matrix multiply_tile(Matrix const & matrix1, Matrix const & matrix2, size_t tilesize){
    Matrix ret(matrix1.nrow(), matrix2.ncol());
    for (size_t row = 0; row < matrix1.nrow(); row += tilesize){
        for (size_t col = 0; col < matrix2.ncol(); col += tilesize){
            for (size_t inner = 0; inner < matrix1.ncol(); inner += tilesize){
                for (size_t k = inner; k < std::min(matrix1.ncol(), inner + tilesize); k++){
                    for (size_t i = row; i < std::min(matrix1.nrow(), row + tilesize); i++){
                        for (size_t j = col; j < std::min(matrix2.ncol(), col + tilesize); j++){
                            ret(i, j) += matrix1(i, k) * matrix2(k, j);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

Matrix multiply_mkl(Matrix const & matrix1, Matrix const & matrix2){
    Matrix ret = Matrix(matrix1.nrow(), matrix2.ncol());
    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        matrix1.nrow(),
        matrix2.ncol(),
        matrix1.ncol(),
        1.0,
        matrix1.get_matrix_buffer(),
        matrix1.ncol(),
        matrix2.get_matrix_buffer(),
        matrix2.ncol(),
        0.0,
        ret.get_matrix_buffer(),
        ret.ncol()
    );

    return ret;
}

void generateValue(Matrix& A){
    size_t n = A.nrow();
    size_t m = A.ncol();
    for (size_t i = 0 ; i < n ; ++i){
        for (size_t j = 0 ; j < m ; ++j){
            double value = rand() % RANDMAX;
            A(i, j) = value;
        }
    }
}