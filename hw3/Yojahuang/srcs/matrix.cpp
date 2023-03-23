#include "matrix.h"
#include <vector>
#include <iostream>
#include <cstddef>
#include <cstring>

Matrix::Matrix() {
    this->m_row = 0;
    this->m_col = 0;
    this->m_buffer = NULL;
}

Matrix::~Matrix() {
    this->m_row = 0;
    this->m_col = 0;
}

Matrix::Matrix(size_t row, size_t col, std::vector<double> const value) {
    this->m_row = row;
    this->m_col = col;

    int cnt = 0;

    this->m_buffer = (double*)malloc(sizeof(double) * value.size());
    memset(this->m_buffer, 0, sizeof(double) * value.size());

    for (const double& v : value) {
        this->m_buffer[cnt++] = v;
    }
}

Matrix::Matrix(size_t row, size_t col) {
    this->m_row = row;
    this->m_col = col;

    size_t size = row * col;

    this->m_buffer = (double*)malloc(sizeof(double) * size);
    memset(this->m_buffer, 0, sizeof(double) * size);
}

Matrix Matrix::transpose() const {
    Matrix result(this->m_col, this->m_row);

    for (size_t i = 0; i < this->m_row; ++i) {
        for (size_t j = 0; j < this->m_col; ++j) {
            size_t idx = this->m_col * i + j;
            result(j, i) = this->m_buffer[idx];
        }
    }

    return result;
}

bool Matrix::operator==(const Matrix rhs) const {
    if (this->n_col() != rhs.n_col()) return false;
    if (this->n_row() != rhs.n_row()) return false;

    int col = this->m_col, row = this->m_row;

    for (int i = 0; i < col; ++i) {
        for (int j = 0; j < row; ++j) {
            size_t idx = this->m_col * i + j;
            if (this->m_buffer[idx] != rhs(i, j)) return false;
        }
    }

    return true;
}

bool Matrix::operator!=(const Matrix rhs) const {
    return !(*this == rhs);
}


double& Matrix::operator()(size_t r, size_t c) const {
    size_t idx = r * this->m_col + c;
    return this->m_buffer[idx];
}

const size_t& Matrix::n_row() const {
    return this->m_row;
}

const size_t& Matrix::n_col() const {
    return this->m_col;
}