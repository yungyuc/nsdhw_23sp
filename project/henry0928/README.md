# Voronoi Diagram and Delaunay Triangulation

GitHub repository : https://github.com/henry0928/Voronoi-Diagram-and-Delaunay-Triangulation

# Basic Information

To put it simply, `Voronoi Diagram` is a kind of graphic segmentation. There will be many specific points on a planar graph. The collection of these specific points is called "A". All the points on the planar graph will go to "A" to find the specific point closest to itself, and form an independent area with the specific point. Then, the planar graph can be divided into various independent areas by using specific points to form a voronoi diagram.

`Delaunay Triangulation`, A triangulation method invented by the mathematician Delaunay in 1934, that is, assuming that there is a set P formed by several points on the plane, then we can connect these points with lines, and any triangle formed by these line segments A circumscribed circle that does not have any other points inside it.

In fact, `Delaunay Triangulation` and `Voronoi Diagram` are dual graphics.
We can use Delaunay Triangulation to prodece Voronoi Diagram or use Voronoi Diagram to produce Delaunay Triangulation, either way will be possible.
 
# Problem to Solve

Both Voronoi Diagram and Delaunay Triangulation play an important role in the fields of facial recognition and geographic data analysis. We can use Voronoi Diagram to analyze the scope of responsibility of each fire brigade. We can also use Delaunay Triangulation to realize face transformation and facial features replacement.

I'm trying to...
1. Use `Fortune's Algorithm` to implement Voronoi Diagram and show the result graph with multiple points.
2. Use Voronoi Diagram, which is created by us, to prodece Delaunay Triangulation on the same planar graph ( if time is enough ).

# Fortune's Algorithm

`Fortune's Algorithm` mainly consists of two parts: Sweep Line and Beach Line.

### Sweep Line :
`Sweep Line` is a vertical or horizontal line, assuming a horizontal line is used, and moves from top to bottom. During the movement of the Sweep Line, every time an input point is passed, the input point will be included in the calculation of the `Beach Line`, forming a parabolic segment with an upward opening

### Beach Line: 
Each input point passed by the `Sweep Line` will form the focus of the parabola, and the `Sweep Line` will be used as the directrix of the focus to form a parabola.
The `Beach Line` consists of the parabola formed by all the input points included in the calculation.

### Event:
The two major events that have impact on the diagram are the following:

1. `Site Event`: Occurs when the Sweep Line passes through any input point, and the passed input point will be included in the calculation and change the Beach Line to form the sides of the Voronoi diagram.
2. `Circle Event`: Occurs when any parabolic segment on the Beach Line disappears. At this time, there will be a special point that reaches 3 different site points and the Sweep Line is equidistant. This special point forms the vertex of the Voronoi Cell

### Pseudo Code:
```
create the event queue with site events for each input points
 
while the event queue is not empty
 
    if the first event in the event queue is a site event
        add the new parabola to the beach line
 
    if the first event in the event queue is a circle event
        add the new vertex and squeeze the parabola
 
finish any remaining edges
```
# Prospective Users

Anyone who wants to generate Voronoi Diagram and Delaunay Triangulation.

# System Architecture

This is a software system of a hybrid system of C++11 (modern C++) and Python.
- Use `Python` to read input (target points, the boundary of the planar graph) and show results.
- Use `C++` to implement "Fortune's Algorithm" and then generate Voronoi Diagram.
- Use `Pybind11` to wrap C++ functions for Python.

# API Description

 The `result_diagram` here does not refer to the complete graph, but refers to the information containing all the edges and site points in the `result_diagram`, and draws a complete graph by using these information.

`Python` API: <br>
``` py 
def Create_diagram(site_list, bb_point1, bb_point2) # Input the site's list and bounding box points to get the Voronoi Diagram as the result_diagram 
def Draw_diagram(result_diagram) # Draw the result diagram
```

`Voronoi()` will get the edge information of Voronoi Diagram, user can use this function to get the edge information rathen than the complete graph. 

`C++` API: <br>
``` cpp
Edge Voronoi(vector<Point> site_list, Point bb_point1, Point bb_point2 )
```

( If it is found that the API needs to be added during the development process, I will modify this part again. )


# Engineering Infrastructure

- Build: `make`
- Version control: `git`
- Testing infrastructure: `pytest`
- Documentation: `README.md`

# Schedule

- Week 1 (4/10)
    - Study the related knowledge, algorithm and finish proposal
- Week 2 (4/17)
    - Implement `Point`, `Edge`, `BeachLine` and `EventQueue` datastructures (C++)
- Week 3 (4/24)
    - Add functionality to `EventQueue` (C++)
- Week 4 (5/01)
    - Add functionality to `BeachLine` (C++)
- Week 5 (5/08)
    - Testing and debugging (C++)
    - Use Voronoi Diagram to prodece Delaunay Triangulation ( if time is enough ) (C++)
- Week 6 (5/15)
    - Code refactoring. (C++)
    - Implement `Create_diagram()` and `Draw_diagram()` (Python)
- Week 7 (5/22)
    - Code refactoring. (Python)
    - Documentation and buffer for previous weeks
- Week 8 (5/29)
    - Prepare for presentation 