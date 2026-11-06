# Conway's Game of Life
Conway's Game of Life with c++ 23, SFML 3.1 and ImGui.
The project uses a 1D grid, double-buffered board, and draws the cells using vertices.

![Conway's Game Of Life Sun](Screenshots/sun.gif)
# Building
The project uses CMake and fetches its dependencies automatically using FetchContent.

Requirements:

- CMake 3.28+
- C++23 compatible compiler
- Git

Clone the repository and configure/build it with CMake.

# Controls
- Left Mouse: Place cell
- Right Mouse: Remove cell
- WASD: Move view
- Mouse Wheel: Zoom
- Spacebar: Play/Pause
- G: Draw grid
- R: Randomize cells
- C: Clear cells
- H: Reset view

# About
This was a learning project on cellular automata, simulation logic.

I learned a lot about rendering a large number of cells efficiently using vertices and triangles, using sf::view, array/grid indexing and probably other things...
It took me longer than I initially expected because I suck at maths.

This is also the first project where I have been able to see value out of optimization which was very cool.
