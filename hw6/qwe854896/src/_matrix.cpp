// #include <pybind11/pybind11.h>
// #include <pybind11/operators.h>
// #include <pybind11/numpy.h>
// namespace py = pybind11;

// #include "Matrix.h"
// #include "Multiply.h"
// #include "CustomAllocator.h"

// PYBIND11_MODULE(_matrix, m)
// {
//     m.doc() = "_matrix package";
//     py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
//         .def(py::init<const size_t &, const size_t &>())
//         .def(py::init<const Matrix &>())
//         .def(py::self == py::self)
//         .def(py::self != py::self)
//         .def("__getitem__", [](const Matrix &mat, py::tuple index)
//              { 
//                 size_t it = index[0].cast<size_t>(); 
//                 size_t jt = index[1].cast<size_t>(); 
//                 return mat(it, jt); })
//         .def("__setitem__", [](Matrix &mat, py::tuple index, double value)
//              {
//                 size_t it = index[0].cast<size_t>();
//                 size_t jt = index[1].cast<size_t>();
//                 mat(it, jt) = value; })
//         .def_property_readonly("nrow", [](const Matrix &mat)
//                                { return mat.nrow(); })
//         .def_property_readonly("ncol", [](const Matrix &mat)
//                                { return mat.ncol(); })
//         .def_property(
//             "array", [](Matrix &mat)
//             { return pybind11::array_t<double>(
//                   {mat.nrow(), mat.ncol()},
//                   {sizeof(double) * mat.ncol(), sizeof(double)},
//                   mat.buffer().data()); },
//             nullptr)
//         .def_buffer([](Matrix &m) -> py::buffer_info
//                     { return py::buffer_info(
//                           m.buffer().data(),                       /* Pointer to buffer */
//                           sizeof(double),                          /* Size of one scalar */
//                           py::format_descriptor<double>::format(), /* Python struct-style format descriptor */
//                           2,                                       /* Number of dimensions */
//                           {m.nrow(), m.ncol()},                    /* Buffer dimensions */
//                           {sizeof(double) * m.ncol(),              /* Strides (in bytes) for each index */
//                            sizeof(double)}); });

//     m.def("multiply_naive", &multiply_naive, py::return_value_policy::take_ownership);
//     m.def("multiply_tile", &multiply_tile, py::return_value_policy::take_ownership);
//     m.def("multiply_mkl", &multiply_mkl, py::return_value_policy::take_ownership);

//     m.def("bytes", &CustomAllocator<double>::bytes);
//     m.def("allocated", &CustomAllocator<double>::allocated);
//     m.def("deallocated", &CustomAllocator<double>::deallocated);
// }