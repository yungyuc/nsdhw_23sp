#ifndef _MATRIX_H
#define _MATRIX_H
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <mkl.h>


static size_t BLK_NUM = 32;


class Matrix {
    public:
        size_t m_row;
        size_t m_col;
        double *buf;
        size_t n_elem;
        Matrix() = default;
        Matrix(size_t n_row, size_t n_col) {
            if(n_row == 0 || n_col == 0) {
                throw std::invalid_argument("number of rows or columns should not be zero");
            }
            this->m_row = n_row;
            this->m_col = n_col;
            this->n_elem = n_row * n_col;
            buf = new double[this->n_elem];
            std::fill(buf, buf + n_elem, (double)0);
        }
        double operator() (size_t row, size_t col) const {
            return buf[row * m_col + col];
        }
        double & operator()(size_t row, size_t col) {
            return buf[row * m_col + col];
        }

        bool operator==(const Matrix &ano) const{
            if(ano.m_row != this->m_row || ano.m_col != this->m_col) {
                return false;
            }
            for(size_t i = 0; i < ano.m_row; i ++) {
                for(size_t j = 0; j < ano.m_col; j ++) {
                    if(ano(i, j) != (*this)(i, j)) {
                        return false;
                    }
                }
            }
            return true;
        }

        Matrix & operator=(const Matrix &ano) {
            if(this->m_row != ano.m_row && this->m_col != ano.m_col) {
                throw std::invalid_argument("Cannot assign matrix to different shape");
            }
            size_t n_elem = this->m_row * this->m_col;
            for(size_t i = 0; i < n_elem; i ++) {
                this->buf[i] = ano[i];
            }
            return *this;
        }

        double operator[] (size_t offset) const {
            return buf[offset];
        }
        double & operator[](size_t offset) {
            return buf[offset];
        }
        size_t nrow() const { return m_row; }
        size_t ncol() const { return m_col; }
};

Matrix multiply_naive(const Matrix &A, const Matrix &B) {
    if(A.m_col != B.m_row) {
        throw std::invalid_argument("A's cols should be the same as B's rows");
    }
    Matrix ret = Matrix(A.m_row, B.m_col);
    for(size_t i = 0; i < ret.m_row; i ++) {
        for(size_t j = 0; j < ret.m_col; j ++) {
            double v = 0;
            for(size_t k = 0; k < A.m_col; k ++) {
                v += A(i, k) * B(k, j);
            }
            (ret)(i, j) = v;
        }
    }
    return ret;
}

static void load_buf(double retbuf[], const Matrix &A, double Abuf[], size_t it1, size_t jt1, const Matrix &B, double Bbuf[], size_t it2, size_t jt2) {
    const size_t ncol1 = A.m_col;
    for(size_t i = 0; i < BLK_NUM; i ++) {
        size_t base_t = i * BLK_NUM;
        size_t base_i = (it1 * BLK_NUM + i);
        size_t base_j = jt1 * BLK_NUM;
        if(base_i >= A.m_row) {
            break;
        }
        base_i *= ncol1;
        for(size_t j = 0; j < BLK_NUM; j ++) {
            if(base_j + j < A.m_col)
                Abuf[base_t + j] = A[base_i + base_j + j];
            else
                Abuf[base_t + j] = (double)0;
        }
    }
    const size_t ncol2 = B.m_col;
    for(size_t i = 0; i < BLK_NUM; i ++) {
        const size_t base_t = i * BLK_NUM;
        size_t base_i = (it2 * BLK_NUM + i);
        size_t base_j = (jt2) * BLK_NUM;
        if(base_i >= B.m_row) {
            break;
        }
        base_i *= ncol2;
        for(size_t j = 0; j < BLK_NUM; j ++) {
            if(base_j + j < B.m_col)
                retbuf[base_t + j] = B[base_i + base_j + j];
            else
                retbuf[base_t + j] = (double)0;
        }
    }

    for(size_t i = 0; i < BLK_NUM; i ++) {
        const size_t base = i * BLK_NUM;
        for(size_t j = 0; j < BLK_NUM; j ++) {
            Bbuf[j * BLK_NUM + i] = retbuf[base + j];
        }
    }
}

