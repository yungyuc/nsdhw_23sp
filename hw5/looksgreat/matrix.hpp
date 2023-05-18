#include<bits/stdc++.h>
#include <mkl.h>
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/operators.h"
using namespace std;
namespace py=pybind11;
class Matrix {


public:
    Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols), m_data(new double[rows*cols]){
        memset(m_data, 0, sizeof(double)*rows*cols);
    }
    Matrix(const Matrix &m) : m_cols(m.ncol()), m_rows(m.nrow()){
        m_data = new double[m_rows*m_cols];
        for (size_t i = 0; i < m_rows; i++){
            for (size_t j = 0; j < m_cols; j++){
                m_data[i*m_cols+j] = m(i, j);
            }
        }
    }
    ~Matrix(){
        delete[] m_data;
    }
    double &operator()(size_t x, size_t y){
        return m_data[y*m_cols+x];
    }
    double operator()(size_t x, size_t y) const{
        return m_data[y*m_cols+x];
    }
    bool operator ==(const Matrix &m) const{
        if (m_rows != m.nrow() || m_cols != m.ncol()){
            return false;
        }
        for (size_t i = 0; i < m_rows; i++){
            for (size_t j = 0; j < m_cols; j++){
                if ((*this)(i, j) != m(i, j)){
                    return false;
                }
            }
        }
        return true;
    }
    size_t nrow() const{ 
        return m_rows; 
    }
    size_t ncol() const{ 
        return m_cols;
    }
    double* data() const{
        return m_data;
    }

private:
    size_t m_rows;
    size_t m_cols;
    double *m_data;
};

Matrix multiply_naive(Matrix const &m1, Matrix const &m2){
    Matrix ret(m1.nrow(), m2.ncol());
    for (size_t i = 0; i < m1.nrow(); i++) {
        for (size_t j = 0; j < m2.ncol(); j++) {
            double sum = 0.0;
            for (size_t k = 0; k < m1.ncol(); k++) {
                sum += m1(i, k) * m2(k, j);
            }
            ret(i, j) = sum;
        }
    }
    return ret;
}

Matrix multiply_tile(Matrix const &m1, Matrix const &m2, size_t const tile_size){
    size_t m = m1.nrow();
    size_t n = m2.ncol();
    size_t k = m1.ncol();

    Matrix ret(m, n);
    for (size_t i0 = 0; i0 < m; i0 += tile_size) {
        size_t imax = std::min(i0 + tile_size, m);
        for (size_t j0 = 0; j0 < n; j0 += tile_size) {
            size_t jmax = std::min(j0 + tile_size, n);
            for (size_t k0 = 0; k0 < k; k0 += tile_size) {
                size_t kmax = std::min(k0 + tile_size, k);
                for (size_t i = i0; i < imax; i++) {
                    for (size_t j = j0; j < jmax; j++) {
                        for (size_t l = k0; l < kmax; l++) {
                            ret(i, j) += m1(i, l) * m2(l, j);
                        }
                    }
                }
            }
        }
    }

    return ret;
    
}

Matrix multiply_mkl(Matrix const &m1, Matrix const &m2){
    Matrix ret(m1.nrow(), m2.ncol());
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1.nrow(),  m2.ncol(), m1.ncol(), 1.0 , m1.data(),
     m1.ncol(), m2.data(), m2.ncol(), 0.0, ret.data(), ret.ncol());
    return ret;
}
