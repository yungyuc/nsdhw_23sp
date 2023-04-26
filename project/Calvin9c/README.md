# RayMarching

## Basic Information

This project is primarily use OpenGL as the tool to design a work similar to those shared on [Shadertoy](https://www.shadertoy.com/browse).

GitHub Repository: [RayMarching](https://github.com/Calvin9c/RayMarching.git)

## Problem to Solve

Signed Distance Field (SDF) is a method of representing geometric shapes by storing the distance and sign (inside or outside) of each point in space to the nearest surface, providing an accurate representation of the geometric shape.
RayMarching is a computer graphics rendering technique based on SDF. The basic idea of Raymarching is to cast a ray from the camera or eye, move along the ray until it reaches an object or boundary in the scene.
In this process, RayMarching uses SDF to quickly and accurately calculate properties such as color, shadows, reflections, and refractions for each pixel.
In addition, RayMarching can achieve better performance and speed by adjusting the sampling density and other parameters, making it suitable for real-time rendering.

In this project, I want to utilize the fast calculation capabilities of C++ and RayMarching, with OpenGL as the primary development tool, to create a work similar to this [video (at 14:09)](https://youtu.be/hUaYxqkrfjA?t=849).

## Prospective Users

This project provides a template for game developers who need to generate scenes in real-time.
By modifying graphic parameters, the developers can generate their desired scenes.

## System Architecture

1. Write `shader` programs using `GLSL` to achieve specific visual effects.
2. Using C++ to design classes related to rendering graphics.
3. Using `pybind` to make API in Python.

## API Description

First, I will define a class called "my_app" by using C++. In this class, in addition to the model parameters, some required numerical calculations will also be included.

Next, there will be a function called "get_user_input()" that receives the user's input for setting the model parameters.

Finally, there will be a function called "pass()" that pass the set model parameters to the Shader for calculation and then outputs the result.

## Engineering Infrastructure
* Version Control: `git`
* Automatic Build System: `CMake`
* Tool Selection: `OpenGL`
* Testing Framework: `pytest`
* Documentation: `README.md`

## Schedule
| Week | Description |
| :-: | :- |
| 1 (4/10) | 1. Read document related to OpenGL.<br>2. Set up the environment and install the necessary tools for use. |
| 2 (4/17) | 1. Define the APIs for each process roughly. <br>2. Design a simple input to the program and check its output. Test the entire process for feasibility and identify the part that need modification. |
| 3 (4/24) | 1. Adjust the process if necessary.<br>2. Define the APIs for each process in detail. |
| 4 (5/01) | 1. Design the scene that I want to present to user. |
| 5 (5/08) | Develop user interface. |
| 6 (5/15) | 1. Refactor. <br>2. Deal with the unfinished parts from the previous weeks of this project (if any). |
| 7 (5/22) | Fine tune the overall process based on issues (if any). |
| 8 (5/29) | Making slide and preparing a script for presentation. |

## References
1. [Opengl Tutorial](https://learnopengl.com/)
2. [Procedural 3D Engine. RayMarching OpenGL Tutorial](https://www.youtube.com/watch?v=hUaYxqkrfjA)
3. [The Implementation of RayMarching sharing on Shadertoy](https://www.shadertoy.com/browse)
