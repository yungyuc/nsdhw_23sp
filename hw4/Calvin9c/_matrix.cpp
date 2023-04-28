#include <vector>
#include <mkl.h>
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "allocator.hpp"

namespace py = pybind11;

static CustomAllocator<double> allocator;

class Matrix{

    friend Matrix multiply_naive(Matrix const &mat1, Matrix const &mat2);
    friend Matrix multiply_tile(Matrix const &mat1, Matrix const &mat2, size_t const tsize);
    friend Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2);
    
    public:
        
        Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol), m_buffer(allocator){
            reset_buffer(nrow, ncol);
        }

        size_t index(size_t row, size_t col) const {
            return row * m_ncol + col;
        }

        size_t nrow() const { return m_nrow; }
        size_t ncol() const { return m_ncol; }

        double operator()(size_t row, size_t col) const {
            return m_buffer[index(row, col)];
        }
        
        double &operator()(size_t row, size_t col){
            return m_buffer[index(row, col)];
        }

        bool operator==(Matrix const &other) const {

            if((m_ncol != other.ncol()) && (m_nrow != other.ncol())) return false;

            for (size_t i = 0; i < m_nrow; ++i){
                for (size_t j = 0; j < m_ncol; ++j){
                    if (m_buffer[index(i, j)] != other(i, j)) return false;
                }
            }
            return true;
        }

        void reset_buffer(size_t nrow, size_t ncol){
            m_buffer.reserve(nrow*ncol);	
            m_nrow = nrow;
            m_ncol = ncol;
        }

        size_t m_nrow = 0;

        size_t m_ncol = 0;

        std::vector<double, CustomAllocator<double>> m_buffer ;

};

Matrix multiply_naive(const Matrix &mat1, const Matrix &mat2){
    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t i = 0; i < mat1.nrow(); ++i){
        for (size_t k = 0; k < mat2.ncol(); ++k){
            double result = 0;
            for (size_t j = 0; j < mat1.ncol(); ++j){
                result += mat1(i, j) * mat2(j, k);
            }
            ret(i, k) = result;
        }
    }
    return ret;
}

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2){

    mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor, /* const CBLAS_LAYOUT Layout */
        CblasNoTrans, /* const CBLAS_TRANSPOSE transa */
        CblasNoTrans, /* const CBLAS_TRANSPOSE transb */
        mat1.nrow(), /* const MKL_INT m */
        mat2.ncol(), /* const MKL_INT n */
        mat1.ncol(), /* const MKL_INT k */
        1.0, /* const double alpha */
        mat1.m_buffer.data(), /* const double *a */
        mat1.ncol(), /* const MKL_INT lda */
        mat2.m_buffer.data(), /* const double *b */
        mat2.ncol(), /* const MKL_INT ldb */
        0.0, /* const double beta */
        ret.m_buffer.data(), /* double * c */
        ret.ncol() /* const MKL_INT ldc */
    );
    return ret;
}

Matrix multiply_tile(const Matrix &mat1, const Matrix &mat2, size_t tsize){

    Matrix ret(mat1.nrow(), mat2.ncol());
    for (size_t o_i = 0; o_i < mat1.nrow(); o_i += tsize){
        for (size_t o_j = 0; o_j < mat2.ncol(); o_j += tsize){
            for (size_t o_k = 0; o_k < mat2.ncol(); o_k += tsize){
                for (size_t i = o_i; i < std::min(o_i + tsize, mat1.nrow()); i++){
                    for (size_t j = o_j; j < std::min(o_j + tsize, mat2.ncol()); j++){
                        for (size_t k = o_k; k < std::min(tsize + o_k, mat1.nrow()); k++){ ret(i, j) += mat1(i, k) * mat2(k, j); }
                    }
                }
            }
        }
    }
    return ret;
}

std::size_t bytes() { return allocator.bytes(); }
std::size_t allocated() { return allocator.allocated(); }
std::size_t deallocated() { return allocator.deallocated(); }

PYBIND11_MODULE(_matrix, m){
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
    m.def("bytes", &bytes);
    m.def("allocated", &allocated);
    m.def("deallocated", &deallocated);
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> index, double value) {self(index.first, index.second) = value;})
        .def("__getitem__", [](Matrix &self, std::pair<size_t, size_t> index) {return self(index.first, index.second);})
        .def("__eq__", [](const Matrix &mat, const Matrix &other) { return mat == other; })
        .def_property_readonly("nrow",&Matrix::nrow)
        .def_property_readonly("ncol",&Matrix::nrow);
}