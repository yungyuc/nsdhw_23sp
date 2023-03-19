#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "PythonMatrix.h"
#include "PythonMultiply.h"
namespace py = pybind11;

PYBIND11_MODULE(_matrix, m)
{
      m.doc() = "_matrix package";
      py::class_<PythonMatrix>(m, "Matrix")
          .def(py::init<const size_t &, const size_t &>())
          .def(py::self == py::self)
          .def(py::self != py::self)
          .def("__getitem__", &PythonMatrix::getitem)
          .def("__setitem__", &PythonMatrix::setitem)
          .def_readonly("nrow", &PythonMatrix::nrow)
          .def_readonly("ncol", &PythonMatrix::ncol);

      m.def("multiply_naive",
            &multiply_naive,
            pybind11::return_value_policy::reference);
      m.def("multiply_tile",
            &multiply_tile,
            pybind11::return_value_policy::reference);
      m.def("multiply_mkl",
            &multiply_mkl,
            pybind11::return_value_policy::reference);
}