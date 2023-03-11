#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

template<typename T>
T dot(const std::vector<T> &A, const std::vector<T> &B) {
    T ret = 0;
    assert(A.size() == B.size());
    for(size_t i = 0; i < A.size(); ++i) {
        ret += A[i] * B[i];
    }
    return ret;
}

template<typename T>
T L2_norm(const std::vector<T> &x) {
    T ret = 0;
    for(size_t i = 0; i < x.size(); ++i) {
        ret += x[i] * x[i];
    }
    return sqrt(ret);
} 

double angle(const std::vector<double> &A, const std::vector<double> &B) {
    if(A.size() != B.size()) {
        std::cout << "Please input vectors of same dims." << '\n';
        return std::nan("");
    }
    double len_A = L2_norm(A);
    double len_B = L2_norm(B);
    if(len_A == 0.0 || len_B == 0.0) {
        std::cout << "Invalid input with length 0.\n";
        return std::nan("");
    }     
    return std::acos(dot(A, B) / len_A / len_B);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "calculate angle between two vector"; 
    m.def("angle", &angle, "angle between A and B");
}

