#include <iostream>
#include <vector>
#include <mkl.h>
//namespace py=pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        m_buffer = new double[nrow*ncol];
        memset(m_buffer, 0, nrow*ncol * sizeof(double));
    }
    // Matrix(const Matrix &m) : m_nrow(m.nrow()), m_ncol(m.ncol())
    // {
    //     m_buffer = new double[m_nrow * m_ncol];
    //     memcpy(m_buffer, m.data(), m_nrow * m_ncol * sizeof(double));
    // }

    ~Matrix()
    {
        //delete[] m_buffer;
    }

    // void setter(size_t row, size_t col, double val)
    // {
        
    //     m_buffer[row*m_ncol + col] = val;
    // }
    // double getter(size_t row, size_t col)
    // {
    //     if(row > m_nrow || col > m_ncol){
    //         throw std::out_of_range("out of matrix index");
    //     }
    //     return m_buffer[row*m_ncol + col];
    // }
    double   operator() (size_t row, size_t col) const
    {
        if(row > m_nrow || col > m_ncol){
            throw std::out_of_range("out of matrix index");
        }
        return m_buffer[row*m_ncol + col];
    }
    double & operator() (size_t row, size_t col)
    {
        if(row > m_nrow || col > m_ncol){
            throw std::out_of_range("out of matrix index");
        }
        return m_buffer[row*m_ncol + col];
    }
    bool operator ==(const Matrix &r) const
    {
        
        if(this->nrow() != r.nrow() || this->ncol() != r.ncol()){
            return 0;
        }else{
            for(size_t i = 0; i < this->nrow(); i ++){
                for(size_t j = 0; j < this->ncol(); j ++){
                    if((*this)(i,j) != r(i,j)){
                        return 0;
                    }
                }
            }
            return 1;
        }
        return 0;
    }
    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    double* data() const {return m_buffer;}

private:

    size_t m_nrow;
    size_t m_ncol;
    double* m_buffer;

};


Matrix multiply_naive(Matrix const &a, Matrix const &b){
     if (a.ncol() != b.nrow())
    {
        throw std::out_of_range("matrix column differs from row size");
    }

    Matrix ret(a.nrow(), b.ncol());

    for (size_t i = 0; i < a.nrow(); i++) {
        for (size_t j = 0; j < b.ncol(); j++) {
            for (size_t k = 0; k < a.ncol(); k++) {
                ret(i, j) += a(i, k) * b(k, j);
            }
        }
    }
    return ret;
}

Matrix multiply_tile(Matrix const &a, Matrix const &b, int T){
    if (a.ncol() != b.nrow())
    {
        throw std::out_of_range("matrix column differs from row size");
    }

    Matrix ret = Matrix(a.nrow(), b.ncol());
    int M = a.nrow();
    int N = a.ncol();
    int K = b.ncol();
    for (int m = 0; m < M; m += T) {
        for (int n = 0; n < N; n += T) {
            for (int k = 0; k < K; k += T) {

                const int minMt = std::min(m + T, M);
                const int minNt = std::min(n + T, N);
                const int minKt = std::min(k + T, K);

                for (int mt = m; mt < minMt; mt++) {
                    for (int nt = n; nt < minNt; nt++) {
                        for (int kt = k; kt < minKt; kt++) {
                            ret(mt , nt) += a(mt , kt) * b(kt , nt);
                        }
                    }
                }
            }
        }
    }


    return ret;
}

Matrix multiply_mkl(Matrix const &mat1, Matrix const &mat2)
{
    if (mat1.ncol() != mat2.nrow()) {
        exit(1);
    }

    mkl_set_num_threads(1);

    Matrix ret(mat1.nrow(), mat2.ncol());

    cblas_dgemm(CblasRowMajor /* const CBLAS_LAYOUT Layout */
                ,
                CblasNoTrans /* const CBLAS_TRANSPOSE transa */
                ,
                CblasNoTrans /* const CBLAS_TRANSPOSE transb */
                ,
                mat1.nrow() /* const MKL_INT m */
                ,
                mat2.ncol() /* const MKL_INT n */
                ,
                mat1.ncol() /* const MKL_INT k */
                ,
                1.0 /* const double alpha */
                ,
                mat1.data() /* const double *a */
                ,
                mat1.ncol() /* const MKL_INT lda */
                ,
                mat2.data() /* const double *b */
                ,
                mat2.ncol() /* const MKL_INT ldb */
                ,
                0.0 /* const double beta */
                ,
                ret.data() /* double * c */
                ,
                ret.ncol() /* const MKL_INT ldc */
    );

    return ret;
}



// PYBIND11_MODULE(_matrix, m) {
//     // py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
//     // .def(py::init<int, int>())
//     // .def_buffer([](Matrix &m) -> py::buffer_info {
//     //     return py::buffer_info(
//     //         m.data(),                               /* Pointer to buffer */
//     //         sizeof(float),                          /* Size of one scalar */
//     //         py::format_descriptor<float>::format(), /* Python struct-style format descriptor */
//     //         2,                                      /* Number of dimensions */
//     //         { m.nrow(), m.ncol() },                 /* Buffer dimensions */
//     //         { sizeof(float) * m.ncol(),             /* Strides (in bytes) for each index */
//     //           sizeof(float) }
//     //     );
//     // });
//     py::class_<Matrix>(m, "Matrix")
//     .def(py::init<int, int>())
//     .def("__getitem__", [](Matrix &self, pybind11::args args)
//             {    
//                 //std::cout << (*args[0]) << std::endl;
//                 py::tuple t = args[0];
//                 // for (size_t it=0; it<t.size(); ++it) {
//                 //  py::print(py::str("{}").format(t[it].cast<int>()));
//                 // }
//                 return self(t[0].cast<int>(),t[1].cast<int>()); 
//             })
//     .def("__setitem__",[](Matrix &self, pybind11::args args)
//             {    
//                 // std::cout << (*args[0]) << " "<< *args[1] << std::endl;
//                 py::tuple t = args[0];  
//                 self(t[0].cast<int>(),t[1].cast<int>()) = args[1].cast<int>(); 
//             })
//     .def_property_readonly("nrow", &Matrix::nrow)
//     .def_property_readonly("ncol", &Matrix::ncol)
//     .def("__eq__", &Matrix::operator ==);

//     m.def("multiply_naive", &multiply_naive, "basic Matrix-Matrix Multiplication");
//     m.def("multiply_tile", &multiply_tile, "tile Matrix-Matrix Multiplication");
//     m.def("multiply_mkl", &multiply_mkl, "mkl Matrix-Matrix Multiplication");
// }


// // def __setitem__(self, key, value):
// //         setattr(self, key, value)
// //[](Vec &self, unsigned index, float val)
//          //    { *self[index] = val; })

// //     def __getitem__(self, key):
// //         return getattr(self, key)
// //static_cast<void (Pet::*)(int)>(&Pet::set)