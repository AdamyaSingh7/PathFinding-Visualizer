# Pathfinding Visualizer

A graphical application built using **SFML 3.0.0** and **C++** to visualize various pathfinding algorithms on a grid-based interface. Designed with an intuitive GUI that allows users to place walls, set start/end points, and visualize algorithms in action with interactive buttons.

---

## Features

* **Grid-based UI** with clickable cells
* Visualize 4 popular algorithms:

  * A\* Search
  * Dijkstra's Algorithm
  * Breadth-First Search (BFS)
  * Depth-First Search (DFS)
* GUI buttons for algorithm selection, run, reset, and quit
* Animated algorithm progression
* Wall creation and dynamic start/end node assignment
* "No path found" warning display

---

### Requirements

* C++ compiler (tested with g++ via MinGW)
* [SFML 3.0.0](https://www.sfml-dev.org/download.php)
* CMake
* CLion (optional, any IDE works)

### Building

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/PathfindingVisualizer.git
   cd PathfindingVisualizer
   ```

2. Setup SFML 3.0.0 in your project

   * Link include and lib paths correctly
   * Ensure the `assets/arial.ttf` font file is available

3. Build with CMake:

   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

4. Run the executable:

   ```bash
   ./PathfindingVisualizer
   ```

---

## Usage

### Mouse Interactions

* **Left Click**:

  * First click: Set **Start** node
  * Second click: Set **End** node
  * Following clicks: Place **Walls**
* **Right Click**: Clear selected cell

### Keyboard Shortcuts

* `R` = Reset grid
* `Q` = Quit application

### Buttons

* **A**\*, **Dijkstra**, **BFS**, **DFS**: Select algorithm
* **Run**: Execute the selected algorithm
* **Reset**: Clear the board
* **Quit**: Exit application

---

## Algorithms Overview

### A\* Search

* Uses both cost-so-far and heuristic to find optimal path
* Time: O(E)
* Space: O(V)

### Dijkstra

* Greedy BFS using distance priority
* Time: O(E log V)
* Space: O(V)

### BFS

* Explores neighbors level by level
* Time: O(V + E)
* Space: O(V)

### DFS

* Explores as deep as possible before backtracking
* Time: O(V + E)
* Space: O(V)

---

## Project Structure

```
PathfindingVisualizer/
├── include/
│   ├── grid.h
│   └── button.h
├── src/
│   ├── main.cpp
│   ├── grid.cpp
│   └── button.cpp
├── assets/
│   └── arial.ttf
├── CMakeLists.txt
└── README.md
```

---

## Acknowledgements

* [SFML Library](https://www.sfml-dev.org/) for graphics and window handling
* JetBrains CLion for IDE support
---

## Author

**Adamya Singh**

* B.Tech CSE @ Jaypee Institute of Information Technology
* [LinkedIn](https://www.linkedin.com/in/your-profile) | [GitHub](https://github.com/yourusername)

---

## License

MIT License. See `LICENSE` file for more info.
