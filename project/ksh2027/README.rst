========================================
BOP: Boolean Operations on Polygons
========================================


Basic Information
=================

BOP aims to develop an efficient and user-friendly library for performing Boolean Operations on Polygons 
in 2D Euclidean space. The project will provide a set of algorithms for performing operations such as 
AND, OR, NOT, XOR, and more on polygons, which will be useful in extracting geometrical properties of graphics.

Github repository: https://github.com/ksh2027/BOP


Problem to Solve
================

Boolean Operations on Polygons (BOP) is a critical problem in computer graphics and geometry processing. 
The goal is to compute the intersection, union, difference, or exclusive-or (XOR) of two or more polygons. 

The physics and mathematics behind BOP involve analyzing the topological relationships between the polygons, 
such as whether they intersect or contain each other. Efficient algorithms for BOP require handling 
complex geometric data structures, such as the arrangement of line segments and vertices.

I choose to implement an efficient and simple algorithm for computing BOP, which is suitable for real-time 
applications and can handle large datasets.

Overall, efficient implementation of BOP is an important problem in computer graphics and geometry processing.
Developing efficient algorithms and numerical methods for BOP is an important research direction in this field.


Prospective Users
=================

The users of this software are primarily researchers, engineers, and practitioners in various fields, 
such as computer-aided design (CAD), graphical information system (GIS), architectural modeling, 
and digital fabrication.

In addition, the software can also be useful for educational purposes, as it provides a practical 
demonstration of geometric algorithms and data structures.


System Architecture
===================

C++: kernel system
  1. a class of polygons data structure
  2. several boolean operations on polygons
  3. a class of BOP algorithm (kernel)

Python: user interface
  1. an user-friendly UI
  2. wrap the kernel system by using pybind11
  

API Description
===============

Users can directly import the python module of BOP.
The python script might look like this::

  import bop

  # create the input polygons
  poly1 = ... # initialize poly1 with vertices and edges
  poly2 = ... # initialize poly2 with vertices and edges

  # create the Boolean operation
  op = bop.BOPBoolean.UNION

  # perform the operation
  result = bop.BOPGeometry.performBoolean(poly1, poly2, op)



Engineering Infrastructure
==========================

``Build:`` make

``Version control:`` git

``Testing:`` unittest/pytest

``Documentation:`` github repository


Schedule
========

**Week 1 (4/10):**
  Learn the algorithm for boolean operations on polygons
  
**Week 2 (4/17):**
  Design data structures and algorithms
  
**Week 3 (4/24):**
  Develop kernel system in C++
  
**Week 4 (5/1):**
  Debugging and refinement
  
**Week 5 (5/8):**
  Develop UI in python 
  
**Week 6 (5/15):**
  Design testing and CI
  
**Week 7 (5/22):**
  Final checking and modification
  
**Week 8 (5/29):**
  Prepare for presentation

References
==========

A simple algorithm for Boolean operations on polygons: https://www.sciencedirect.com/science/article/pii/S0965997813000379
