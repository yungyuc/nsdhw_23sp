===========================================================
MatrixCurer  for  Solving  Ill-Conditioned  Linear  Systems
===========================================================

MatrixCurer: a C++ library for solving ill-conditioned linear systems that
provide seamless interface with numpy ndarray.

Basic Information
=================

There are many libraries which can solve linear systems. But they may suffer
from approximation error when solving ill-conditioned linear systems.

MatrixCurer is a C++ library that provides an efficient and scalable
solution for solving ill-conditioned linear systems.

There are many existing techniques related to solving ill-conditioned
linear systems:

1. Tikhonov regularization
2. Truncated SVD

Problem to Solve
================

Ill-conditioned linear systems often arise in a variety of scientific
and engineering applications, and solving them can be challenging due
to numerical instability.

Existing methods can be computationally
expensive or require significant manual intervention, making them
impractical for many real-world problems.

MatrixCurer aims to provide
an efficient and easy-to-use solution for solving ill-conditioned linear
systems, enabling researchers and practitioners to focus on their
domain-specific tasks without worrying about numerical issues.

Prospective Users
=================

MatrixCurer is intended for researchers and practitioners in a wide range
of fields, including physics, engineering, computer science, and applied
mathematics. It is designed to be easy to use and integrate with existing
codebases, making it accessible to both novice and experienced users.

System Architecture
===================

MatrixCurer will be developed in C++ and does not depend on Python.
Python builtin list or numpy ndarray will be converted to the template
class in C++ for further computation.

The library is designed to be efficient and scalable, utilizing
state-of-the-art numerical algorithms for solving ill-conditioned linear
systems.
The system's workflow involves pre-processing of the input matrix,
followed by iterative refinement of the solution vector until a
convergence criterion is met.

API Description
===============

MatrixCurer can be programmed in both C++ and Python. In C++, users can call
the library functions by including the header files and linking with the
library. In Python, users can use the numpy ndarray interface to pass the
input matrix to the C++ library and retrieve the output solution vector.
The library provides a range of configuration options, including tolerance
levels and convergence criteria, to enable users to fine-tune the solution
process.

Engineering Infrastructure
==========================

MatrixCurer uses CMake for automatic build system, enabling users to easily
build the library on their respective platforms. The library is hosted on
GitHub and uses Git for version control, enabling users to easily collaborate
on the codebase. MatrixCurer uses pytest for unit testing and reStructuredText
for documentation. Continuous integration is utilized for testing the library
across multiple platforms.

Github Action is configured for running the tests regularly and for
each check-in and pull request.

All new features developed will have corresponding unit tests.

Correctness will be verified by using numpy: computation carried out in C++
should produce exactly identical results to that in numpy.

Schedule
========

Week 1 (4/10):
  Research and implement state-of-the-art numerical algorithms for solving
  ill-conditioned linear systems.

Week 2 (4/17):
  Develop test cases and follow test-driven development.

Week 3 (4/24):
  Implement the C++ library functions and develop the numpy ndarray interface.

Week 4 (5/1):
  Refactoring week (and accommmodation of overflown work).

Week 5 (5/8):
  Add parallelization by introducing third-party thread manging library.

Week 6 (5/15):
  Optimize the library performance and finalize the API documentation.

Week 7 (5/22):
  Refactoring week (and accommmodation of overflown work).

Week 8 (5/29):
  Refine the library based on issues and develop a roadmap for future
  development.

References
==========

* Trefethen, L. N. (1997). Numerical linear algebra. SIAM.
* Demmel, J. (1997). Applied numerical linear algebra. SI
