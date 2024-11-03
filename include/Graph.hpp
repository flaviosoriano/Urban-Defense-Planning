#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <limits>

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

#define INF std::numeric_limits<double>::infinity()


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

    node *capital;

public:
    Graph(int size);
    ~Graph();

    void addRoad(std::string city1, std::string city2);


    std::unordered_map<std::string, node*> getCities();
    node* getCapital();

    double BFS(node& originCity) const;
    void printGraph() const;

    void defineCapital();
};

#endif