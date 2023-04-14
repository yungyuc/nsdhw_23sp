#include <cstring>
#include <cstddef>
#include <mkl.h>
#include <algorithm>

class Matrix{
    public:
        Matrix(size_t init_row, size_t init_col):number_row(init_row),number_col(init_col){
            matrix_buffer = new double[init_row*init_col]();
        }
        ~Matrix(){
            delete [] matrix_buffer;
        }
        Matrix(const Matrix &matrix){
            number_row = matrix.number_row;
            number_col = matrix.number_col;
            matrix_buffer = new double[number_row * number_col](); 
            memcpy(matrix_buffer, matrix.matrix_buffer, number_row * number_col*sizeof(double));
        }
        bool operator == (Matrix const &matrix) const {
            for (size_t i = 0; i < number_row; i++) 
                for (size_t j = 0; j < number_col; j++)
                    if (matrix_buffer[i*number_col + j] != matrix(i, j)) 
                        return false;
            return true;
        }
        Matrix& operator= (const Matrix& matrix) {
            matrix_buffer = matrix.matrix_buffer;
            return *this;
        }
        double  operator() (size_t row, size_t col) const { 
            return matrix_buffer[row*number_col + col]; 
        }
        double & operator() (size_t row, size_t col){ 
            return matrix_buffer[row*number_col + col]; 
        }
        double * get_matrix_buffer() const { 
            return matrix_buffer; 
        }
        size_t nrow() const { return number_row; }
        size_t ncol() const { return number_col; }

    private:
        size_t number_row;
        size_t number_col;
        double *matrix_buffer;
};


Matrix multiply_naive(Matrix const & matrix1, Matrix const & matrix2);
Matrix multiply_tile(Matrix const & matrix1, Matrix const & matrix2, size_t tilesize);
Matrix multiply_mkl(Matrix const & matrix1, Matrix const & matrix2);
Matrix generate_num(Matrix& A);
void check_matrix_multiply(Matrix const & matrix1, Matrix const & matrix2);
