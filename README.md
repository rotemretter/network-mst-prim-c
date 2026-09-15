# Network Minimum Spanning Tree & Route Finder

A modular C application that optimizes network topology by finding a Minimum Spanning Tree (MST) and determines communication paths between computers using graph algorithms.

## Features
- **Graph Representation:** Adjacency lists using dynamically allocated linked lists with sorted neighbor insertion.
- **Minimum Spanning Tree (MST):** Implements Prim's algorithm with a custom priority queue (doubly linked list) and bit-array tracking.
- **Routing & Pathfinding:** Constructs tree paths and uses Depth-First Search (DFS) with 3-color node tracking (White, Gray, Black) to calculate communication routes between computers.
- **Memory Management:** Fully handles dynamic allocations with dedicated cleanup routines.

## Project Structure
- `graph.h` - Data structure definitions and function prototypes.
- `def.c` - Core graph algorithms (Prim's MST, adjacency lists, and DFS routing).
- `freefunc.c` - Memory deallocation functions.
- `main.c` - Program entry point and execution flow.

## Compilation & Execution
Compile all source files using `gcc`:
```bash
gcc -Wall -Wextra -std=c99 main.c def.c freefunc.c -o network_solver
