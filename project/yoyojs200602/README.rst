=======================================================
GACR: A Genetic Algorithm-based Channel Router
=======================================================
A C++ channel router utilizing genetic algorithm, with a Python interface provided.


Basic Information
========================
Channel routing is a crucial step in physical design. It deals with determining the optimal layout of wires within a specified area called channel, which is a rectangular region between two rows of pins. The goal is to connect pins with wires without overlaps while minimizing the number of tracks used and total wirelength.

Genetic algorithm is inspired by the principles of natural selection and genetics, representing a population-based optimization technique that can provide efficient and effective solutions to complex problems.


Problem to Solve
========================
Traditional heuristic algorithms for channel routing tend to be deterministic and problem-specific, which can lead to suboptimal solutions, especially when dealing with complex and large-scale designs. Genetic algorithms, on the other hand, offer several advantages over heuristic approaches:

**1. Global optimization**

Genetic algorithms search the solution space more broadly, decreasing the likelihood of getting trapped in local optima.

**2. Adaptability** 
 
Genetic algorithms can be easily adapted to different types of routing problems and constraints, making them more versatile.

**3. Parallelism** 
 
Genetic algorithms operate on a population of solutions, which enables natural parallelism and potentially faster convergence.
By applying genetic algorithms to channel routing problems, the router can provide better routing solutions compared to conventional heuristic methods.

Prospective Users
========================
The target users include researchers, students, and professionals in electronic engineering, computer science, and VLSI design, as well as software developers and engineers working on electronic design automation (EDA) tools that require efficient channel routing solutions.

System Architecture
========================
The library will be developed in C++ without depending on Python. The system may consist of the following modules and classes:

``GeneticAlgorithm``: implementation of genetic algorithm with selection, crossover, and mutation operations for channel routing problem.

``Population``: manage the population of routing solutions.

``Individual``: representation of a routing solution and its fitness score.

``RoutingGraph``: model the routing problem as a graph.

``Channel``: representation of the rectangular channel.

A Python Wrapper will expose C++ functionality to Python using pybind11.

API Description
========================
The following API will be provided in C++ (and Python)

``GeneticAlgorithm(top_signal, bottom_signal)``: provide constructors for creating instances of the channel routing solver.

``initial_population(n)``: create initial routing soultions.

``selection()``: select a parent individual.

``crossover(i1, i2)``: crossover on selected parent individuals.

``mutation(i)``: mutation on an individual.

``fitness_calculation(i)``: calculate fitness score of an individual.

Engineering Infrastructure
============================
The engineering infrastructures will include the following:

**1. Unit Testing**

Python unit-testing framework will be used, providing unit tests in Python. Public API will be wrapped to Python for testing.

**2. Continuous Integration (CI)**

GitHub Actions will be configured to run tests regularly and for each commit and pull request.

**3. Documentation**

Comprehensive documentation will be provided, including installation instructions, usage guides, and API references.


Schedule
============================
**Week 1 (4/10):**
    Study existing algorithms. Design data structure. CI and testing features will be added along the development process.

**Week 2 (4/17):**
    Develop initial populaition, selection, crossover stategies.

**Week 3 (4/24):**
    Develop reduction, mutation stategies.

**Week 4 (5/1):**
    Refactoring week (debugging and accomodation of overflown work)

**Week 5 (5/8):**
    Create Python interface using pybind. Visualization will also be provided. 

**Week 6 (5/15):**
    Further optimization, including parameter tuning, performance enhancement or some new techniques.

**Week 7 (5/22):**
    Refactoring week (write documentation and organize data for the presentation, including routing graphs and performance comparison with other algorithms)

**Week 8 (5/29):**
    Refactoring week (overflown work, prepare for the presentation)

References
===========
1. `A Genetic Algorithm for Channel Routing in VLSI Circuits <https://ieeexplore.ieee.org/document/6792156?denied=>`_
2. `A Parallel Genetic Algorithm for Two Detailed Routing Problems <https://ieeexplore.ieee.org/document/542012>`_
