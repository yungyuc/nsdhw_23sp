==============================================================================
AQuaDRR: Adaptive Quadtree Detailed Routing with Rip-up and Reroute for VLSI
==============================================================================

A project that utilizes Quadtree data structure for efficient adaptive detailed routing with rip-up and reroute functionality in VLSI.

Basic Information
=================

The Adaptive Quadtree Detailed Routing with Rip-up and Reroute project aims to provide an efficient solution to the detailed routing problem in VLSI. By employing the Quadtree data structure, the algorithm can adaptively choose the appropriate window size for rerouting, effectively reducing the time complexity and improving the routing quality.

Problem to Solve
================

Detailed routing is a crucial step in the VLSI design process. Handling overlapping nets in the routing process can be computationally expensive and time-consuming, especially for complex designs. This project aims to tackle this problem by utilizing the Quadtree data structure to enable efficient routing with adaptive window size selection for rip-up and reroute.

Prospective Users
=================

VLSI design engineers, researchers, and semiconductor manufacturers looking for an efficient and adaptive routing solution for their designs.

System Architecture
===================

This software will be developed using C++ for computational work and Python with Pybind for the interface. The system will consist of the following components:

1. A Quadtree data structure implementation in C++ for representing the routing space and managing the routing windows.
2. An initial detailed routing algorithm in C++ that allows overlapping nets with a high cost.
3. An adaptive rip-up and reroute algorithm in C++ based on Quadtree data structure for efficient overlapping nets handling.
4. A Python interface with Pybind for users to interact with the C++ routing solution.


API Description
===============

* createRoutingSpace(x, y, z): create a routing space with x * y grid, and allow the software to use at most z layers.
* addNetlist(netlist): add a netlist containing the nets, chip size, wire cost, and other related information.
* route(): perform the detailed routing process.
* getRoutingResult(): return the routing result.

Engineering Infrastructure
==========================

* Automatic build system: CMake, GNU Make
* Version control: git
* Testing framework: Google Test, Pytest
* Documentation: README.rst

Schedule
========

* Week 1 (4/10): Study Quadtree data structure and related algorithms
* Week 2 (4/17): Design data structures and the overall system architecture
* Week 3 (4/24): Write tests for Quadtree data structure and initial detailed routing algorithm
* Week 4 (5/1): Implement Quadtree data structure and initial detailed routing algorithm
* Week 5 (5/8): Write tests for adaptive rip-up and reroute algorithm based on Quadtree
* Week 6 (5/15): Implement adaptive rip-up and reroute algorithm based on Quadtree
* Week 7 (5/22): Develop user interface, API, and integrate all components
* Week 8 (5/29): Refactoring, finalize documentation, and prepare for release

References
==========

1. `Quadtree <https://en.wikipedia.org/wiki/Quadtree>`_
2. `Routing Algorithm <https://ieeexplore.ieee.org/document/6504553>`_
