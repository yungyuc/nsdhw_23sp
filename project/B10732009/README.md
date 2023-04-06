# Procedural Terrain Generation #

Procedurally generate random terrain with [Perlin Noise](https://en.wikipedia.org/wiki/Perlin_noise).

## Basic Information ##

GitHub repository : [https://github.com/B10732009/procedural-terrain-generation](https://github.com/B10732009/procedural-terrain-generation)

**[Procedural Generation](https://en.wikipedia.org/wiki/Procedural_generation)** is a method of **creating data algorithmically** as opposed to manually, typically through a combination of human-generated assets and algorithms coupled with computer-generated randomness and processing power. 

In video games,  **Procedural Terrain Generation** is very useful where you want generate natural terrain (caves, hills, rivers, etc.) that has a smooth feel, but is still random.

## Problem to Solve ##

**Perlin Noise** is a very popular algorithm for procedural generation developed by Ken Perlin in 1983. It can be used for any kind of wave-like, undulating material, texture, or terrain. 

Compared with just some random values, Perlin Noise can generate values very smoothly and continuously, which looks more realistic in terrain generation. 

The graph below shows the difference between normal random values and perlin noise.

<p style="text-align: center;">
    <img src="img/compare.png" alt="compare">
</p>

In this project, I am going to implement a set of API (for C++ and Python) of the Perlin Noise algorithm in 1D, 2D, and 3D (if time is enough), and use these API to render some terrain with [Ursina](https://www.ursinaengine.org/) (a game engine in Python).

## Prospective Users ##

Anyone who wants to simulate random terrain or uses Perlin Noise for application.

## System Architecture ##

- Use `C++` to implement the algorithm.
- Use `Pybind11` to wrap C++ functions for Python.
- Use `Python` to render the terrain with these APIs (with [Ursina](https://www.ursinaengine.org/)).

<p style="text-align: center;">
    <img src="img/system_architecture.png" alt="system_architecture">
</p>

## API Description ##

The API will have both C++ and Python version.

```cpp
// C++ API
// get the value of specific coordinate
double getNoise1D(int seed=0, int octaves=1, double lacunarity=2.0, double persistance=0.5, double x);
double getNoise2D(int seed=0, int octaves=1, double lacunarity=2.0, double persistance=0.5, double x, double y);
double getNoise3D(int seed=0, int octaves=1, double lacunarity=2.0, double persistance=0.5, double x, double y, double z);

// get the values of a specific length/area/space
std::vector<double> getNoises1D(int seed=0, int octaves=1, double lacunarity=2.0, double persistance=0.5, double x);
std::vector<std::vector<double>> getNoises2D(int seed=0, int octaves=1, double lacunarity=2.0, double persistance=0.5, double x, double y);
std::vector<std::vector<std::vector<double>>> getNoises3D(int seed=0, int octaves=1, double lacunarity=2.0, double persistance=0.5, double x, double y, double z);
```

```py
# Python API
# get the value of specific coordinate
def getNoise1D(seed=0, octaves=1, lacunarity=2.0, persistance=0.5, x);
def getNoise2D(seed=0, octaves=1, lacunarity=2.0, persistance=0.5, x, y);
def getNoise3D(seed=0, octaves=1, lacunarity=2.0, persistance=0.5, x, y, z);

# get the values of a specific length/area/space
def getNoises1D(seed=0, octaves=1, lacunarity=2.0, persistance=0.5, x);
def getNoises2D(seed=0, octaves=1, lacunarity=2.0, persistance=0.5, x, y);
def getNoises3D(seed=0, octaves=1, lacunarity=2.0, persistance=0.5, x, y, z);
```

## Engineering Infrastructure ##

- Automatic build system : `GNU Make`
- Version control : `git`
- Testing framework : `pytest`
- Documentation : `README.md` in the github repository 

## Schedule ##

- Week 1 (4/10)
    - Study the algorithm and related knowledge.
- Week 2 (4/17)
    - Implement the 1D perlin noise algorithm.
- Week 3 (4/24)
    - Implement the 2D perlin noise algorithm.
- Week 4 (5/01)
    - Implement the 3D perlin noise algorithm (if time is enough).
    - Code refactoring.
- Week 5 (5/08)
    - Setup python rendering environment.
    - Try to render the terrain.
- Week 6 (5/15)
    - Add details to the terrain.
        - Color
        - Textures
        - Falloff map
- Week 7 (5/22)
    - Write test.
    - Code refactoring.
- Week 8 (5/29)
    - Write document & prepare for presentation. 

## References ##

- [Procedural Generation (wikipedia)](https://en.wikipedia.org/wiki/Procedural_generation)
- [Perlin Noise (wikipedia)](https://en.wikipedia.org/wiki/Perlin_noise)
- [Minecraft terrain generation in a nutshell (YouTube)](https://www.youtube.com/watch?v=CSa5O6knuwI)
- [Procedural Terrain Generation (YouTube)](https://www.youtube.com/playlist?list=PLFt_AvWsXl0eBW2EiBtl_sxmDtSgZBxB3)
- [Understanding Perlin Noise (article)](https://adrianb.io/2014/08/09/perlinnoise.html)
- [Improving Noise by Ken Perlin](https://mrl.cs.nyu.edu/~perlin/paper445.pdf)