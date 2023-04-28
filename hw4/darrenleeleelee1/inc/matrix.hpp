#pragma once
#include <cstddef>
#include <algorithm>
#include <vector>
#include <cstring>
#include <memory>
#include "allocator.hpp"
class Matrix {

public:
    Matrix() = default;
    
    Matrix(size_t nrow, size_t ncol)
        : m_nrow(nrow), m_ncol(ncol), m_data(std::vector<double, CustomAllocator<double>>(nrow * ncol)) {
    }
    
    // Matrix &operator=(const Matrix &matrix) {
    //     m_nrow = matrix.m_nrow;
    //     m_ncol = matrix.m_ncol;
    //     m_data = matrix.m_data;
    //     return *this;
    // }

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
        return m_data[row*m_ncol + col];
    }
    
    double & operator() (size_t row, size_t col){
        return m_data[row*m_ncol + col];
    }

    void zero(){
        for(size_t r = 0; r < m_nrow; r++){
            for(size_t c = 0; c < m_ncol; c++){
                m_data[r*m_ncol + c] = 0.0;
            }
        }
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    double *buffer() const { return const_cast<double *>(m_data.data()); }
private:

    size_t m_nrow;
    size_t m_ncol;
    std::vector<double, CustomAllocator<double>> m_data;

};