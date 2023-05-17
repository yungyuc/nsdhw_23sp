#include <iostream>
#include <vector>
#include <mkl.h>

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol), data_v(nrow * ncol){}

    ~Matrix(){}

    // No bound check.
    double   operator() (size_t row, size_t col) const{
        return data_v[row*m_ncol + col];
    }
    double & operator() (size_t row, size_t col){
        return data_v[row*m_ncol + col];
    }
    bool operator == (const Matrix& other) const{
        if(nrow() != other.nrow() || ncol() != other.ncol()) {return 0;}
        for(size_t i=0; i<m_nrow; i++)
            for(size_t j=0; j<m_ncol; j++)
                if( (*this)(i,j) != other(i,j) )
                    return 0;
        return 1;
    }
    bool operator != (const Matrix& other) const{
        return !(*this == other);
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    const double* data() const { return data_v.data(); }

private:

    size_t m_nrow;
    size_t m_ncol;
    std::vector<double> data_v;

};

Matrix multiply_naive(Matrix const & A, Matrix const & B) {
    if (A.ncol() != B.nrow()){
        throw std::out_of_range("the number of first matrix column differs from that of second matrix row");
    }

    Matrix C(A.nrow(), B.ncol());
    for (size_t i = 0; i < A.nrow(); i++) {
        for (size_t j = 0; j < B.ncol(); j++) {
            double sum = 0.0;
            for (size_t k = 0; k < A.ncol(); k++) {
                sum += A(i,k) * B(k,j);
            }
            C(i,j) = sum;
        }
    }
    return C;
}

Matrix multiply_tile(const Matrix& A, const Matrix& B, size_t tile_size) {
    if (A.ncol() != B.nrow()) {
        throw std::out_of_range("the number of first matrix column differs from that of second matrix row");
    }

    Matrix C(A.nrow(), B.ncol());
    for (size_t i = 0; i < A.nrow(); i += tile_size) {
        for (size_t j = 0; j < B.ncol(); j += tile_size) {
            for (size_t k = 0; k < A.ncol(); k += tile_size) {
                for (size_t ii = i; ii < std::min(i + tile_size, A.nrow()); ii++) {
                    for (size_t jj = j; jj < std::min(j + tile_size, B.ncol()); jj++) {
                        double sum = 0.0;
                        for (size_t kk = k; kk < std::min(k + tile_size, A.ncol()); kk++) {
                            sum += A(ii,kk) * B(kk,jj);
                        }
                        C(ii,jj) += sum;
                    }
                }
            }
        }
    }
    return C;
}

Matrix multiply_mkl(const Matrix& A, const Matrix& B) {
    if (A.ncol() != B.nrow()) {
        throw std::out_of_range("the number of first matrix column differs from that of second matrix row");
    }

    Matrix C(A.nrow(), B.ncol());
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A.nrow(), B.ncol(), A.ncol(), 1.0, A.data(), A.ncol(), B.data(), B.ncol(), 0.0, const_cast<double*>(C.data()), C.ncol());
    return C;
}