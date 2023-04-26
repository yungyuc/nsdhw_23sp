# Graph Partitioning for Solving PDEs
A library that can perform graph partitioning for solving PDE purpose. 

## Basic Information
For solving PDEs numerically, there are some famous methods to achieve the goal. FEM(Finite element method) is one of them.

Graph partitioning can be used in FEM to divide the domain into subdomain or elements. Which can then be sloved independently or in parallel. So a good design of graph partition algorithm can greatly the performance for solving PDE numerically in some cases.


## Problem to Solve
The definition of graph partition is the reduction of a graph to smaller graph by partitioning its set of nodes into mutally exclusive groups. With edges accros between the groups.

The problem has been known to be NP-complete, however, there still be some heuristic algorithms for finding good solutions for graph partition, include spectral partitioning, recursive partitioning, multi-level partitioning, and evolutionary algorithms.

This project will try to implement one or more algorithm for graph partitioning.

## System Architecture
- Implement the library in C++.
- An interface for C++.
- An interface for python by using pybind11.

## API Description
There will be APIs as following description.
- Graph(information about graph ....)
    - returning a graph that is generate according to the information
- graph_partition(graph, ret, ....)
    - the api that will partition graph and save the returning graph to the ret.
    - Can specify other information like the algorithm for partition graph or the condition used by the algorithm......

## Engineering Infrastructure
- Use ``CMake`` to build the code
- Use ``pytest`` for testing
- Use ``git`` for version control

## Schedule
### Week 1(4/10)
- Try to grap the basic idea of graph paritioning for solving PDEs.
- Plan the project.

### Week 2(4/17)
- Get more understand about graph partitioning for solving PDEs.
- Try to learn some algorithm for performing graph partitioning.

### Week 3(4/24)
- Try to define a way of testing the correctness and performance.
- Design a small testing, and get more idea of the appearance of the library.

### Week 4(5/1)
- Design the graph structure
- Test the graph structure with previous small testing

### Week 5(5/8)
- Implement the algorithms.
- Test with previous small testing

### Week 6(5/15)
- Design more complicated testing

### Week 7(5/22)
- Reserved time for debugging

### Week 8(5/29)
- Reserved time for debugging
- Write document and prepare presentation.

## Reference
- [Partial differential equation Wiki](https://en.wikipedia.org/wiki/Partial_differential_equation)
- [Finite element method Wiki](https://en.wikipedia.org/wiki/Finite_element_method)
- [Graph Partition Wiki](https://en.wikipedia.org/wiki/Graph_partition)
