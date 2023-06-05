#include<bits/stdc++.h>
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/operators.h"
#include <mkl/mkl.h>
#include <mkl/mkl_cblas.h>
#include <mkl/mkl_lapack.h>
#include <mkl/mkl_lapacke.h>
using namespace std;
namespace py=pybind11;

template <class T>
class CustomAllocator{
public:
    using value_type = T;
    CustomAllocator() = default;
    
    T* allocate(size_t n){
        if (n > numeric_limits<size_t>::max()/sizeof(T)) throw bad_alloc();
        m_allocated += n*sizeof(T);
        m_byte += n*sizeof(T);
        T *ret = (T *)(malloc(n*sizeof(T)));
        if(ret == nullptr)
            throw std::bad_alloc();
        return ret;
    }
    void deallocate(T *ptr, size_t n){
        m_deallocated += n*sizeof(T);
        m_byte -= n*sizeof(T);
        free(ptr);
    }
    static size_t allocated(){
        return m_allocated;
    }
    static size_t deallocated(){
        return m_deallocated;
    }
    static size_t bytes(){
        return m_byte;
    }
private:
    static size_t m_allocated, m_deallocated , m_byte;
};

template <class T> size_t CustomAllocator<T>::m_allocated = 0;
template <class T> size_t CustomAllocator<T>::m_deallocated = 0;
template <class T> size_t CustomAllocator<T>::m_byte = 0;


class Matrix {
public:
    Matrix(): m_rows(0), m_cols(0), m_data(vector<double, CustomAllocator<double>>(0)) {}
    Matrix(int nrow, int ncol): m_rows(nrow), m_cols(ncol), m_data(vector<double, CustomAllocator<double>>(nrow * ncol)) {}
    double& operator()(int i, int j) {
        return m_data[i*m_cols+j];
    }
    const double& operator()(int i, int j) const {
        return m_data[i*m_cols+j];
    }
    
    bool operator ==(const Matrix &m) const
    {
        if (m_rows != m.nrow() || m_cols != m.ncol())
            return false;

        for (int i = 0; i < m_rows; i++){
            for (int j = 0; j < m_cols; j++){
                if (this->operator()(i, j) != m(i, j))
                    return false;
            }
        }

        return true;
    }
    int nrow() const { 
        return m_rows; 
    }
    int ncol() const { 
        return m_cols; 
    }
    double* data() { 
        return m_data.data(); 
    }
    const double* data() const { 
        return m_data.data(); 
    }

private:
    int m_rows, m_cols;
    vector<double, CustomAllocator<double>> m_data;
};

Matrix multiply_naive(Matrix const &m1, Matrix const &m2){
    Matrix ret(m1.nrow(), m2.ncol());
    for (int i = 0; i < m1.nrow(); i++) {
        for (int j = 0; j < m2.ncol(); j++) {
            double sum = 0.0;
            for (int k = 0; k < m1.ncol(); k++) {
                sum += m1(i, k) * m2(k, j);
            }
            ret(i, j) = sum;
        }
    }
    return ret;
}

Matrix multiply_tile(Matrix const &m1, Matrix const &m2, int const tile_size){
    int m = m1.nrow();
    int n = m2.ncol();
    int k = m1.ncol();

    Matrix ret(m, n);
    for (int i0 = 0; i0 < m; i0 += tile_size) {
        int imax = std::min(i0 + tile_size, m);
        for (int j0 = 0; j0 < n; j0 += tile_size) {
            int jmax = std::min(j0 + tile_size, n);
            for (int k0 = 0; k0 < k; k0 += tile_size) {
                int kmax = std::min(k0 + tile_size, k);
                for (int i = i0; i < imax; i++) {
                    for (int j = j0; j < jmax; j++) {
                        double sum = 0.0;
                        for (int l = k0; l < kmax; l++) {
                            sum += m1(i, l) * m2(l, j);
                        }
                        ret(i, j) += sum;
                    }
                }
            }
        }
    }

    return ret;
    
}

Matrix multiply_mkl(Matrix const &m1, Matrix const &m2){
    mkl_set_num_threads(1);
    Matrix ret(m1.nrow(), m2.ncol());
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.nrow(),  m2.ncol(), m1.ncol(), 1.0 , m1.data(),
     m1.ncol(), m2.data(), m2.ncol(), 0.0, const_cast<double*>(ret.data()), ret.ncol());
    return ret;
}


PYBIND11_MODULE(_matrix, m){
    m.doc() = "matrix-matrix multiplication";
    m.def("multiply_naive", &multiply_naive, "naive");
    m.def("multiply_tile", &multiply_tile, "tile");
    m.def("multiply_mkl", &multiply_mkl, "mkl");
    m.def("bytes", &CustomAllocator<double>::bytes);
    m.def("allocated", &CustomAllocator<double>::allocated);
    m.def("deallocated", &CustomAllocator<double>::deallocated);
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<>())
        .def(py::init<int, int>())
        .def("data", pybind11::overload_cast<>(&Matrix::data))
        .def("data", pybind11::overload_cast<>(&Matrix::data, pybind11::const_))
        .def_property_readonly("array", [](Matrix const &mat){
            return py::array_t<double>({mat.nrow(), mat.ncol()}, {mat.ncol() * sizeof(double), sizeof(double)}, mat.data(), py::cast(mat));
        })

        
        .def_property_readonly("nrow", [](const Matrix &mat) { return mat.nrow(); })
        .def_property_readonly("ncol", [](const Matrix &mat) { return mat.ncol(); })
        .def("__eq__", [](const Matrix &a, const Matrix &b) { 
            return a == b; })
        .def("__setitem__", [](Matrix &self, std::pair<int, int> idx, double val) {
            self(idx.first, idx.second) = val;
        })
        .def("__getitem__", [](const Matrix &self, std::pair<int, int> idx) {
            return self(idx.first, idx.second);
        });
}