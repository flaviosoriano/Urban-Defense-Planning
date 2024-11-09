#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <limits>

#include <string>

#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <map>
#include <set>

#define INF std::numeric_limits<int>::max()

struct node {
    std::string City_name;
    std::vector<node*> roads;

    node(std::string name){
        City_name = name;
        roads = std::vector<node*>();
    }
};

class Graph {
private:

    std::unordered_map<std::string, node*> cities;
    int size;
    std::vector<Graph*> sccs;

    node *capital;

public:
    Graph();
    ~Graph();

    void addRoad(std::string city1, std::string city2);
    void addCity(std::string city);

    std::unordered_map<std::string, node*> getCities();
    node* getCapital();
    int getSize() const;
    void printGraph() const;

    int BFS(node& originCity) const;
    void defineCapital();
    void BatalhaoPrincipal(node& stateCapital);

    void DFS(node* city, std::unordered_map<node*, bool>* visited, std::stack<node*>* stack) const;
    void Kosaraju_DFS(node* city, std::unordered_map<node*, bool>* visited, Graph* component) const;
    void Kosaraju();
    void DefineBatalhoes() ;
};

#endif