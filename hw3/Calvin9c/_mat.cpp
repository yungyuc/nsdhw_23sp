#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <mkl.h>
#include <pybind11/pybind11.h>

using namespace std;

namespace py = pybind11;

class Matrix{
    public:

        Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol){
            reset_buffer(nrow, ncol);
        }

        Matrix(Matrix const & other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol){
            reset_buffer(other.m_nrow, other.m_ncol);
            for (size_t i=0; i<m_nrow; ++i){
                for (size_t j=0; j<m_ncol; ++j){
                    (*this)(i,j) = other(i,j);
                }
            }
        }

        Matrix & operator=(Matrix const & other){
            if (this == &other) { return *this; }
            if (m_nrow != other.m_nrow || m_ncol != other.m_ncol){
                reset_buffer(other.m_nrow, other.m_ncol);
            }
            for (size_t i=0; i<m_nrow; ++i){
                for (size_t j=0; j<m_ncol; ++j){
                    (*this)(i,j) = other(i,j);
                }
            }
            return *this;
        }

        bool operator==(Matrix const & other)const{

            if( m_ncol != other.ncol() or m_nrow != other.nrow() ){
                cout<<"the order of two matrix are not match";
                return false;
            }

            for(size_t i=0;i<m_nrow;++i){
                for(size_t j=0;j<m_ncol;++j){
                    if(m_buffer[index(i, j)]!=other(i, j)){
                        return false;
                    }
                }
            }
            return true;

        }


        // TODO: move constructors and assignment operators.

        ~Matrix(){
            reset_buffer(0, 0);
        }

        double operator() (size_t row, size_t col) const {
            return m_buffer[index(row, col)];
        }
        double & operator() (size_t row, size_t col){
            return m_buffer[index(row, col)];
        }

        size_t nrow() const { return m_nrow; }
        size_t ncol() const { return m_ncol; }
        size_t size() const { return m_nrow * m_ncol; }
        
        double buffer(size_t i) const { return m_buffer[i]; }
        std::vector<double> buffer_vector() const {
            return std::vector<double>(m_buffer, m_buffer+size());
        }

        bool is_transposed() const { return m_transpose; }

        Matrix & transpose(){
            m_transpose = !m_transpose;
            std::swap(m_nrow, m_ncol);
            return *this;
        }

        double * get_buffer() const { return m_buffer; }
    
        void initialize(){
            for(size_t i=0 ; i<m_nrow ; ++i){
                for(size_t j=0 ; j<m_nrow ; ++j){
                    (*this)(i,j) = 0.0;
                }
            }
        }

    private:
        size_t index(size_t row, size_t col) const{
            if ( m_transpose ) { return row + col * m_nrow; }
            else{
                return row * m_ncol + col;
            }
        }

        void reset_buffer(size_t nrow, size_t ncol){
            
            if (m_buffer){ delete[] m_buffer; }
            
            const size_t nelement = nrow * ncol;
            
            if (nelement) { m_buffer = new double[nelement]; }
            else{
                m_buffer = nullptr;
            }
            m_nrow = nrow;
            m_ncol = ncol;
        }

        size_t m_nrow = 0;
        size_t m_ncol = 0;
        bool m_transpose = false;
        double * m_buffer = nullptr;

};





Matrix multiply_naive(const Matrix &x, const Matrix &y){

    if(x.ncol()!=y.nrow()){
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row"
        );
    }

    Matrix ret(x.nrow(), y.ncol());

    for(size_t i=0; i<ret.nrow(); ++i){
        for(size_t k=0; k<ret.ncol(); ++k){

            double v = 0;
            for(size_t j=0; j<x.ncol(); ++j){
                v += x(i,j) * y(j,k);
            }
            ret(i,k) = v;
        }
    }
    return ret;
}

Matrix multiply_tile(const Matrix &x, const Matrix &y, size_t tile_size){

    if(x.ncol()!=y.nrow()){
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row"
        );
    }

    Matrix ret(x.nrow(), y.ncol());
    ret.initialize();

    for( size_t i=0 ; i<x.ncol() ; i+=tile_size ){
        for( size_t j=0 ; j<y.nrow() ; j+=tile_size ){
            for( size_t k=0 ; k<x.ncol() ; k+=tile_size ){

                size_t upper_bound_sub_i = min(i+tile_size, x.nrow());
                for( size_t sub_i=i ; sub_i<upper_bound_sub_i ; ++sub_i ){
                    
                    size_t upper_bound_sub_j = min(j+tile_size, y.ncol());
                    for( size_t sub_j=j ; sub_j<upper_bound_sub_j ; ++sub_j ){

                        size_t upper_bound_sub_k = min(k+tile_size, x.ncol());
                        double v = 0;
                        for( size_t sub_k=k ; sub_k<upper_bound_sub_k ; ++sub_k ){
                            v += x(sub_i, sub_k) * y(sub_k,sub_j);
                        }
                        ret(sub_i, sub_j) += v ;

                    }

                }

            }
        }
    }
    return ret;
}

Matrix multiply_mkl(const Matrix &x, const Matrix &y){

    if(x.ncol()!=y.nrow()){
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row"
        );
    }

    Matrix ret(x.nrow(), y.ncol());

    const int alpha = 1;
    const int beta = 0;

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, x.nrow(), y.ncol(), x.ncol(), alpha, x.get_buffer(), x.ncol(), y.get_buffer(), y.ncol(), beta, ret.get_buffer(), y.ncol());

    return ret;
}


// std::ostream & operator << (std::ostream & ostr, Matrix const & mat){
//     for (size_t i=0; i<mat.nrow(); ++i){
//         ostr << std::endl << " ";
//         for (size_t j=0; j<mat.ncol(); ++j){
//             ostr << " " << std::setw(2) << mat(i, j);
//         }
//     }
//     return ostr;
// }


PYBIND11_MODULE(_mat, m){
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<const Matrix &>())
        .def_property_readonly("nrow", [](const Matrix &mat) { return mat.nrow(); })
        .def_property_readonly("ncol", [](const Matrix &mat) { return mat.ncol(); })
        .def("__eq__", [](const Matrix &x, const Matrix &y) { return x == y; })
        .def("__setitem__", [](Matrix &self, std::pair<int, int> idx, double val) {
            self(idx.first, idx.second) = val;
        })
        .def("__getitem__", [](const Matrix &self, std::pair<int, int> idx) {
            return self(idx.first, idx.second);
        })
        // .def("assign", &Matrix::operator=)
        .def("initialize", &Matrix::initialize); 

    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);
}