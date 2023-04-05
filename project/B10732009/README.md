# Procedural Terrain Generation #

Procedurally generate random terrain with **[Perlin noise](https://en.wikipedia.org/wiki/Perlin_noise)**.

## Basic Information ##

GitHub repository : [https://github.com/B10732009/procedural-terrain-generation](https://github.com/B10732009/procedural-terrain-generation)

**[Procedural Generation](https://en.wikipedia.org/wiki/Procedural_generation)** is a method of **creating data algorithmically** as opposed to manually, typically through a combination of human-generated assets and algorithms coupled with computer-generated randomness and processing power. 

In video games,  **Procedural Terrain Generation** is very useful where you want generate natural terrain (caves, hills, rivers, etc.) that has a smooth feel, but is still random.

<p style="text-align: center;">
    <img src="img/minecraft.png" alt="Compare"><br>
    Minecraft, a 3D graphic game <br>that has many kinds of terrain generation.
</p>

## Problem to Solve ##

**Perlin Noise** is very pouplar an algorithm for procedural generation developed by Ken Perlin in 1983. It can be used for any sort of wave-like, undulating material, texture, or terrain. 

Compared with just some random values, Perlin Noise can generate values very smoothly and continuously, which looks more realistic in terrain generation. 

The image below shows the difference between normal random values and perlin noise.

<p style="text-align: center;">
    <img src="img/compare.png" alt="Minecraft">
</p>

In this project, I am going to implement a set of APIs of the Perlin Noise algorithm in 1D, 2D, and 3D (if time is enough), and use these APIs to render some terrain.

## Prospective Users ##

## System Architecture ##

## API Description ##

## Engineering Infrastructure ##

- Automatic build system : GNU Make
- Version control : git
- Testing framework : pytest
- Documentation : README in the github repository 

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

- [Procedural Generation (Wikipedia)](https://en.wikipedia.org/wiki/Procedural_generation)
- [Perlin noise (Wikipedia)](https://en.wikipedia.org/wiki/Perlin_noise)
- [Minecraft terrain generation in a nutshell (YouTube)](https://www.youtube.com/watch?v=CSa5O6knuwI)