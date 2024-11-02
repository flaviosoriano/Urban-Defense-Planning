#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>

struct node{
    std::string City_name;
    std::vector<node*> roads;

    node(std::string name){
        City_name = name;
        roads = std::vector<node*>();
    }
};

class Graph
{
private:

    std::vector<node*> cities;
    int size;

    node *capital;

public:
    Graph(int size);
    ~Graph();

    void addNode(std::string name);
};

#endif