static void tile_mul(double retbuf[], double Abuf[], double Bbuf[]) {

    for(size_t i = 0; i < BLK_NUM; i ++) {
        const size_t base1 = i * BLK_NUM;
        for(size_t j = 0; j < BLK_NUM; j ++) {
            const size_t base2 = j * BLK_NUM;
            double v = 0;
            for(size_t k = 0; k < BLK_NUM; k ++) {
                v += Abuf[base1 + k] * Bbuf[base2 + k];
            }
            retbuf[base1 + j] = v;
        }
    }
}

static void save(Matrix &ret, double retbuf[], size_t it, size_t jt) {
    const size_t ncol = ret.m_col;
    for(size_t i = 0; i < BLK_NUM; i ++) {
        const size_t base_s = i * BLK_NUM;
        const size_t base_t = (it * BLK_NUM + i) * ncol + jt * BLK_NUM;
        for(size_t j = 0; j < BLK_NUM; j ++) {
            if(base_t + j < ret.n_elem)
                ret[base_t + j] += retbuf[base_s + j];
        }
    }
}

Matrix multiply_tile(const Matrix &A, const Matrix &B, size_t tile_size) {
    if(A.m_col != B.m_row) {
        throw std::invalid_argument("A's cols should be the same as B's rows");
    }
    if(tile_size == 0) {
        throw std::invalid_argument("tile_size should greater than 0");
    }
    BLK_NUM = tile_size;

    Matrix ret = Matrix(A.m_row, B.m_col);
    double Abuf[BLK_NUM * BLK_NUM];
    std::fill(Abuf, Abuf + BLK_NUM * BLK_NUM, (double)0);
    double Bbuf[BLK_NUM * BLK_NUM];
    std::fill(Bbuf, Bbuf + BLK_NUM * BLK_NUM, (double)0);
    double retbuf[BLK_NUM * BLK_NUM];
    std::fill(retbuf, retbuf + BLK_NUM * BLK_NUM, (double)0);
    size_t row1 = A.m_row / BLK_NUM + (A.m_row % BLK_NUM > 0);
    size_t col1 = A.m_col / BLK_NUM + (A.m_row % BLK_NUM > 0);
    size_t row2 = B.m_row / BLK_NUM + (B.m_row % BLK_NUM > 0);
    size_t col2 = B.m_col / BLK_NUM + (B.m_row % BLK_NUM > 0);
    for(size_t i = 0; i < row1; i ++) {
        for(size_t j = 0; j < col2; j ++) {
            for(size_t k = 0; k < col1; k ++) {
                load_buf(retbuf, A, Abuf, i, k, B, Bbuf, k, j);
                tile_mul(retbuf, Abuf, Bbuf);
                save(ret, retbuf, i, j);
            }
        }
    }
    return ret;
}

Matrix multiply_mkl(Matrix &A, Matrix &B) {
    Matrix ret(A.m_row, B.m_col);

    cblas_dgemm(
        CblasRowMajor /* const CBLAS_LAYOUT Layout */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transa */
      , CblasNoTrans /* const CBLAS_TRANSPOSE transb */
      , A.m_row /* const MKL_INT m */
      , B.m_col /* const MKL_INT n */
      , A.m_col /* const MKL_INT k */
      , 1.0 /* const double alpha */
      , A.buf /* const double *a */
      , A.m_col /* const MKL_INT lda */
      , B.buf /* const double *b */
      , B.m_col /* const MKL_INT ldb */
      , 0.0 /* const double beta */
      , ret.buf /* double * c */
      , ret.m_col /* const MKL_INT ldc */
    );

    return ret;
}

#endif