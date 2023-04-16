# Maximum Graph Matching Library (MGML)

## Basic Information

In graph theory, a matching is a set of non-adjacent edges in a graph,
so the maximum matching problem is to find the maximum set in terms of
cardinality or weight.

## Problem to Solve

There are different algorithms for different situations. This project
is trying to implement some popular algorithms in C++ and to also
provide a API in python. Here are the problems I am going to solve.

* Maximum Cardinality Bipartite Matching
	* Hopcroft-Karp Algorithm ($O(|E|\sqrt{|V|})$)
* Maximum Weight Bipartite Matching
	* Hungarian Algorithm ($O(|V|^3)$)
* Maximum Cardinality Matching in arbitrary graphs
	* Blossom Algorithm ($|V||E|\alpha(|V|)$)
	* MV Matching Algorithm (**optional**)
* Maximum Weight Matching in arbitrary graphs
	* Blossom Algorithm (**optional**)

## Prospective Users

Maximum matching in bipartite graphs can be used to solve resource allocation
problems. Additionally, in general graphs, where nodes represent individuals
and edges represent social connections such as friendships, collaborations,
or interactions, finding a maximum matching can help identify optimal pairs of
individuals for potential partnerships. This has applications in social network
analysis, where it can optimize the formation of collaborations or partnerships
based on criteria such as common interests, skills, etc.

## System Architecture

The algorithms are implemented in C++ for fast runtime, and there are
interfaces in C++ for sure and in python by pybind11.

## API Description

The input will consist of a graph represented in adjacency list or
adjacency matrix, and the output will be a matching.

* `Matching MCBM(Graph g);`: Maximum Cardinality Bipartite Matching
* `Matching MWBM(Graph g);`: Maximum Weight Bipartite Matching
* `Matching MCM(Graph g);`: Maximum Cardinality Matching (in arbitrary graphs)
* `Matching MWM(Graph g);`: Maximum Weight Matching (in arbitrary graphs) (**optional**)

## Engineering Infrastructure

* Build: GNU Make
* Version control: Git (GitHub)
* Testing: unittest (python)
* Document: Markdown (README.md)

## Schedule

Week 1 (4/17): Design data types, interface, and architecture

Week 2 (4/24): Write some unit tests

Week 3 (5/01): Implement Hopcroft-Karp Algorithm

Week 4 (5/08): Implement Hungarian Algorithm

Week 5 (5/15): Implement Blossom Algorithm (Maximum Cardinality Matching)

Week 6 (5/22): Make presentation slides (and maybe try a optional one)

Week 7 (5/29): Prepare for the presentation

## References

* [wikipedia - matching](https://en.wikipedia.org/wiki/Matching_(graph_theory))

