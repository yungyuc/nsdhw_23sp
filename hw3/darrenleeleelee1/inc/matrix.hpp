#pragma once
#include <cstddef>
#include <algorithm>
class Matrix {

public:
    Matrix() = default;
    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement];
    }

    ~Matrix()
    {
        delete[] m_buffer;
    }

    Matrix(const Matrix &matrix){
            m_nrow = matrix.m_nrow;
            m_ncol = matrix.m_ncol;
            m_buffer = new double[m_nrow * m_ncol](); 
            memcpy(m_buffer, matrix.m_buffer, m_nrow * m_ncol*sizeof(double));
    }

    Matrix& operator= (const Matrix& matrix) {
        m_buffer = matrix.m_buffer;
        return *this;
    }

    bool operator==(const Matrix &other) const
    {
        if (nrow() != other.nrow() || ncol() != other.ncol()) {
            return false;
        }

        for (size_t i = 0; i < m_nrow; i++) {
            for (size_t j = 0; j < m_ncol; j++) {
                if ((*this)(i, j) != other(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix &other) const
    {
        return !(*this == other);
    }
    
    // No bound check.
    double   operator() (size_t row, size_t col) const{
        return m_buffer[row*m_ncol + col];
    }
    
    double & operator() (size_t row, size_t col){
        return m_buffer[row*m_ncol + col];
    }

    void zero(){
        for(size_t r = 0; r < m_nrow; r++){
            for(size_t c = 0; c < m_ncol; c++){
                m_buffer[r*m_ncol + c] = 0.0;
            }
        }
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    double *buffer() const { return m_buffer; }
private:

    size_t m_nrow;
    size_t m_ncol;
    double * m_buffer;

};