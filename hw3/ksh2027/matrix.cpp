#include <iostream>
#include <vector>
#include <mkl.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> //change python list to C++ vector<T>
namespace py = pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol), data(nrow * ncol){}

    ~Matrix(){}

    // No bound check.
    double   operator() (size_t row, size_t col) const{
        return data[row*m_ncol + col];
    }
    double & operator() (size_t row, size_t col){
        return data[row*m_ncol + col];
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

private:

    size_t m_nrow;
    size_t m_ncol;
    std::vector<double> data;

};

Matrix multiply_naive(Matrix const & A, Matrix const & B) {
    if (A.ncol() != B.nrow()){
        throw std::out_of_range("the number of first matrix column differs from that of second matrix row");
    }

    Matrix C(A.nrow(), B.ncol());
    for (int i = 0; i < A.nrow(); i++) {
        for (int j = 0; j < B.ncol(); j++) {
            double sum = 0.0;
            for (int k = 0; k < A.ncol(); k++) {
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
    for (int i = 0; i < A.nrow(); i += tile_size) {
        for (int j = 0; j < B.ncol(); j += tile_size) {
            for (int k = 0; k < A.ncol(); k += tile_size) {
                for (int ii = i; ii < std::min(i + tile_size, A.nrow()); ii++) {
                    for (int jj = j; jj < std::min(j + tile_size, B.ncol()); jj++) {
                        double sum = 0.0;
                        for (int kk = k; kk < std::min(k + tile_size, A.ncol()); kk++) {
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
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A.nrow(), B.ncol(), A.ncol(), 1.0, A.data.data(), A.ncol(), B.data.data(), B.ncol(), 0.0, C.data.data(), C.ncol());
    return C;
}

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "pybind11 matrix multiplication";
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("__setitem__", [](Matrix& mat, std::pair<size_t, size_t> idx, double val){ mat(idx.first, idx.second) = val; })
        .def("__getitem__", [](const Matrix& mat, std::pair<size_t, size_t> idx){ return mat(idx.first, idx.second); })
        .def("__eq__", &Matrix::operator==)
        .def("__ne__", &Matrix::operator!=)
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol);

    m.def("multiply_naive", &multiply_naive, "naive matrix-matrix multiplication");
    m.def("multiply_tile", &multiply_tile, "matrix-matrix multiplication with tiling");
    // m.def("multiply_mkl", &multiply_mkl, "multiplication by using DGEMM");
}