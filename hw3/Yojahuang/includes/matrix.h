#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstddef>

class Matrix
{
public:
    Matrix();
    ~Matrix();

    Matrix(size_t, size_t, std::vector<double> const);
    Matrix(size_t, size_t);

    double& operator()(size_t, size_t) const;
    bool operator==(const Matrix) const;
    bool operator!=(const Matrix) const;

    Matrix transpose() const;

    const size_t& n_row() const;
    const size_t& n_col() const;

    friend Matrix multiply_mkl(Matrix const&, Matrix const&);
private:
    // Member functions
    // Member data.
    size_t m_col, m_row;
    double* m_buffer;
}; /* end class Line */

#endif