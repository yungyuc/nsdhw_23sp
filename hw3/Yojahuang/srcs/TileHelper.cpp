#include "TileHelper.h"
#include <cstddef>

Matrix TileHelper::padMatrix(size_t blocksize, const Matrix& originalMatrix) {
    size_t row, col;
    row = originalMatrix.n_row() + ((blocksize - (originalMatrix.n_row() % blocksize)) % blocksize);
    col = originalMatrix.n_col() + ((blocksize - (originalMatrix.n_col() % blocksize)) % blocksize);
    
    Matrix result(row, col);
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            if (i >= originalMatrix.n_row() || j >= originalMatrix.n_col()) result(i, j) = 0;
            else result(i,j) = originalMatrix(i, j);
        }
    }
    return result;
}