#include <vector>
#include <math.h>
#include <iostream>
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

double calculate_angle(std::vector<int>& v1, std::vector<int>& v2)
{
    double l1 = std::sqrt(std::pow(v1[0], 2) + std::pow(v1[1], 2));
    double l2 = std::sqrt(std::pow(v2[0], 2) + std::pow(v2[1], 2));

    if (l1 == 0)
    {
        std::cout << "Vector 1 length invalid";
        return -4;
    }

    if (l2 == 0)
    {
        std::cout << "Vector 2 length invalid";
        return -4;
    }

    return std::acos((v1[0]*v2[0] + v1[1]*v2[1]) / (l1*l2));
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "angle calculator using pybind";
    m.def("calculate_angle", &calculate_angle, pybind11::arg("v1"), pybind11::arg("v2"));
}