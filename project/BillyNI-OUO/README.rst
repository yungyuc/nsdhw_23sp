==============================================================================
MinSteinerTree: A Comparative Analysis of Minimum Steiner Tree and FLUTE Algorithms
==============================================================================

A project that implement a Minimum Steiner Tree Construction and compare to FLUTE Algorithms.

Basic Information
=================

In the field of VLSI, the challenge of global routing has become increasingly important, and finding a good Minimum Steiner Tree algorithm is crucial. Without an efficient algorithm, the time complexity can increase and global routing quality can suffer. By improving the Minimum Steiner Tree algorithm, we can reduce the time complexity and improve the overall performance of the VLSI system. With this in mind, it is essential to develop and compare effective algorithms for constructing Minimum Steiner Trees.

Problem to Solve
================

Global routing is a critical step in the VLSI design process. To simplify the problem, we model the design as a grid graph, taking into account cells, pins, and tracks. The key challenge in this process is finding a minimal Steiner tree that can connect all the pins and nets. This task is essential for achieving an optimal layout and reducing the overall complexity of the VLSI system.

Prospective Users
=================

VLSI design engineers, researchers, and semiconductor manufacturers looking for an efficient and adaptive routing solution for their designs.

System Architecture
===================

This software will be developed using C++ for computational work and Python with Pybind for the interface. The system will consist of the following components(layer).

1. A Python IO interface for users to interact with program.
2. A Python logic layer to handle useing right algorithm at the lower layer.
3. Algorithm implement by C++. 


API Description
===============

* createGraph(x, y): create a routing space with x * y grid.
* addNetlist(netlist): add a netlist containing the nets, chip size, wire cost, and other related information.
* addCbstacle(obstacle): add obstacle into the graph.
* createMST(): create the Minimum Steiner Tree.

Engineering Infrastructure
==========================

* Automatic build system: CMake, GNU Make
* Version control: git
* Testing framework: Google Test, Pytest
* Documentation: README.rst

Schedule
========

* Week 1 (4/10): Study Minimum Steiner Tree and related algorithms(FLUTE).
* Week 2 (4/17): Decide implement the MST in 3D place or in varity weight version.
* Week 3 (4/24): Design data structures and the overall system architecture.
* Week 4 (5/1): Implement the Python 2 layer and the interface between python & c++. 
* Week 5 (5/8): Write tests for algorithm based on MST.
* Week 6 (5/15): Implement MST algorithm.
* Week 7 (5/22): Implement MST algorithm.
* Week 8 (5/29): Refactoring, finalize documentation, and prepare for release

References
==========

1. `MST <https://userweb.cs.txstate.edu/~mb92/papers/pdsec22.pdf>`_
2. `MST Algorithm <https://ieeexplore.ieee.org/document/4358497/>`_