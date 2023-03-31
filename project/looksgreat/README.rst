========================
SPER: SMT-Based PCB Escape Routing 
========================

A project that transform PCB escape routing problem to SMT problem and use SMT solver to solve it.

Basic Information
=================

Github repository: 

The satisfiability modulo theories (SMT) is a more general form of the SAT (satisfiability problem). In addition to boolean variable, it can also deal with integer, and even if-else and many more statement in first order logic.

Problem to Solve
================

PCB escape routing is one of the problems in EDA. In this problem, we have a grid map with multiple pins on it, and several fan-out around the border of the grip map. The goal is to find a path for each pin to fan-out, and each path should not cross each other. 

The problem is NP-hard, and there are many heuristic algorithms to solve it. However, the heuristic algorithms are not always optimal, and it is hard to find a good heuristic algorithm.

In this project, I will transform the problem to SMT problem, and use SMT solver to solve it. The SMT solver I use here will be Z3(https://github.com/Z3Prover/z3).

Prospective Users
=================

PCB factory that want to solve PCB routing problem.

The software will contain multiple option, for example, user can decide whether each fan-out's signal is given by user or decide by the software. Moreover, the software offer two modes, one is aim at minimize path length, which will reduce the production cost. The other mode is aim at making each path length as similar as possible, which will reduce the delay difference between each signal.

System Architecture
===================

This software will be developed in C++ and use the API provided by z3, and use pybind to support python user.

The system architecture consist of three parts:

* A class serve as a interface between z3 API and our software. Since z3 is a general tool for many problems, this interface will help us develop easier.
* A core part that transform the problem to SMT problem.
* An interface that allow user to use the software.

API Description
===============

* createBoard(x, y): create a board with x*y grid.
* addPin(x, y): add a pin at (x, y).
* addFanout(x, y, signal(optional)): add a fanout at (x, y). If signal is not given, the software will decide the signal for this fanout.
* setMode(mode): set the mode of the software. The mode can be "minimize" or "balance".

Engineering Infrastructure
==========================

* Automatic build system: GNU make
* Version control: git
* Testing framework: pytest
* Documentation: README.md

Schedule
========

* Week 1 (4/10): Study Z3 document
* Week 2 (4/17): Design data structures
* Week 3 (4/24): Impelment transformation algorithm
* Week 4 (5/1): Refactoring week (and accommodation of overflowing work).
* Week 5 (5/8): Impelment transformation algorithm and API
* Week 6 (5/15): Write test
* Week 7 (5/22): Refactoring week (and accommodation of overflowing work).
* Week 8 (5/29): Visualize using klayout and write documentation

References
==========

Z3: An Efficient SMT Solver, Leonardo de Moura and Nikolaj Bjørner(https://dl.acm.org/doi/10.5555/1792734.1792766)
