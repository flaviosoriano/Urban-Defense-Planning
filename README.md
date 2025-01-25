# Urban Defense Planning

## Overview
This project is a practical assignment developed for the **Algorithms I** course at UFMG. The objective is to design an efficient urban defense strategy for the fictional empire of Archadia by leveraging graph theory algorithms to address key logistical challenges.

## Problem Statement
The emperor of Archadia seeks to defend the empire against potential invasions by defining optimal strategies for selecting capitals, positioning military battalions, and planning patrol routes within various states. The solution must consider the following aspects:

1. **Capital Selection:** Identifying the most strategic urban center to serve as the state's capital, ensuring minimal travel time to all other centers.
2. **Battalion Placement:** Determining the minimum number of secondary battalions needed to allow troops to return to a central point.
3. **Patrol Planning:** Evaluating the feasibility of patrol routes that ensure all roads are covered and troops can return to the starting point.

## Solution Approach
The problem is modeled as a **directed unweighted graph**, where:
- **Nodes (Vertices):** Represent urban centers.
- **Edges:** Represent one-way roads connecting the cities.
- **Graph Algorithms Used:**
  - **BFS (Breadth-First Search):** To determine the optimal capital based on shortest paths.
  - **Kosaraju's Algorithm:** To identify strongly connected components (SCCs) for battalion placement.
  - **Hierholzer's Algorithm:** To construct Eulerian paths for patrol route planning.

## Input & Output
The program processes an input file describing the state's urban network with the following format:

### Input Example
```
6 8
Rhedrise Vandrad
Vandrad Benith
Khudealine Thonet
Thonet Khudealine
Rhedrise Khudealine
Benith Vandrad
Vandrad Muafland
Muafland Vandrad
```

### Expected Output
```
Rhedrise
2
Khudealine
Vandrad
2
Khudealine Thonet
Vandrad Muafland Vandrad Benith
```

### Explanation of Output
1. **Capital:** `Rhedrise` is selected as the capital based on minimum distance sum.
2. **Battalions:** `2` secondary battalions are required at `Khudealine` and `Vandrad`.
3. **Patrol Routes:** Two patrol routes covering all roads are proposed.

## Technologies Used
- Programming Language: **C / C++**
- Graph Algorithms: BFS, Kosaraju, Hierholzer
- Compilation: GCC with strict compilation flags

## Build & Run Instructions
### Compilation
Ensure you have a GCC compiler installed and run the following command to compile the project:

```sh
make
```

### Execution
Run the compiled program with an input file:

```sh
./tp1 < testCase01.txt
```

## Project Structure
```
.
├── src/            # Source code files
├── include/        # Header files
├── tests/          # Test case files
├── docs/           # Documentation and reports
├── Makefile        # Compilation script
└── README.md       # Project overview
```

## Compilation Flags Used
The project is compiled with the following flags to ensure high-quality code:

```sh
-Wall -Wextra -Wpedantic -Wformat-security -Wconversion -Werror
```

## Complexity Analysis
### Capital Selection
- **Time Complexity:** `O(V × (V + E))`
- **Space Complexity:** `O(V)`

### Battalion Placement
- **Time Complexity:** `O(V × (V + E))`
- **Space Complexity:** `O(V + E)`

### Patrol Planning
- **Time Complexity:** `O(V² + E)`
- **Space Complexity:** `O(V² + E)`

## Documentation
The documentation provides:
- **Problem understanding and model**
- **Algorithm explanations**
- **Complexity analysis**
- **Challenges faced and solutions adopted**

Find the full documentation in the `docs/` directory.

## How to Contribute
Feel free to fork the repository, submit pull requests, and report any issues you encounter.

## License
This project is released under the MIT License.
