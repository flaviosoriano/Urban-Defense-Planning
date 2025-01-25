# Urban Defense Planning

## Introduction
This project addresses a computational problem inspired by a fictional scenario in which the emperor of Archadia seeks to reinforce the security of his empire against invasion threats. To achieve this, it is necessary to reorganize the geographical states using graph concepts to model urban centers and the roads connecting them.

The challenges to be solved include identifying the best urban center to serve as the capital, ensuring the most efficient access to all locations; determining the minimum number and optimal locations for secondary battalions to allow all troops to return to bases; and evaluating the feasibility of preventive patrol routes covering all roads and returning to the starting point. These problems are addressed using algorithms that consider optimization and strategic efficiency criteria.

## Modeling
To solve this problem, it was modeled as a directed unweighted graph, where cities are vertices and roads are edges directed from the departure city to the arrival city. The graph was implemented using a linked list and consists of attributes such as the capital (pointer to the capital node), cities (linked list of vertices), size (stores the graph size), and SCC (vector storing the strongly connected components as independent graphs to facilitate the patrol task). Additionally, vertices are represented as a struct storing the city name and a list of its neighbors (i.e., cities to which there is a road).

### Part I: Defining the Capital
This part was performed using BFS from each vertex of the graph and calculating the sum of the distances to each city. The city with the smallest sum of distances is chosen as the capital.

### Part II: Battalions
To determine the strategic locations for battalions, Kosaraju's algorithm was used to identify the strongly connected components (SCCs). Once the SCCs were determined, battalions were placed by running BFS from the capital and selecting the closest SCC node (in case of ties, an internal BFS within the SCC was run to determine the node with the shortest total distance to all component vertices).

### Part III: Patrols
For the patrol planning, the Chinese postman problem was used as inspiration, since the graphs are not necessarily Eulerian. The solution involves balancing the SCCs to make them Eulerian and then applying Hierholzer's algorithm to determine the patrol route.

## Solution

### Part I: Defining the Capital
To identify the optimal capital, a BFS-based approach was used to compute the sum of the shortest distances from each city to all others. The city with the lowest total distance is selected as the capital, ensuring rapid access to all areas.

Steps:
1. **Breadth-First Search (BFS)** to compute shortest distances.
2. **Capital identification:** The city with the minimum accumulated distance is chosen.

### Part II: Battalions
Kosaraju's algorithm was used to identify SCCs in the graph. Each SCC is stored as an independent graph in an array, and a battalion is strategically placed inside each one based on the BFS search criteria.

Steps:
1. **Identify SCCs:** Using Kosaraju’s algorithm to find mutually reachable nodes.

2. **Determine battalion locations:** BFS is used to find the closest node to the capital.

### Part III: Patrol Planning
For patrol route planning, the graph is balanced to become Eulerian, and Hierholzer's algorithm is used to compute the patrol path.

Steps:
1. **Balancing SCCs:** Adjusting the graph by adding necessary edges.

2. **Eulerian path calculation:** Applying Hierholzer’s algorithm.

## Complexity Analysis

### Part I: Defining the Capital
- **Time Complexity:** O(V × (V + E))
- **Space Complexity:** O(V)

### Part II: Battalion Placement
- **Time Complexity:** O(V × (V + E))
- **Space Complexity:** O(V + E)

### Part III: Patrol Planning
- **Time Complexity:** O(V² + E)
- **Space Complexity:** O(V² + E)

## Conclusion
This project provided a deep understanding of graph algorithms and their practical application in solving complex problems. Dividing tasks into clear stages, such as defining the capital, planning battalions, and creating patrol routes, facilitated modular and optimized solutions.

The most challenging aspect was implementing graph balancing for patrols, as it required efficient handling of unbalanced pairs. On the other hand, identifying SCCs and defining capitals were more straightforward due to well-established algorithms like Kosaraju and BFS.

## References
1. Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. Introduction to Algorithms. MIT Press, 3rd Edition, 2009.
2. Sedgewick, R., & Wayne, K. Algorithms. Addison-Wesley, 4th Edition, 2011.
