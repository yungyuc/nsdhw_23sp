# ggdGVC: A Simple Vertex Coloring Library

## Basic Information

Github repository url: https://github.com/SmallHanley/ggdGVC

Graph coloring is a popular research topics in graph theory. Vertex coloring is one of the coloring problems that involves assigning colors to the vertices of a graph such that no two adjacent vertices have the same color.

## Problem to Solve

Graph coloring is an essential problem in computer science. The problem has many practical applications, including scheduling, register allocation, and frequency allocation in wireless communication.

Some applications may pre-color specific vertices, making the usage scenario more complex.

Therefore, ggdGVC aims to develop a graph coloring library that can solve graph coloring problems efficiently and provide an easy-to-use API.

## Prospective Users

The prospective users of ggdGVC include computer science students and researchers. The library can be used in various applications, including scheduling, register allocation, and frequency allocation in wireless communication.

## System Architecture

ggdGVC will be developed in C++, and create Python bindings of the C++ code.

The system architecture of the library will consist of the following components:

* Graph data structures
* Vertex coloring algorithm
* API

## API Description

* `create_graph(size)`: Create or resize a graph
* `add_vertex(id)`: Add vertex
* `add_edge(id1, id2)`: Connect two vertex with an edge
* `set_vertex_color(id, color)`: Coloring the specific vertex
* `color_graph()`: Coloring the graph
* `read_graph(file): Read graph from file

## Engineering Infrastructure

1. Automatic build system: GNU make
2. Version control: git
3. Testing framework: pytest
4. Documentation: README.md

## Schedule

* Week 1 (4/10): Study domain knowledge and some algorithms
* Week 2 (4/17): Design data structures and functions
* Week 3 (4/24): Implement greedy/backtracking algorithm
* Week 4 (5/1): Impelment other alorithm
* Week 5 (5/8): Implement the API functions and write tests
* Week 6 (5/15): Optimize
* Week 7 (5/22): Visualize
* Week 8 (5/29): Write documentation

## References

* https://www.boost.org/doc/libs/1_81_0/libs/graph/doc/sequential_vertex_coloring.html
* http://www.dharwadker.org/vertex_coloring/
